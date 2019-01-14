/*!----------------------------------------------------------------------------
\file   MemoryManager.cpp

\author Garrett Conti

\brief  Project: Golems_Magic
         Course: GAM300

\par    COPYRIGHT © 2018 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
---------------------------------------------------------------------------- */

#include "Public/Utils/MemoryManager.h"

#include <algorithm>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryPtr MemoryManager::NullPtr{};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool MemoryManager::MemoryPtrSetSorter::operator()( const MemoryPtr *const Lhs, const MemoryPtr *const Rhs ) const NoExcept
{ return *Lhs == *Rhs; /* Don't need to check actual ptrs since it's so fast and possibly inlined */ }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::MemoryPtr::TempMemoryPtrDtor::operator()( MemoryPtr *const Ptr ) const NoExcept { Ptr->~MemoryPtr(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryPtr::MemoryPtr() NoExcept : Ptr( nullptr ), PtrSize( 0 ), Manager( nullptr ) {}

MemoryManager::MemoryPtr::MemoryPtr( void *const Ptr ) NoExcept : Ptr( Ptr ), PtrSize( 0 ), Manager( nullptr ) {}

MemoryManager::MemoryPtr::MemoryPtr( void *const Ptr, const size_t PtrSize, MemoryManager *const Manager ) NoExcept :
  Ptr( Ptr ), PtrSize( PtrSize ), Manager( Manager ) { std::memset( Ptr, 0, sizeof( void* ) ); }

MemoryManager::MemoryPtr::MemoryPtr( void *const Ptr, const size_t PtrSize, MemoryManager *const Manager, bool ) NoExcept :
  Ptr( Ptr ), PtrSize( PtrSize ), Manager( Manager ), UserCreated( true ) { std::memset( Ptr, 0, sizeof( void* ) ); }

MemoryManager::MemoryPtr::MemoryPtr( void *const Ptr, const size_t PtrSize, const size_t NegitivePtrSize, MemoryManager *const Manager ) NoExcept :
  Ptr( Ptr ), PtrSize( PtrSize ), NegitivePtrSize( NegitivePtrSize ), Manager( Manager ), UserCreated( true )
{
  std::memset( Ptr, 0, sizeof( void* ) );
}

//MemoryManager::MemoryPtr::MemoryPtr( const MemoryPtr &Copy ) NoExcept : Ptr( Copy.Ptr ), PtrSize( Copy.PtrSize ),
//#if DebugMode 
//NegitivePtrSize( Copy.NegitivePtrSize ),
//#endif
//Manager( Copy.Manager ) {}

MemoryManager::MemoryPtr::MemoryPtr( MemoryPtr &&Move ) NoExcept : Ptr( Move.Ptr ), PtrSize( Move.PtrSize ),
#if DebugMode 
NegitivePtrSize( Move.NegitivePtrSize ),
#endif
Manager( Move.Manager ), UserCreated( Move.UserCreated )
{
  *( const_cast< void* * >( &( Move.Ptr ) ) ) = nullptr;

  if( UserCreated ) Manager->PtrSpawnedData.emplace( this );
} // Remove const to 'move' data

MemoryManager::MemoryPtr::~MemoryPtr() NoExcept { if( UserCreated ) Manager->PtrSpawnedData.erase( this ); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//MemoryManager::MemoryPtr & MemoryManager::MemoryPtr::operator=( const MemoryPtr &Copy ) NoExcept
//{
//  *( const_cast< void* * >( &( Ptr ) ) ) = Copy.Ptr;
//
//  *( const_cast< Size_T* >( &( PtrSize ) ) ) = Copy.PtrSize;
//
//  DebugOnly( *( const_cast< Size_T* >( &( NegitivePtrSize = Copy.NegitivePtrSize ) ) ) );
//
//  *( const_cast< MemoryManager* * >( &( Manager ) ) ) = Copy.Manager;
//
//  return *this;
//}

//MemoryManager::MemoryPtr & MemoryManager::MemoryPtr::operator=( MemoryPtr &&Move ) NoExcept
//{
//  // Remove const to 'move' data
//
//  *( const_cast< void* * >( &( Ptr      ) ) ) = Move.Ptr;
//  *( const_cast< void* * >( &( Move.Ptr ) ) ) = nullptr;
//
//  *( const_cast< Size_T* >( &( PtrSize ) ) ) = Move.PtrSize;
//
//  DebugOnly( *( const_cast< Size_T* >( &( NegitivePtrSize = Move.NegitivePtrSize ) ) ) );
//
//  *( const_cast< MemoryManager* * >( &( Manager ) ) ) = Move.Manager;
//
//  return *this;
//}

MemoryManager::MemoryPtr::TempMemoryPtr MemoryManager::MemoryPtr::operator+( const Size_T CharCount ) const NoExcept
{
  DebugAssert( !CharCount, "Adding 0 to a MemoryPtr is not allowed!", return nullptr; );

#if DebugMode
  char *const NewPtr = reinterpret_cast< char* >( Ptr ) + CharCount;

  DebugAssert( NewPtr > reinterpret_cast< char* >( Ptr ) + PtrSize, "A MemoryPtr overflowed! (went out-of-bounds positivly).", return nullptr; );

  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ NewPtr, PtrSize - CharCount, NegitivePtrSize + CharCount, Manager } ) ).first ) ) ), TempMemoryPtrDtor{} };

  return Return; // NRVO
#else
  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ reinterpret_cast< char* >( Ptr ) + Amount, PtrSize - Amount, Manager, true } ) ).first ) ) ), TempMemoryPtrDtor{} };

  return Return; // NRVO
#endif
}

MemoryManager::MemoryPtr::TempMemoryPtr MemoryManager::MemoryPtr::operator+( const int CharCount ) const NoExcept
{
  DebugAssert( !CharCount, "Adding 0 to a MemoryPtr is not allowed!", return nullptr; );

#if DebugMode
  if( CharCount > 0 )
  {
    DebugAssert( PtrSize < static_cast< Size_T >( CharCount ), "A MemoryPtr overflowed! (went out-of-bounds positivly).", return nullptr; );
  }
  else DebugAssert( NegitivePtrSize < static_cast< Size_T >( -CharCount ), "A MemoryPtr underflowed! (went out-of-bounds negitivly).", return nullptr; );

  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ reinterpret_cast< char* >( Ptr ) + CharCount, PtrSize - CharCount, NegitivePtrSize + CharCount, Manager } ) ).first ) ) ),
                        TempMemoryPtrDtor{} };
  return Return; // NRVO
#else
  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ reinterpret_cast< char* >( Ptr ) + Amount, PtrSize - Amount, Manager, true } ) ).first ) ) ), TempMemoryPtrDtor{} };

  return Return; // NRVO
#endif
}

MemoryManager::MemoryPtr::TempMemoryPtr MemoryManager::MemoryPtr::operator-( const Size_T CharCount ) const NoExcept
{
  DebugAssert( !CharCount, "Subtracting 0 from a MemoryPtr is not allowed!", return nullptr; );

#if DebugMode
  char *const NewPtr = reinterpret_cast< char* >( Ptr ) - CharCount;

  DebugAssert( NewPtr < reinterpret_cast< char* >( Ptr ) - NegitivePtrSize, "A MemoryPtr underflowed! (went out-of-bounds negitivly).", return nullptr; );

  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ NewPtr, PtrSize + CharCount, NegitivePtrSize - CharCount, Manager } ) ).first ) ) ), TempMemoryPtrDtor{} };

  return Return; // NRVO
#else
  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ reinterpret_cast< char* >( Ptr ) - Amount, PtrSize + Amount, Manager, true } ) ).first ) ) ), TempMemoryPtrDtor{} };

  return Return; // NRVO
#endif
}

MemoryManager::MemoryPtr::TempMemoryPtr MemoryManager::MemoryPtr::operator-( const int CharCount ) const NoExcept
{
  DebugAssert( !CharCount, "Subtracting 0 from a MemoryPtr is not allowed!", return nullptr; );

#if DebugMode
  if( CharCount < 0 )
  {
    DebugAssert( NegitivePtrSize < static_cast< Size_T >( -CharCount ), "A MemoryPtr underflowed! (went out-of-bounds negitivly).", return nullptr; );
  }
  else DebugAssert( PtrSize < static_cast< Size_T >( CharCount ), "A MemoryPtr overflowed! (went out-of-bounds positivly).", return nullptr; );

  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ reinterpret_cast< char* >( Ptr ) - CharCount, PtrSize + CharCount, NegitivePtrSize - CharCount, Manager } ) ).first ) ) ),
                        TempMemoryPtrDtor{} };

  return Return; // NRVO
#else
  TempMemoryPtr Return{ &( const_cast< MemoryPtr& >( **( Manager->PtrSpawnedData.emplace(
    Manager->PtrArray.emplace( MemoryPtr{ reinterpret_cast< char* >( Ptr ) - Amount, PtrSize + Amount, Manager, true } ) ).first ) ) ), TempMemoryPtrDtor{} };

  return Return; // NRVO
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::MemoryPtr::operator=( MemoryPtr &&Move ) NoExcept
{
  const_cast< void* & >( Ptr ) = Move.Ptr;
  const_cast< void* & >( Move.Ptr ) = nullptr;

  const_cast< size_t& >( PtrSize ) = Move.PtrSize;

  DebugOnly( const_cast< size_t& >( NegitivePtrSize ) = Move.NegitivePtrSize );

  const_cast< MemoryManager* & >( Manager ) = Move.Manager;

  const_cast< bool& >( UserCreated ) = Move.UserCreated;
}

bool MemoryManager::MemoryPtr::operator<( const MemoryPtr &Rhs ) const NoExcept { return Ptr < Rhs.Ptr; } // RVO
bool MemoryManager::MemoryPtr::operator>( const MemoryPtr &Rhs ) const NoExcept { return Ptr > Rhs.Ptr; } // RVO

bool MemoryManager::MemoryPtr::operator!=( const MemoryPtr &Rhs ) const NoExcept { return Ptr != Rhs.Ptr; } // RVO
bool MemoryManager::MemoryPtr::operator==( const MemoryPtr &Rhs ) const NoExcept { return Ptr == Rhs.Ptr; } // RVO
bool MemoryManager::MemoryPtr::operator<=( const MemoryPtr &Rhs ) const NoExcept { return Ptr <= Rhs.Ptr; } // RVO
bool MemoryManager::MemoryPtr::operator>=( const MemoryPtr &Rhs ) const NoExcept { return Ptr >= Rhs.Ptr; } // RVO

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryPtr::operator bool() const NoExcept { return Ptr; }

MemoryManager::MemoryPtr::operator unsigned char() const NoExcept { return static_cast< unsigned char >( reinterpret_cast< unsigned long long >( Ptr ) ); }
MemoryManager::MemoryPtr::operator unsigned short() const NoExcept { return static_cast< unsigned short >( reinterpret_cast< unsigned long long >( Ptr ) ); }
MemoryManager::MemoryPtr::operator unsigned int() const NoExcept { return static_cast< unsigned int >( reinterpret_cast< unsigned long long >( Ptr ) ); }
MemoryManager::MemoryPtr::operator unsigned long() const NoExcept { return static_cast< unsigned long >( reinterpret_cast< unsigned long long >( Ptr ) ); }

MemoryManager::MemoryPtr::operator unsigned long long() const NoExcept { return reinterpret_cast< unsigned long long >( Ptr ); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::MemoryPtr::SetData_Memcpy_Array( const void *const Array, const size_t ArraySize, const Size_T Index, const Size_T Count ) NoExcept
{
  //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

  DebugAssert( Index * ArraySize + Count * ArraySize > PtrSize,
               "The ArraySize + Index + Count goes out of bounds!", return );

  DebugAssert( Count < 2, "Count cannot be less then 2 for MemCopy!", );

  std::memcpy( reinterpret_cast< UChar* >( Ptr ) + Index * ArraySize, Array, ArraySize * Count );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::MemoryPtr::SetData_Memcpy( const void *const Value, const size_t ValueSize ) NoExcept
{
  DebugAssert( ValueSize > PtrSize,
               "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

  std::memcpy( Ptr, Value, ValueSize );
}

void MemoryManager::MemoryPtr::SetData_Memcpy( const void *const Value, const size_t ValueSize, const Size_T Index ) NoExcept
{
  DebugAssert( ValueSize + Index * ValueSize > PtrSize,
               "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

  DebugAssert( !Index, "Use the no-index SetData instead!", );

  std::memcpy( reinterpret_cast< UChar* >( Ptr ) + Index * ValueSize, Value, ValueSize );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::MemoryPtr::SetData_Memset( const bool Value, const Size_T Index, const Size_T Count ) NoExcept
{ SetData_Memset( static_cast< const UChar >( Value ), Index, Count ); }

void MemoryManager::MemoryPtr::SetData_Memset( const char Value, const Size_T Index, const Size_T Count ) NoExcept
{ SetData_Memset( static_cast< const UChar >( Value ), Index, Count ); }

void MemoryManager::MemoryPtr::SetData_Memset( const UChar Value, const Size_T Index, const Size_T Count ) NoExcept
{
  DebugAssert( Index >= PtrSize,
               "The Index you are trying to Set in a MemoryPtr is too far for the size of memory it contains!", return );

  DebugAssert( Index + Count > PtrSize,
               "The Count you are trying to Set in a MemoryPtr is too many for the size of memory it contains!", return );

  DebugAssert( Count < 2, "This Memset requires a count larger than 1!", );

  std::memset( reinterpret_cast< UChar* >( Ptr ) + Index, Value, Count );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryManager() NoExcept : RegularDataSize( DefaultSize ), RegularCapacity( STLDefaultCapacity ),
Data( malloc( DefaultSize * STLDefaultCapacity ) ), DataEnd( reinterpret_cast< const char* >( Data ) + DefaultSize * STLDefaultCapacity ),
PtrArray{ STLDefaultCapacity * STLDefaultGrow }
{
  UsedData.reserve( STLDefaultCapacity );

  FreedDataByPtr.reserve( STLDefaultCapacity );
}

MemoryManager::MemoryManager( const MemoryManager &Copy ) NoExcept : MemoryManager{ Copy.RegularCapacity, Copy.RegularDataSize } {}

MemoryManager::MemoryManager( MemoryManager &&Move ) NoExcept : RegularDataSize( Move.RegularDataSize ), RegularCapacity( Move.RegularCapacity ),
Data( Move.Data ), DataEnd( Move.DataEnd ), PtrArray{ RegularCapacity * STLDefaultGrow }
{
  DebugAssert( Move.FreeIndex != Move.Data, "Some data was not free'd from the moved memory manager!", );
  DebugAssert( Move.PtrSpawnedData.size(),
               "Some ptrs that were created from other ptrs for the moved memory manager have not gone out of scope!", );

  Move.Data = nullptr;
  DebugOnly( Move.FreeIndex = nullptr; ) // Only needed for debug checking

    UsedData.reserve( RegularCapacity );

  FreedDataByPtr.reserve( RegularCapacity );

  PtrSpawnedData.reserve( RegularCapacity );
}

MemoryManager::MemoryManager( const void *const DefualtValue ) NoExcept : MemoryManager{ STLDefaultCapacity, DefaultSize, DefualtValue } {}
MemoryManager::MemoryManager( const void *const DefualtValue, const size_t ValueSize ) NoExcept :
  MemoryManager{ STLDefaultCapacity, DefaultSize, DefualtValue, ValueSize } {}

MemoryManager::MemoryManager( const Size_T Capacity ) NoExcept : MemoryManager{ Capacity, DefaultSize } {}

MemoryManager::MemoryManager( const Size_T Capacity, const size_t RegularDataSize ) NoExcept :
  RegularDataSize( RegularDataSize ), RegularCapacity( Capacity ), Data( malloc( RegularDataSize * RegularCapacity ) ),
  DataEnd( reinterpret_cast< const char* >( Data ) + RegularDataSize * RegularCapacity ), PtrArray{ RegularCapacity * STLDefaultGrow }
{
  UsedData.reserve( Capacity );

  FreedDataByPtr.reserve( Capacity );

  PtrSpawnedData.reserve( Capacity );
}

MemoryManager::MemoryManager( const Size_T Capacity, const size_t RegularDataSize, const void *const DefualtValue ) NoExcept :
MemoryManager{ Capacity, RegularDataSize }
{
  for( auto DataIter = reinterpret_cast< char* >( Data ); DataIter < DataEnd; DataIter += RegularDataSize )
  {
    std::memcpy( DataIter, DefualtValue, RegularDataSize );
  }
}

MemoryManager::MemoryManager( const Size_T Capacity, const size_t RegularDataSize, const void *const DefualtValue, const size_t ValueSize ) NoExcept :
MemoryManager{ Capacity, RegularDataSize }
{
  DebugAssert( ValueSize == RegularDataSize, "ValueSize is equal to the DataSize, use the no ValueSize Ctor instead!", );
  DebugAssert( ValueSize > RegularDataSize, "ValueSize is greater than the DataSize!", );
  DebugAssert( ValueSize > RegularDataSize * Capacity, "ValueSize is greater than the entire memory array! Skipping initialization!", return );

  for( auto DataIter = reinterpret_cast< char* >( Data ); DataIter < DataEnd; DataIter += ValueSize )
  {
    std::memcpy( DataIter, DefualtValue, ValueSize );
  }
}

MemoryManager::~MemoryManager() NoExcept
{
  DebugAssert( FreeIndex != Data, "Some data was not free'd from a memory manager!", );

  DebugAssert( PtrSpawnedData.size(), "Some ptrs that were created from other ptrs for a memory manager have not gone out of scope!", );

  free( Data );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::operator=( const MemoryManager &Copy ) NoExcept
{
  DebugAssert( FreeIndex != Data, "Some data was not free'd from a memory manager before copying!", );
  DebugAssert( PtrSpawnedData.size(),
               "Some ptrs that were created from other ptrs for a memory manager have not gone out of scope before copying!", );


  const_cast< size_t& >( RegularDataSize ) = Copy.RegularDataSize;
  RegularCapacity = Copy.RegularCapacity;

  free( Data );

  PtrArray.clear();

  UsedData.clear();

  FreedDataByPtr.clear();
  FreedDataBySize.clear();

  PtrSpawnedData.clear();


  FreeIndex = Data = malloc( RegularDataSize * RegularCapacity );

  DataEnd = reinterpret_cast< const char* >( Data ) + RegularDataSize * RegularCapacity;

  PtrArray.reserve( RegularCapacity * STLDefaultGrow );

  UsedData.reserve( RegularCapacity );

  FreedDataByPtr.reserve( RegularCapacity );

  PtrSpawnedData.reserve( RegularCapacity );
}

void MemoryManager::operator=( MemoryManager &&Move ) NoExcept
{
  DebugAssert( FreeIndex != Data, "Some data was not free'd from a memory manager before moving!", );
  DebugAssert( PtrSpawnedData.size(),
               "Some ptrs that were created from other ptrs for a memory manager have not gone out of scope before moving!", );

  DebugAssert( Move.FreeIndex != Move.Data, "Some data was not free'd from the moved memory manager!", );
  DebugAssert( Move.PtrSpawnedData.size(),
               "Some ptrs that were created from other ptrs for the moved memory manager have not gone out of scope!", );


  free( Data );

  PtrArray.clear();

  UsedData.clear();

  FreedDataByPtr.clear();
  FreedDataBySize.clear();

  PtrSpawnedData.clear();


  const_cast< size_t& >( RegularDataSize ) = Move.RegularDataSize;
  RegularCapacity = Move.RegularCapacity;

  FreeIndex = Data = Move.Data;
  Move.Data = nullptr;
  DebugOnly( Move.FreeIndex = nullptr; ); // Only needed for debug checking

  DataEnd = Move.DataEnd;

  PtrArray.reserve( RegularCapacity * STLDefaultGrow );

  UsedData.reserve( RegularCapacity );

  FreedDataByPtr.reserve( RegularCapacity );

  PtrSpawnedData.reserve( RegularCapacity );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryPtrSet::iterator MemoryManager::begin() NoExcept { return UsedData.begin(); }
MemoryManager::MemoryPtrSet::const_iterator MemoryManager::cbegin() const NoExcept { return UsedData.cbegin(); }

MemoryManager::MemoryPtrSet::iterator MemoryManager::end() NoExcept { return UsedData.end(); }
MemoryManager::MemoryPtrSet::const_iterator MemoryManager::cend() const NoExcept { return UsedData.cend(); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryPtr & MemoryManager::Allocate() NoExcept { return Allocate_Internal( RegularDataSize ); } // RVO

MemoryManager::MemoryPtr & MemoryManager::Allocate( const size_t DataSize ) NoExcept
{
  DebugAssert( DataSize == RegularDataSize, "Use the no-arg Allocate for the MemoryManager instead!", );

  return Allocate_Internal( DataSize ); // RVO 
}

MemoryManager::MemoryPtr & MemoryManager::Allocate( const void *const DataValue ) NoExcept
{
  MemoryManager::MemoryPtr &Ptr = Allocate_Internal( RegularDataSize );

  Ptr.SetData_Memcpy( DataValue, RegularDataSize );

  return Ptr; // NRVO 
}

MemoryManager::MemoryPtr & MemoryManager::Allocate( const void *const DataValue, const size_t ValueSize ) NoExcept
{
  DebugAssert( ValueSize == RegularDataSize, "ValueSize is equal to the DataSize, use the no-arg ValueSize Allocator instead!", );
  DebugAssert( ValueSize > RegularDataSize, "ValueSize is greater than the DataSize!", return NullPtr );

  MemoryManager::MemoryPtr &Ptr = Allocate_Internal( RegularDataSize );

  Ptr.SetData_Memcpy( DataValue, ValueSize );

  for( Size_T i = 1, End = static_cast< Size_T >( RegularDataSize / ValueSize ); i < End; ++i )
  {
    Ptr.SetData_Memcpy( DataValue, ValueSize, i );
  }

  return Ptr; // NRVO 
}

MemoryManager::MemoryPtr & MemoryManager::Allocate( const size_t DataSize, const void *const DataValue ) NoExcept
{
  DebugAssert( DataSize == RegularDataSize, "Use the no-DataSize Allocate for the MemoryManager instead!", );

  MemoryManager::MemoryPtr &Ptr = Allocate_Internal( DataSize );

  Ptr.SetData_Memcpy( DataValue, DataSize );

  return Ptr; // NRVO 
}

MemoryManager::MemoryPtr & MemoryManager::Allocate( const size_t DataSize, const void *const DataValue, const size_t ValueSize ) NoExcept
{
  DebugAssert( DataSize == RegularDataSize, "Use the no-DataSize Allocate for the MemoryManager instead!", );
  DebugAssert( ValueSize == DataSize, "ValueSize is equal to the DataSize, use the no-ValueSize Allocator instead!", );
  DebugAssert( ValueSize > DataSize, "ValueSize is greater than the DataSize!", return NullPtr );

  MemoryManager::MemoryPtr &Ptr = Allocate_Internal( DataSize );

  Ptr.SetData_Memcpy( DataValue, ValueSize );

  for( Size_T i = 1, End = static_cast< Size_T >( DataSize / ValueSize ); i < End; ++i )
  {
    Ptr.SetData_Memcpy( DataValue, ValueSize, i );
  }

  return Ptr; // NRVO 
}

MemoryManager::MemoryPtr & MemoryManager::Allocate( const size_t DataSize, const Size_T DataCount, const void *const ValueArray ) NoExcept
{
  MemoryManager::MemoryPtr &Ptr = Allocate_Internal( DataSize * DataCount );

  Ptr.SetData_Memcpy_Array( ValueArray, DataSize, 0, DataCount );

  return Ptr; // NRVO 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MemoryManager::Free( MemoryPtr &DataPtr ) NoExcept { Free( &DataPtr ); }

void MemoryManager::Free( MemoryPtr *const DataPtr ) NoExcept
{
  char *MemPtr;
  size_t PtrSize;

  {
    const auto Iter = UsedData.find( DataPtr );

    DebugAssert( Iter == UsedData.cend(), "The MemoryPtr passed to the MemoryManager was not returned from an Allocate function!", return );

    MemPtr = reinterpret_cast< char * >( ( *Iter )->Ptr );
    PtrSize = ( *Iter )->PtrSize;
  }

  const auto FreeDataBySize = [ this ]( const decltype( FreedDataByPtr )::iterator &FreedIter )
  {
    auto IterPair = FreedDataBySize.equal_range( FreedIter->second );

    const size_t RangeSize = std::distance( IterPair.first, IterPair.second );

    if( RangeSize > 8 ) // 8 = tested value
    {
      std::vector< decltype( FreedDataBySize )::iterator* > SortedPtrs;

      SortedPtrs.reserve( RangeSize );

      do
      {
        SortedPtrs.emplace_back( &( IterPair.first ) );
      }
      while( ++( IterPair.first ) != IterPair.second );

      {
        static const auto PairSorter = []( const decltype( FreedDataBySize )::iterator *const Lhs,
                                           const decltype( FreedDataBySize )::iterator *const Rhs )NoExcept->bool
        { return ( *Lhs )->second < ( *Rhs )->second; };

        std::sort( SortedPtrs.begin(), SortedPtrs.end(), PairSorter );
      }

      FreedDataBySize.erase( *( BinarySearch( SortedPtrs, FreedIter->first ) ) );
    }
    else do // Manual search
    {
      if( IterPair.first->second == FreedIter->first )
      {
        FreedDataBySize.erase( IterPair.first );

        break;
      }
    }
    while( ++( IterPair.first ) != IterPair.second );
  };

  if( MemPtr == reinterpret_cast< char * >( FreeIndex ) - PtrSize )
  {
    FreeIndex = MemPtr;
  }
  else
  {
    const auto FreedIter = FreedDataByPtr.find( MemPtr + PtrSize );

    if( FreedIter != FreedDataByPtr.cend() ) // Single right combine
    {
      FreeDataBySize( FreedIter );

      const size_t Size = PtrSize + FreedIter->second;

      FreedDataByPtr.erase( FreedIter );

      FreedDataBySize.emplace( Size, MemPtr );

      FreedDataByPtr.emplace( MemPtr, Size );
    }
    else // No combine
    {
      FreedDataBySize.emplace( PtrSize, MemPtr );

      FreedDataByPtr.emplace( MemPtr, PtrSize );
    }
  }

  UsedData.erase( DataPtr );

  PtrArray.erase( DataPtr );

  for( auto Iter = FreedDataByPtr.begin(), End = FreedDataByPtr.end(); Iter != End; )
  {
    if( Iter->first != MemPtr )
    {
      bool Incremented = false;

      for( ; ; )
      {
        void *const NextPtr = reinterpret_cast< char* >( Iter->first ) + Iter->second;

        const auto FreedIter = FreedDataByPtr.find( NextPtr );

        if( FreedIter != FreedDataByPtr.cend() ) // Single right combine
        {
          FreeDataBySize( FreedIter );

          FreeDataBySize( Iter );

          const size_t Size = Iter->second + FreedIter->second;

          FreedDataByPtr.erase( FreedIter );

          FreedDataBySize.emplace( Size, Iter->first );

          Iter->second = Size;
        }
        else // no combine
        {
          if( NextPtr == FreeIndex )
          {
            FreeIndex = Iter->first;

            FreeDataBySize( Iter );

            FreedDataByPtr.erase( Iter++ );

            Incremented = true;
          }

          break;
        }
      }

      if( !Incremented ) ++Iter;
    }
    else ++Iter;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::MemoryPtr & MemoryManager::Allocate_Internal( const size_t DataSize ) NoExcept
{
  void *ReturnIndex;

  if( FreedDataBySize.size() )
  {
    const auto Iter = FreedDataBySize.lower_bound( DataSize );

    if( Iter != FreedDataBySize.cend() )
    {
      ReturnIndex = Iter->second;

      const size_t RemaingSize = Iter->first - DataSize;

      FreedDataByPtr.erase( ReturnIndex );

      FreedDataBySize.erase( Iter );

      if( RemaingSize )
      {
        char *const NewPtr = reinterpret_cast< char* >( ReturnIndex ) + DataSize;

        FreedDataBySize.emplace( RemaingSize, NewPtr );

        FreedDataByPtr.emplace( NewPtr, RemaingSize );
      }
    }
    else
    {
      if( FreeIndex == DataEnd )
      {
        Grow();

        ReturnIndex = FreeIndex;

        FreeIndex = reinterpret_cast< char* >( FreeIndex ) + DataSize; // Re-calc pos since FreeIndex changed after Grow
      }
      else
      {
        char *NextPtr = reinterpret_cast< char* >( FreeIndex ) + DataSize;

        if( NextPtr > DataEnd )
        {
          Grow();

          ReturnIndex = FreeIndex;

          FreeIndex = reinterpret_cast< char* >( FreeIndex ) + DataSize; // Re-calc pos since FreeIndex changed after Grow
        }
        else
        {
          ReturnIndex = FreeIndex;

          FreeIndex = NextPtr;
        }
      }
    }
  }
  else
  {
    if( FreeIndex == DataEnd )
    {
      Grow();

      ReturnIndex = FreeIndex;

      FreeIndex = reinterpret_cast< char* >( FreeIndex ) + DataSize;
    }
    else
    {
      char *const NextPtr = reinterpret_cast< char* >( FreeIndex ) + DataSize;

      if( NextPtr > DataEnd )
      {
        Grow();

        ReturnIndex = FreeIndex;

        FreeIndex = reinterpret_cast< char* >( FreeIndex ) + DataSize; // Re-calc pos since FreeIndex changed after Grow
      }
      else
      {
        ReturnIndex = FreeIndex;

        FreeIndex = NextPtr;
      }
    }
  }

  MemoryPtr *const Return = PtrArray.emplace( MemoryPtr{ ReturnIndex, DataSize, this } );

  UsedData.emplace( Return );

  return *Return; // NRVO
}

//bool MemoryManager::Compress() NoExcept
//{
//  std::vector< void* > RemovePtrs;
//
//  RemovePtrs.reserve( FreedDataByPtr.size() - 1 ); // Minus one, since we can't compress to nothing
//
//  const auto End = FreedDataByPtr.cend();
//
//  for( auto CombineIter = FreedDataByPtr.begin(); CombineIter != End; )
//  {
//    for( auto CheckIter = CombineIter; ;  )
//    {
//      ++CheckIter;
//
//      // Since ptrs are sorted, the next ptr should always be: CurPtr + CurSize
//      if( CheckIter != End && reinterpret_cast< char* >( CombineIter->first ) + CombineIter->second == CheckIter->first ) // Combine with right
//      {
//        CombineIter->second += CheckIter->second;
//
//        RemovePtrs.emplace_back( CheckIter->first );
//      }
//      else CombineIter = CheckIter; break;
//    }
//  }
//
//  bool Compressed;
//
//  if( RemovePtrs.size() ) // Clear out removed ptrs and add back the resized ones to the size map
//  {
//    Compressed = true;
//
//    for( const auto &Iter : RemovePtrs )
//    {
//      FreedDataByPtr.erase( Iter );
//    }
//
//    FreedDataBySize.clear();
//
//    for( const auto &Iter : FreedDataByPtr )
//    {
//      FreedDataBySize.emplace( Iter.second, Iter.first );
//    }
//  }
//  else Compressed = false;
//
//  return Compressed; // NRVO
//}

void MemoryManager::Grow() NoExcept
{
  const size_t NewSize = RegularDataSize * ( RegularCapacity *= STLDefaultGrow );

  char *const NewData = static_cast< char* >( malloc( NewSize ) );

  DataEnd = NewData + NewSize;

  const char *const OldData = reinterpret_cast< char* >( Data );

  FreeIndex = NewData + NewSize / STLDefaultGrow;

  // Set new used ptrs
  for( auto &Iter : PtrArray )
  {
    for( auto &DataIter : Iter )
    {
      if( DataIter.Ptr )
      {
        void *const NewPtr = NewData + ( reinterpret_cast< char* >( DataIter.Ptr ) - OldData );

        std::memcpy( NewPtr, DataIter.Ptr, DataIter.PtrSize );

        const_cast< void* & >( DataIter.Ptr ) = NewPtr;
      }
    }
  }

  std::vector< std::pair< void*, size_t > > TempDataByPtr{};

  TempDataByPtr.reserve( FreedDataByPtr.size() );

  // Get new free'd ptrs
  for( const auto &Iter : FreedDataByPtr )
  {
    const size_t Size = reinterpret_cast< char* >( Iter.first ) - OldData;

    void *const NewPtr = NewData + Size;

    std::memcpy( NewPtr, Iter.first, Iter.second );

    TempDataByPtr.emplace_back( std::make_pair( NewPtr, Iter.second ) );
  }

  FreedDataByPtr.clear();
  FreedDataBySize.clear();

  // Set new free'd ptrs
  for( const auto &Iter : TempDataByPtr )
  {
    FreedDataByPtr.emplace( Iter.first, Iter.second );
    FreedDataBySize.emplace( Iter.second, Iter.first );
  }

  free( Data );

  Data = NewData;
  
  UsedData.reserve( UsedData.size() );
  PtrSpawnedData.reserve( PtrSpawnedData.size() );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const std::multimap< size_t, void * >::iterator *
MemoryManager::BinarySearch( const std::vector< decltype( FreedDataBySize )::iterator* > &Vector, const void *const Value ) NoExcept
{
  for( size_t Begin = 0, End = Vector.size(); ; )
  {
    const auto Half = static_cast< size_t >( std::ceil( ( End - Begin ) / 2.0 ) );

    const auto *const Iter = Vector[ Half ];

    const void *const CheckValue = ( *Iter )->second;

    if( CheckValue != Value )
    {
      if( CheckValue < Value )
      {
        End = Half;
      }
      else Begin = Half + 1;
    }
    else return Iter;
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::DynamicSortContainer::DynamicSortContainer( const size_t Capacity ) NoExcept { Grow( Capacity ); }

MemoryManager::MemoryPtr * MemoryManager::DynamicSortContainer::emplace( MemoryPtr &&Node ) NoExcept
{
  auto *FrontVector = &( Data.front() );

  {
    const size_t Capacity = FrontVector->capacity();

    if( FrontVector->size() == Capacity )
    {
      Grow( Capacity * STLDefaultGrow );

      FrontVector = &( Data.front() );
    }
  }

  MemoryPtr *const MemPtr = FrontVector->emplace( std::move( Node ) );

  return MemPtr; // NRVO
}

void MemoryManager::DynamicSortContainer::erase( MemoryPtr *const MemPtr ) NoExcept
{
  const_cast< void* & >( MemPtr->Ptr ) = nullptr;

  auto DataIter = Data.begin();

  const auto End = DataIter->end();

  if( MemPtr < End )
  {
    DataIter->erase( static_cast< Size_T >( MemPtr - DataIter->begin() ) );
  }
  else for( auto PrevIter = DataIter; ; PrevIter = DataIter ) // BUG: Potential crash, if never found...
  {
    ++DataIter;

    if( MemPtr < End )
    {
      DataIter->erase( static_cast< Size_T >( MemPtr - DataIter->begin() ) ); // Need to get new begin each time DataIter is incremented

      if( !( DataIter->size() ) )
      {
        Data.erase_after( PrevIter );
      }

      break;
    }
  }
}

void MemoryManager::DynamicSortContainer::clear() NoExcept { Data.clear(); }

void MemoryManager::DynamicSortContainer::reserve( const size_t Capacity ) NoExcept
{
  size_t TotalSize = 0;

  for( const auto &Iter : Data )
  {
    TotalSize += Iter.size();
  }

  if( TotalSize )
  {
    if( TotalSize < Capacity )
    {
      const size_t NextCapacity = Data.front().capacity() * STLDefaultGrow;

      if( NextCapacity < Capacity )
      {
        Grow( Capacity );
      }
      else Grow( NextCapacity );
    }
  }
  else Grow( Capacity );
}

MemoryManager::DynamicSortContainer::DataType::iterator MemoryManager::DynamicSortContainer::begin() NoExcept { return Data.begin(); }

MemoryManager::DynamicSortContainer::DataType::const_iterator MemoryManager::DynamicSortContainer::end() NoExcept { return Data.cend(); }

void MemoryManager::DynamicSortContainer::Grow( const size_t Capacity ) NoExcept
{
  Data.emplace_front( decltype( Data )::value_type{ Capacity } );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryManager::DynamicSortContainer::MemoryVector::MemoryVector( const size_t Capacity_ ) NoExcept :
PtrArray( reinterpret_cast< MemoryPtr* >( malloc( Capacity_ * sizeof( MemoryPtr ) ) ) ), Capacity( Capacity_ )
{
  const_cast< void* & >( PtrArray->Ptr ) = nullptr;

  for( size_t i = 1; i < Capacity_; ++i )
  {
    const_cast< void* & >( ( PtrArray + i )->Ptr ) = nullptr;
  }
}

MemoryManager::DynamicSortContainer::MemoryVector::MemoryVector( MemoryVector &&Move ) NoExcept :
PtrArray( Move.PtrArray ), Capacity( Move.Capacity ) { Move.PtrArray = nullptr; }

MemoryManager::DynamicSortContainer::MemoryVector::~MemoryVector() NoExcept { free( PtrArray ); }

MemoryManager::MemoryPtr * MemoryManager::DynamicSortContainer::MemoryVector::emplace( MemoryPtr &&Move ) NoExcept
{
  MemoryManager::MemoryPtr *const MemPtr = PtrArray + Size;

  *MemPtr = std::move( Move );

  ++Size;

  return MemPtr; // NRVO
}

void MemoryManager::DynamicSortContainer::MemoryVector::erase( const Size_T Index ) NoExcept
{
  if( Index == Size - 1 )
  {
    --Size;

    if( Size )
    {
      MemoryPtr *MemPtr = PtrArray + Index - 1;

      if( !( MemPtr->Ptr ) )
      {
        do
        {
          --Size;

          --MemPtr;
        }
        while( Size && !( MemPtr->Ptr ) );
      }
    }
  }
}

MemoryManager::MemoryPtr * MemoryManager::DynamicSortContainer::MemoryVector::begin() NoExcept { return PtrArray; }
const MemoryManager::MemoryPtr * MemoryManager::DynamicSortContainer::MemoryVector::end() const NoExcept { return PtrArray + Capacity; } // RVO

size_t MemoryManager::DynamicSortContainer::MemoryVector::size()     const NoExcept { return Size; }
size_t MemoryManager::DynamicSortContainer::MemoryVector::capacity() const NoExcept { return Capacity; }

const MemoryManager::MemoryPtr * MemoryManager::DynamicSortContainer::MemoryVector::GetIndex( const Size_T Index ) const NoExcept
{ return PtrArray + Index; } // RVO
