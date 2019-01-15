/*!------------------------------------------------------------------------------
\file   MultiObjectAllocator.h

\author Garrett Conti

\brief  Project: MultiObjectAllocator
         Course: FUN
---------------------------------------------------------------------------- */

#pragma once

#include "pch.h"

#include <type_traits>
#include <unordered_map>
#include <forward_list>
#include <array>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename ... AllocatorDataTypes >
class MultiObjectAllocator;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace MOA
{
  template< bool TrueFalse, typename T1, typename T2 >
  using TemplateIf = typename std::conditional< TrueFalse, T1, T2 >::type;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  template< typename Type, typename Check, typename... OtherChecks >
  struct IsContainedIn
  {
    static constexpr bool IsContained = std::is_same< Type, Check >::value || IsContainedIn< Type, OtherChecks... >::IsContained;
  };

  template< typename Type, typename Check >
  struct IsContainedIn< Type, Check >
  {
    static constexpr bool IsContained = std::is_same< Type, Check >::value;
  };

  template< typename Type, typename... Checks >
  struct AreTypesUnique
  {
    static constexpr bool IsUnique = AreTypesUnique< Checks... >::IsUnique && !( IsContainedIn< Type, Checks... >::IsContained );
  };

  template< typename Type1, typename Type2 >
  struct AreTypesUnique< Type1, Type2 >
  {
    static constexpr bool IsUnique = !( IsContainedIn< Type1, Type2 >::IsContained );
  };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  template< typename DataType >
  class AllocatedPtr
  {
    public:
      using AllocatedPtrType = DataType;
    
    private:
      DataType *Ptr;
      const size_t DataCount = 1;

    private:
      template< typename ... AllocatorDataTypes >
      friend class MultiObjectAllocator;
  };

  // TODO: Specialize bool, since 8 bools can fit in a char
  template<>
  class AllocatedPtr< bool >
  {
    public:
      using AllocatedPtrType = bool;

    private:
      char *Ptr;
      const size_t DataCount = 1;

    private:
      template< typename ... AllocatorDataTypes >
      friend class MultiObjectAllocator;
  };

  template< typename DataType >
  class FreedPtrs
  {
    public:
      using FreedPtrsType = DataType;
      using FreedPtrsContainer = std::pmr::unordered_multimap< size_t, DataType* >;

    private:
      FreedPtrsContainer FreeData; // First is the FreedPtr, Second is the count

    private:
      template< typename ... AllocatorDataTypes >
      friend class MultiObjectAllocator;
  };

  // TODO: Specialize bool, since 8 bools can fit in a char
  template<>
  class FreedPtrs< bool >
  {
    public:
      using FreedPtrsType = bool;
      using FreedPtrsContainer = std::pmr::unordered_multimap< size_t, char* >;

    private:
      FreedPtrsContainer FreeData; // First is the FreedPtr, Second is the count

    private:
      template< typename ... AllocatorDataTypes >
      friend class MultiObjectAllocator;
  };

  template< typename ... DataTypes >
  class MultiObjectStorage;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  template< typename DataType >
  size_t CalcMemSize( const size_t *ObjCounts ) noexcept;

  template<  typename DataType1, typename DataType2 >
  size_t CalcMemSize( const size_t *ObjCounts ) noexcept;

  template< typename DataType1, typename DataType2, typename DataType3, typename ... OtherDataTypes >
  size_t CalcMemSize( const size_t *ObjCounts ) noexcept;

  template< typename DataType >
  constexpr size_t CalcMemSize() noexcept;

  template< typename DataType1, typename DataType2 >
  constexpr size_t CalcMemSize() noexcept;

  template< typename DataType1, typename DataType2, typename DataType3, typename ... OtherDataTypes >
  constexpr size_t CalcMemSize() noexcept;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  constexpr size_t DefaultObjectCount = 15;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename ... AllocatorDataTypes >
class MultiObjectAllocator
{
  public:
   MultiObjectAllocator() noexcept; // Will default all objects to 15
   explicit MultiObjectAllocator( const std::array< size_t, sizeof...( AllocatorDataTypes ) > &AverageObjectCount ) noexcept; // Will default any 0s to 15

  public:
    /**
     * \brief Allocates an object of DataType.
     *
     * \tparam DataType - The object to dynamically allocate.
     *
     * \return MOA::AllocatedPtr< DataType >& - A reference to the allocated object.
     */
    template< typename DataType >
    MOA::AllocatedPtr< DataType > & Allocate() noexcept;

    /**
     * \brief Allocates an object of DataType.
     *
     * \tparam bool - The object to dynamically allocate.
     *
     * \return MOA::AllocatedPtr< bool >& - A reference to the allocated object.
     */
    template<>
    MOA::AllocatedPtr< bool > & Allocate() noexcept;

    /**
     * \brief Allocates an object of DataType.
     *
     * \tparam DataType - The object to dynamically allocate.
     * 
     * \param Amount - The number of objects to allocate.
     *
     * \return MOA::AllocatedPtr< DataType >& - A reference to the array of allocated objects.
     */
    template< typename DataType >
    MOA::AllocatedPtr< DataType > & Allocate( size_t Amount ) noexcept;

  public:
    void Reserve( size_t NewSize ) noexcept;

  public:
    size_t Size() const noexcept; // Will return the size of the memory block
    size_t Capacity() const noexcept; // Will return the capacity of the memory block
    bool IsEmpty() const noexcept; // Will return if the memory block is empty
    bool IsFull() const noexcept; // Will return if the memory block is full

    template< typename DataType >
    size_t Size() const noexcept; // Will return the number of objects allocated for the template type

    template< typename DataType >
    size_t Capacity() const noexcept; // Will return the number of available object allocations for the template type

    template< typename DataType >
    bool IsEmpty() const noexcept; // Will return if has been no object allocated for the template type

    template< typename DataType >
    bool IsFull() const noexcept; // Will return if the max amount of objects have been allocated for the template type

  private:
    static constexpr size_t DefaultMemCapacity = MOA::CalcMemSize< AllocatorDataTypes... >(); 

  private:
          char *MemoryPtr;   // Our block of memory
          char *SizePtr;     // The start of the contiguous free section of memory
    const char *CapacityPtr; // The extent of the memory block

    MOA::FreedPtrs< char > FreePtrs; // All the non-contiguous free sections of general memory

    MOA::MultiObjectStorage< AllocatorDataTypes... > ObjectSpecificStorage; // All the non-contiguous free/allocated sections of specific memory
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace MOA
{
  template< typename OurDataType >
  class MultiObjectStorage< OurDataType >
  {
    public:
      using PrivateChildType = void;
      using AllocationBufferType = std::vector< MOA::AllocatedPtr< OurDataType > >;
      using DynamicAllocationBufferType = std::forward_list< AllocationBufferType >;

    public:
      MultiObjectStorage() noexcept { InitialAllocationBuffer.reserve( MOA::DefaultObjectCount ); }
      explicit MultiObjectStorage( const size_t *const ObjCounts ) noexcept
      {
        const size_t Count = *ObjCounts;

        InitialAllocationBuffer.reserve( Count ? Count : MOA::DefaultObjectCount );
      }

    private:
      void Reserve( char *const OldContainer, char *const NewContainer ) noexcept
      {
        for( MOA::AllocatedPtr< OurDataType > &Element : InitialAllocationBuffer )
        {
          Element.Ptr = NewContainer + ( Element.Ptr - OldContainer ); // Find the offset in the existing MemoryBlock, and get the new offset in the new Block
        }

        for( AllocationBufferType &OuterElement : AdditionalAllocationBuffers )
        {
          for( MOA::AllocatedPtr< OurDataType > &Element : OuterElement )
          {
            Element.Ptr = NewContainer + ( Element.Ptr - OldContainer ); 
          }
        }

        for( auto &Element : FreePtrs.FreeData )
        {
          Element.second = NewContainer + ( Element.second - OldContainer );
        }
      }

    private:
      size_t Size() const noexcept
      {
        size_t CurrentSize = InitialAllocationBuffer.size();

        for( AllocationBufferType &BufferElement :AdditionalAllocationBuffers )
        {
          CurrentSize += BufferElement.size();
        }

        return CurrentSize;
      }

      size_t Capacity() const noexcept
      {
        size_t CurrentCapacity = InitialAllocationBuffer.capacity();

        for( AllocationBufferType &BufferElement : AdditionalAllocationBuffers )
        {
          CurrentCapacity += BufferElement.capacity();
        }

        return CurrentCapacity;
      }

      bool IsEmpty() const noexcept { return InitialAllocationBuffer.empty(); }
      bool IsFull() const noexcept
      {
        bool Full;

        if( AdditionalAllocationBuffers.empty() == false )
        {
          AllocationBufferType &Buffer = AdditionalAllocationBuffers.front();

          Full = Buffer.size() == Buffer.capacity();
        }
        else Full = InitialAllocationBuffer.size() == InitialAllocationBuffer.capacity();

        return Full;
      }

    private: AllocationBufferType InitialAllocationBuffer;     // Move outside to speed up initial accesses (no ptr to ptr)
      DynamicAllocationBufferType AdditionalAllocationBuffers; // Need list to not invalidate data when adding more Buffers

      MOA::FreedPtrs< OurDataType > FreePtrs; // All the non-contiguous free sections of OurDataType memory

    private:
      template< typename ... AllocatorDataTypes >
      friend class MultiObjectAllocator;
  };

  template< typename OurDataType, typename ... ChildDataTypes >
  class MultiObjectStorage< OurDataType, ChildDataTypes... > : public MultiObjectStorage< OurDataType >, private MultiObjectStorage< ChildDataTypes... >
  {
    StaticAssert( !( MOA::AreTypesUnique< OurDataType, ChildDataTypes... >::IsUnique ), "The MultiObjectStorage cannot have duplicate types!" );
    StaticAssert( std::is_void< OurDataType >::value, "The MultiObjectStorage cannot have a void type!" );

    public:
      using PrivateChildType = MultiObjectStorage< ChildDataTypes... >;

    public:
      MultiObjectStorage() noexcept = default;
      explicit MultiObjectStorage( const size_t *const ObjCounts ) noexcept :
        MultiObjectStorage< OurDataType >{ ObjCounts }, MultiObjectStorage< ChildDataTypes... >{ ObjCounts + 1 } {}

    private:
      void Reserve( char *const OldContainer, char *const NewContainer ) noexcept
      {
        MultiObjectStorage< OurDataType >::Reserve( OldContainer, NewContainer );

        MultiObjectStorage< ChildDataTypes... >::Reserve( OldContainer, NewContainer );
      }

    private:
      template< typename ... AllocatorDataTypes >
      friend class MultiObjectAllocator;
  };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  template< typename ... DataTypes >
  struct MultiObjectElement;

  template< typename DataType, typename TheirDataType >
  struct MultiObjectElement< DataType, TheirDataType >
  {
    using MultiObjectStorageType = MOA::TemplateIf< std::is_same< DataType, TheirDataType >::value, MultiObjectStorage< TheirDataType >, void >;
  };

  template< typename DataType, typename TheirDataType, typename ... OtherDataTypes >
  struct MultiObjectElement< DataType, TheirDataType, OtherDataTypes... >
  {
    using MultiObjectStorageType = MOA::TemplateIf< std::is_same< DataType, TheirDataType >::value,
                                                    MOA::MultiObjectStorage< TheirDataType, OtherDataTypes... >,
                                                    typename MultiObjectElement< DataType, OtherDataTypes... >::MultiObjectStorageType >;
  };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MultiObjectAllocator.cpp"
