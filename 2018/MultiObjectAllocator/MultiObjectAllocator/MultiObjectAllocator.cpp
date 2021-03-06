/*!------------------------------------------------------------------------------
\file   MultiObjectAllocator.cpp

\author Garrett Conti

\brief  Project: MultiObjectAllocator
         Course: FUN
---------------------------------------------------------------------------- */

#include "MultiObjectAllocator.h"

template< typename DataType >
size_t MOA::CalcMemSize( const size_t *const ObjCounts ) noexcept
{
  {
    const size_t Count = *ObjCounts;

    if( Count )
    {
      return sizeof( DataType ) * Count;
    }
  }

  return sizeof( DataType ) * MOA::DefaultObjectCount;
}

template< typename DataType1, typename DataType2 >
size_t MOA::CalcMemSize( const size_t *const ObjCounts ) noexcept
{
  size_t Return;

  {
    const size_t Count = *ObjCounts;

    if( Count )
    {
      const size_t Count2 = *( ObjCounts + 1 );

      if( Count2 )
      {
        Return = sizeof( DataType1 ) * Count + sizeof( DataType2 ) * Count2;
      }
      else Return = sizeof( DataType1 ) * Count + sizeof( DataType2 ) * MOA::DefaultObjectCount;
    }
    else
    {
      const size_t Count2 = *( ObjCounts + 1 );

      if( Count2 )
      {
        Return = sizeof( DataType1 ) * MOA::DefaultObjectCount + sizeof( DataType2 ) * Count2;
      }
      else Return = sizeof( DataType1 ) * MOA::DefaultObjectCount + sizeof( DataType2 ) * MOA::DefaultObjectCount;
    }
  }

  return Return;
}

template< typename DataType1, typename DataType2, typename DataType3, typename ... OtherDataTypes >
size_t MOA::CalcMemSize( const size_t* const ObjCounts ) noexcept
{
  return MOA::CalcMemSize< DataType1, DataType2 >( ObjCounts ) + MOA::CalcMemSize< DataType3, OtherDataTypes... >( ObjCounts + 2 );
}

template< typename DataType>
constexpr size_t MOA::CalcMemSize() noexcept { return sizeof( DataType ) * MOA::DefaultObjectCount; }

template< typename DataType1, typename DataType2 >
constexpr size_t MOA::CalcMemSize() noexcept { return sizeof( DataType1 ) * MOA::DefaultObjectCount + sizeof( DataType2 ) * MOA::DefaultObjectCount; }

template< typename DataType1, typename DataType2, typename DataType3, typename ... OtherDataTypes >
constexpr size_t MOA::CalcMemSize() noexcept { return MOA::CalcMemSize< DataType1, DataType2 >() + MOA::CalcMemSize< DataType3, OtherDataTypes... >(); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename ... AllocatorDataTypes >
MultiObjectAllocator< AllocatorDataTypes... >::MultiObjectAllocator() noexcept : MemoryPtr( new char[ DefaultMemCapacity ] ),
SizePtr( MemoryPtr ), CapacityPtr( MemoryPtr + DefaultMemCapacity ), ObjectSpecificStorage{} {}

template< typename ... AllocatorDataTypes >
MultiObjectAllocator< AllocatorDataTypes... >::MultiObjectAllocator( const std::array< size_t, sizeof...( AllocatorDataTypes ) > &AverageObjectCount ) noexcept :
  ObjectSpecificStorage{ AverageObjectCount.data() }
{
  const size_t MemCapacity = MOA::CalcMemSize< AllocatorDataTypes... >( AverageObjectCount.data() );

  SizePtr = MemoryPtr = new char[ MemCapacity ];

  CapacityPtr = MemoryPtr + MemCapacity;
}

template< typename ... AllocatorDataTypes >
template< typename DataType >
MOA::AllocatedPtr< DataType > & MultiObjectAllocator< AllocatorDataTypes... >::Allocate() noexcept
{
  using MultiObjectStorageType = typename MOA::MultiObjectElement< DataType, AllocatorDataTypes... >::MultiObjectStorageType;

  StaticAssert( std::is_void< MultiObjectStorageType >::value, "The MultiObjectAllocator does not contain that DataType!" );

  MOA::AllocatedPtr< DataType > *Return;

  {
    auto *const ObjectStorage = reinterpret_cast< MultiObjectStorageType* >( reinterpret_cast< char* >( &ObjectSpecificStorage ) +
      ( sizeof( ObjectSpecificStorage ) - sizeof( MultiObjectStorageType ) ) );

    typename MultiObjectStorageType::AllocationBufferType *Buffer = &( ObjectStorage->InitialAllocationBuffer );

    // std::vector<> micro-optimization: _Has_unused_capacity()
    if( Buffer->_Myend() == Buffer->_Mylast() ) // If full, allocate from the slower dynamic buffers
    {
      typename MultiObjectStorageType::DynamicAllocationBufferType &DynamicBuffers = ObjectStorage->AdditionalAllocationBuffers;

      if( DynamicBuffers.empty() == false ) // Make sure we have a buffer
      {
        Buffer = DynamicBuffers._Myhead();

        if( Buffer->_Myend() == Buffer->_Mylast() ) // If full, allocate a new dynamic buffer
        {
          const size_t BufferSize = Buffer->capacity();

          Buffer = &( DynamicBuffers.emplace_front( typename MultiObjectStorageType::AllocationBufferType{} ) );

          Buffer->reserve( BufferSize );
        }
        // else emplace a new alloc ptr
      }
      else // Add to the first dynamic buffer
      {
        const size_t BufferSize = Buffer->capacity();

        Buffer = &( DynamicBuffers.emplace_front( typename MultiObjectStorageType::AllocationBufferType{} ) );

        Buffer->reserve( BufferSize );
      }
    }
    // else Emplace a new alloc ptr

    typename MOA::FreedPtrs< DataType >::FreedPtrsContainer &FreeData = ObjectStorage->FreePtrs.FreeData;

    if( FreeData.size() ) // If there are free ptrs
    {
      const auto FirstFreeObject = FreeData.lower_bound( 1 );

      Return = &( Buffer->emplace_back( MOA::AllocatedPtr< DataType >{ FirstFreeObject->second } ) );

      if( FirstFreeObject->first > 1 )
      {
        FreeData.emplace( FirstFreeObject->second - 1, FirstFreeObject->first + 1 );
      }
      /* else There is only 1 left, don't add the new ptr */

      FreeData.erase( FirstFreeObject );
    }
    else // Try the 'general' free ptrs, then try to allocate from the main block. Otherwise, resize main block making sure to fix ptrs!
    {
      if( FreePtrs.FreeData.size() )
      {
        const auto FirstFreeObject = FreeData.lower_bound( sizeof( DataType ) );

        Return = &( Buffer->emplace_back( MOA::AllocatedPtr< DataType >{ FirstFreeObject->second } ) );

        if( FirstFreeObject->first > sizeof( DataType ) )
        {
          FreeData.emplace( FirstFreeObject->second - sizeof( DataType ), FirstFreeObject->first + sizeof( DataType ) );
        }
        /* else There is only 1 left, don't add the new ptr */

        FreeData.erase( FirstFreeObject );
      }
      else // Allocate from the MemoryBlock
      {
        char *const NewSizePtr = SizePtr + sizeof( DataType );

        if( NewSizePtr < CapacityPtr )
        {
          Return = &( Buffer->emplace_back( MOA::AllocatedPtr< DataType >{ reinterpret_cast< DataType* >( SizePtr ) } ) );

          SizePtr = NewSizePtr;
        }
        else // Resize the MemoryBlock and try again
        {
          Reserve( Capacity() * 2 );

          Return = &( Buffer->emplace_back( MOA::AllocatedPtr< DataType >{ reinterpret_cast< DataType* >( SizePtr ) } ) );

          SizePtr += sizeof( DataType );
        }
      }
    }
  }

  return *Return;
}

template< typename ... AllocatorDataTypes >
void MultiObjectAllocator< AllocatorDataTypes... >::Reserve( const size_t NewSize ) noexcept
{
  DebugAssert( CapacityPtr - MemoryPtr <= NewSize, "Don't reserve the same size or less, than the existing memory block!" );

  auto *const NewMemoryPtr = new char[ NewSize ];

  for( auto &Element : FreePtrs.FreeData )
  {
    Element.second = NewMemoryPtr + ( Element.second - MemoryPtr ); // Find the offset in the existing MemoryBlock, and get the new offset in the new Block
  }

  ObjectSpecificStorage.Reserve( MemoryPtr, NewMemoryPtr );

  SizePtr = NewMemoryPtr + ( SizePtr - MemoryPtr );

  CapacityPtr = NewMemoryPtr + NewSize;

  MemoryPtr = NewMemoryPtr;
}

template< typename ... AllocatorDataTypes >
size_t MultiObjectAllocator< AllocatorDataTypes... >::Size() const noexcept { return SizePtr - MemoryPtr; }

template< typename ... AllocatorDataTypes >
size_t MultiObjectAllocator< AllocatorDataTypes... >::Capacity() const noexcept { return CapacityPtr - MemoryPtr; }

template< typename ... AllocatorDataTypes >
bool MultiObjectAllocator< AllocatorDataTypes... >::IsEmpty() const noexcept { return SizePtr == MemoryPtr; }

template< typename ... AllocatorDataTypes >
bool MultiObjectAllocator< AllocatorDataTypes... >::IsFull() const noexcept { return SizePtr == CapacityPtr; }

template< typename ... AllocatorDataTypes >
template< typename DataType >
size_t MultiObjectAllocator< AllocatorDataTypes... >::Size() const noexcept
{
  using MultiObjectStorageType = typename MOA::MultiObjectElement< DataType, AllocatorDataTypes... >::MultiObjectStorageType;

  StaticAssert( std::is_void< MultiObjectStorageType >::value, "The MultiObjectAllocator does not contain that DataType!" );

  return MultiObjectStorageType::MultiDataContainerType.Size();
}

template< typename ... AllocatorDataTypes >
template< typename DataType >
size_t MultiObjectAllocator< AllocatorDataTypes... >::Capacity() const noexcept
{
  using MultiObjectStorageType = typename MOA::MultiObjectElement< DataType, AllocatorDataTypes... >::MultiObjectStorageType;

  StaticAssert( std::is_void< MultiObjectStorageType >::value, "The MultiObjectAllocator does not contain that DataType!" );

  return MultiObjectStorageType::MultiDataContainerType.Capacity();
}

template< typename ... AllocatorDataTypes >
template< typename DataType >
bool MultiObjectAllocator< AllocatorDataTypes... >::IsEmpty() const noexcept
{
  using MultiObjectStorageType = typename MOA::MultiObjectElement< DataType, AllocatorDataTypes... >::MultiObjectStorageType;

  StaticAssert( std::is_void< MultiObjectStorageType >::value, "The MultiObjectAllocator does not contain that DataType!" );

  return MultiObjectStorageType::MultiDataContainerType.IsEmpty();
}

template< typename ... AllocatorDataTypes >
template< typename DataType >
bool MultiObjectAllocator< AllocatorDataTypes... >::IsFull() const noexcept
{
  using MultiObjectStorageType = typename MOA::MultiObjectElement< DataType, AllocatorDataTypes... >::MultiObjectStorageType;

  StaticAssert( std::is_void< MultiObjectStorageType >::value, "The MultiObjectAllocator does not contain that DataType!" );

  return MultiObjectStorageType::MultiDataContainerType.IsFull();
}
