/*!----------------------------------------------------------------------------
\file   MemoryManager.h

\author Garrett Conti

\brief  Project: Golems_Magic
         Course: GAM300

\par    COPYRIGHT © 2018 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
---------------------------------------------------------------------------- */

#pragma once

#include "Public/Utils/Macros.h"
#include "Public/Utils/TemplateTypeHelpers.h"

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <utility>
#include <forward_list>
#include <memory>

class MemoryManager
{
  public:
    class MemoryPtr;

  private:
    struct DynamicSortContainer
    {
      class MemoryVector
      {
        public:
          MemoryVector() NoExcept = default;
          MemoryVector( size_t Capacity ) NoExcept;

          MemoryVector( MemoryVector &&Move ) NoExcept;

          ~MemoryVector() NoExcept;

        public:
          MemoryPtr * emplace( MemoryPtr &&Move ) NoExcept;
          void erase( Size_T Index ) NoExcept;

        public:
          MemoryPtr * begin() NoExcept;
          const MemoryPtr* end() const NoExcept;

          size_t size()     const NoExcept;
          size_t capacity() const NoExcept;

        public:
          const MemoryPtr * GetIndex( Size_T Index ) const NoExcept;

        private:
          MemoryPtr * PtrArray = nullptr;

          const size_t Capacity;
          size_t Size = 0;
      };

      private:
        using DataType = std::forward_list< MemoryVector >;

      public:
        DynamicSortContainer() NoExcept = default;
        DynamicSortContainer( size_t Capacity ) NoExcept;

      public:
        MemoryPtr * emplace( MemoryPtr &&Node ) NoExcept;

        void clear() NoExcept;
        void reserve( size_t Capacity ) NoExcept;

        void erase( MemoryPtr *Ptr ) NoExcept;

        DataType::iterator       begin() NoExcept;
        DataType::const_iterator end()   NoExcept;

      private:
        void Grow( size_t Capacity ) NoExcept;

      private:
        DataType Data;
  
      //std::vector< PtrHash > HashTables;
    };

    struct MemoryPtrSetSorter
    {
      bool operator()( const MemoryPtr *const Lhs, const MemoryPtr *const Rhs ) const NoExcept;
    };

  public:
    using MemoryPtrSet = std::unordered_set< MemoryPtr*, std::hash< MemoryPtr* >, MemoryPtrSetSorter >;

  public:
    class MemoryPtr
    {
      struct TempMemoryPtrDtor
      {
        void operator()( MemoryPtr *const Ptr ) const NoExcept;
      };

      public:
        using TempMemoryPtr = std::unique_ptr< MemoryPtr, TempMemoryPtrDtor >;

      public:
        MemoryPtr() NoExcept;

        MemoryPtr( const MemoryPtr &Copy ) NoExcept = delete; // Can't use this becouse of how the Manager referances Ptrs

        ~MemoryPtr() NoExcept;

      private:
        MemoryPtr( void *Ptr ) NoExcept; // Used only for finding an item in a set

        MemoryPtr( void *Ptr, size_t PtrSize, MemoryManager *Manager ) NoExcept;
        MemoryPtr( void *Ptr, size_t PtrSize, MemoryManager *Manager, bool UserCreated ) NoExcept;
        DebugOnly( MemoryPtr( void *Ptr, size_t PtrSize, size_t NegitivePtrSize, MemoryManager *Manager ) NoExcept );

        MemoryPtr( MemoryPtr &&Move ) NoExcept; // DON'T USE! FOR STL ONLY!

      public:
        MemoryPtr & operator=( const MemoryPtr &Copy ) NoExcept = delete;

        TempMemoryPtr operator+( Size_T CharCount ) const NoExcept; // Increases by one character per CharCount
        TempMemoryPtr operator-( Size_T CharCount ) const NoExcept; // Decreases by one character per CharCount
        TempMemoryPtr operator+( int CharCount ) const NoExcept; // Increases/Decreases by one character per CharCount
        TempMemoryPtr operator-( int CharCount ) const NoExcept; // Decreases/Increases by one character per CharCount

        bool operator<( const MemoryPtr &Rhs ) const NoExcept;
        bool operator>( const MemoryPtr &Rhs ) const NoExcept;

        bool operator!=( const MemoryPtr &Rhs ) const NoExcept;
        bool operator==( const MemoryPtr &Rhs ) const NoExcept;
        bool operator<=( const MemoryPtr &Rhs ) const NoExcept;
        bool operator>=( const MemoryPtr &Rhs ) const NoExcept;

        operator bool() const NoExcept;
        operator unsigned char () const NoExcept;
        operator unsigned short() const NoExcept;
        operator unsigned int  () const NoExcept;
        operator unsigned long () const NoExcept;
        operator unsigned long long() const NoExcept;

      private:
        void operator=( MemoryPtr &&Move ) NoExcept; // DON'T USE! FOR STL ONLY!

      public:
        template< typename DataType >
        DataType GetData() const NoExcept
        {
          StaticAssert( sizeof( DataType ) > sizeof( void* ),
                        "The Type you are trying to Get from a MemoryPtr is larger than a reference, use the unsafe GetData!" );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );
          
          return *( reinterpret_cast< const DataType* >( Ptr ) );
        }

        template< typename DataType >
        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted so do not save the reference!
        const DataType & GetData_Unsafe() const NoExcept
        {
          StaticAssert( sizeof( DataType ) <= sizeof( void* ),
                        "The Type you are trying to Get from a MemoryPtr is smaller or equal to a reference, use the normal GetData!" );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          return *( reinterpret_cast< const DataType* >( Ptr ) );
        }

        template< typename DataType >
        DataType && GetData_Move() NoExcept
        {
          StaticAssert( IsTypeMoveAble< DataType >::value,
                        "The data you are trying to Get in the MemoryPtr cannot be Moved!" );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          return std::move( *( reinterpret_cast< DataType* >( Ptr ) ) );
        }

        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted so usee at your own risk!
        template< typename DataType >
        DataType * GetData_Ptr() NoExcept
        {
          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          return reinterpret_cast< DataType* >( Ptr );
        }

      public:
        template< typename DataType >
        //! \brief The data returned from this will NOT be corrupted if the Ptr gets corrupted.
        DataType GetData( const Size_T Index ) const NoExcept
        {
          DebugAssert( !Index, "Use the no-arg GetData for the MemoryPtr insted!", );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                       "The Index you are trying to Get from a MemoryPtr is too far for the size of memory it contains!", );

          return *( reinterpret_cast< const DataType* >( Ptr ) + Index );
        }

        template< typename DataType >
        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted.
        const DataType & GetData_Unsafe( const Size_T Index ) const NoExcept
        {
          StaticAssert( sizeof( DataType ) <= sizeof( void* ),
                        "The Type you are trying to Get from a MemoryPtr is smaller or equal to a reference, use the normal GetData!" );

          DebugAssert( !Index, "Use the no-arg GetData_Unsafe for the MemoryPtr insted!", );      

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                       "The Index you are trying to Get from a MemoryPtr is too far for the size of memory it contains!", );

          return *( reinterpret_cast< const DataType* >( Ptr ) + Index );
        }

        template< typename DataType >
        DataType && GetData_Move( const Size_T Index ) NoExcept
        {
          StaticAssert( IsTypeMoveAble< DataType >::value,
                        "The data you are trying to Get in the MemoryPtr cannot be Moved!" );

          DebugAssert( !Index, "Use the no-arg GetData_Move for the MemoryPtr insted!", );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                       "The Index you are trying to Get from a MemoryPtr is too far for the size of memory it contains!", );

          return std::move( *( reinterpret_cast< DataType* >( Ptr ) + Index ) );
        }

        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted so usee at your own risk!
        template< typename DataType >
        DataType * GetData_Ptr( const Size_T Index ) NoExcept
        {
          DebugAssert( !Index, "Use the no-arg GetData for the MemoryPtr insted!", );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Get from a MemoryPtr is too large for the size of memory it contains!", );

          DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                       "The Index you are trying to Get from a MemoryPtr is too far for the size of memory it contains!", );

          return reinterpret_cast< DataType* >( Ptr ) + Index;
        } 

      public:
        template< typename DataType >
        void SetData_Trivial( const DataType Value ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value != SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should be Copied!" );

          SetData_Trivial_Forced( Value );
        }

        void SetData_Memcpy( const void *const Value, const size_t ValueSize ) NoExcept;

        template< typename DataType >
        void SetData_Move( DataType &&Value ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should not be Moved but Trivially Copied!" );

          StaticAssert( !( IsTypeMoveAble< DataType >::value ),
                        "The Type you are trying to Set in the MemoryPtr cannot be Moved!" );

          SetData_Move_Forced( std::move( Value )  );
        }

        template< typename DataType >
        void SetData_Copy( const DataType &Value ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should not be Moved but Trivially Copied!" );

          SetData_Copy_Forced( Value );
        }

        template< typename DataType >
        void SetData_Copy_Forced( const DataType &Value ) NoExcept
        {
          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

          *( reinterpret_cast< DataType* >( Ptr ) ) = Value;
        }

      public:
        template< typename DataType >
        void SetData_Trivial( const DataType Value, const Size_T Index ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value != SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should be Copied!" );

          SetData_Trivial_Forced( Value, Index );
        }

        void SetData_Memcpy( const void *const Value, const size_t ValueSize, const Size_T Index ) NoExcept;

        template< typename DataType >
        void SetData_Move( DataType &&Value, const Size_T Index ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should not be Moved but Trivially Copied!" );

          StaticAssert( !( IsTypeMoveAble< DataType >::value ),
                        "The Type you are trying to Set in the MemoryPtr cannot be Moved!" );

          SetData_Move_Forced( std::move( Value ), Index );
        }

        template< typename DataType >
        void SetData_Copy( const DataType &Value, const Size_T Index ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Memset,
                         "The data you are trying to Set in the MemoryPtr should not be Copied but Memset!" );

          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                         "The data you are trying to Set in the MemoryPtr should not be Copied but Trivially Copied!" );

          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Memcpy,
                         "The data you are trying to Set in the MemoryPtr should not be Copied but Memcopied!" );

          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Move,
                         "The data you are trying to Set in the MemoryPtr should not be Copied but Moved!" );

          SetData_Copy_Forced( Value, Index );
        } 
        
        template< typename DataType >
        void SetData_Copy_Forced( const DataType &Value, const Size_T Index ) NoExcept
        {
          DebugAssert( !Index, "Use the no-index SetData for the MemoryPtr insted!", );

          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Set in from a MemoryPtr is too large for the size of memory it contains!", );

          DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                       "The Index you are trying to Set in from a MemoryPtr is too far for the size of memory it contains!", );

          *( reinterpret_cast< DataType* >( Ptr ) + Index ) = Value;
        }

      public:
        void SetData_Memset( const bool Value,  const Size_T Index, const Size_T Count ) NoExcept;
        void SetData_Memset( const char Value,  const Size_T Index, const Size_T Count ) NoExcept;
        void SetData_Memset( const UChar Value, const Size_T Index, const Size_T Count ) NoExcept;

        template< typename DataType >
        void SetData_Trivial( const DataType Value, const Size_T Index, const Size_T Count ) NoExcept
        {
          StaticAssert( IsTypeMemsetAble< DataType >::value,
                        "The data you are trying to Set in the MemoryPtr should be Memset!" );

          StaticAssert( SetFuncType< DataType >::value != SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should be Copied!" );

          SetData_Trivial_Forced( Value, Index, Count );
        }

        template< typename DataType >
        void SetData_Move( DataType &&Value, Size_T Index, Size_T Count ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should not be Moved but Trivially Copied!" );

          StaticAssert( !( IsTypeMoveAble< DataType >::value ),
                        "The Type you are trying to Set in the MemoryPtr cannot be Moved!" );

          SetData_Move_Forced( Value, Index, Count );
        }

        template< typename DataType >
        void SetData_Copy( const DataType &Value, const Size_T Index, const Size_T Count ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should not be Copied but Trivially Copied!" );

          SetData_Copy_Forced( Value, Index, Count );
        }

        template< typename DataType >
        void SetData_Copy_Forced( const DataType &Value, Size_T Index, Size_T Count ) NoExcept
        {
          //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

          DebugAssert( Count < 2, "Use the no-count SetData_Move for the MemoryPtr insted!", return );

          DebugAssert( Index * sizeof( DataType ) + Count * sizeof( DataType ) > PtrSize,
                       "The Count you are trying to Set in a MemoryPtr is too many for the size of memory it contains!", return );

          if( Index )
          {
            for( Count += Index; Index < Count; ++Index )
            {
              SetData_Copy_Forced( Value, Index );
            }
          }
          else
          {
            SetData_Copy_Forced( Value );

            for( ++Index; Index < Count; ++Index )
            {
              SetData_Copy_Forced( Value, Index );
            }
          }
        }

      public:
        template< typename DataType >
        void SetData_Memcpy_Array( const DataType *const Array, const Size_T Index, const Size_T Count ) NoExcept
        {
          StaticAssert( !( IsTypeMemcpyAble< DataType >::value ),
                        "The Type you are trying to Set in the MemoryPtr cannot be MemCopied!" );

          //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

          DebugAssert( Index * sizeof( DataType ) + Count * sizeof( DataType ) > PtrSize,
                       "The sizeof( Array ) + Index + Count goes out of bounds!", return );

          DebugAssert( Count < 2, "Count cannot be less then 2 for MemCopy!", );

          std::memcpy( reinterpret_cast< DataType* >( Ptr ) + Index, Array, sizeof( DataType ) * Count );
        }

        void SetData_Memcpy_Array( const void *Array, size_t ArraySize, Size_T Index, Size_T Count ) NoExcept;

        template< typename DataType >
        void SetData_Move_Array( DataType *const Array, const Size_T Index, const Size_T Count ) NoExcept
        {
          StaticAssert( !( IsTypeMoveAble< DataType >::value ),
                        "The Type you are trying to Set in the MemoryPtr cannot be Moved!" );

          //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

          DebugAssert( Index * sizeof( DataType ) + Count * sizeof( DataType ) > PtrSize,
                       "The sizeof( Array ) + Index + Count goes out of bounds!", return );

          DebugAssert( Count < 2, "Count cannot be less then 2 for MoveArray!", );

          if( Index )
          {
            for( Count += Index; Index < Count; ++Index )
            {
              SetData_Move_Forced( std::move( Array[ Index ] ), Index );
            }
          }
          else
          {
            SetData_Move_Forced( std::move( *Array ) );

            for( ++Index; Index < Count; ++Index )
            {
              SetData_Move_Forced( std::move( Array[ Index ] ), Index );
            }
          }
        }

        template< typename DataType >
        void SetData_Copy_Array( const DataType *const Array, const Size_T Index, const Size_T Count ) NoExcept
        {
          StaticAssert( SetFuncType< DataType >::value == SetFuncTypes::Trivial,
                        "The data you are trying to Set in the MemoryPtr should not be Copied but MemCopied!" );

          //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

          DebugAssert( Index * sizeof( DataType ) + Count * sizeof( DataType ) > PtrSize,
                       "The sizeof( Array ) + Index + Count goes out of bounds!", return );

          DebugAssert( Count < 2, "Count cannot be less then 2 for CopyArray!", );

          if( Index )
          {
            for( Count += Index; Index < Count; ++Index )
            {
              SetData_Copy_Forced( Array[ Index ], Index );
            }
          }
          else
          {
            SetData_Copy_Forced( *Array );

            for( ++Index; Index < Count; ++Index )
            {
              SetData_Copy_Forced( Array[ Index ], Index );
            }
          }
        }

      protected:
        template< typename DataType >
        void SetData_Trivial_Forced( const DataType Value ) NoExcept
        {
          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

          *( reinterpret_cast< DataType* >( Ptr ) ) = Value;
        }

        template< typename DataType >
        void SetData_Move_Forced( DataType &&Value ) NoExcept
        {
          DebugAssert( sizeof( DataType ) > PtrSize,
                       "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

          *( reinterpret_cast< DataType* >( Ptr ) ) = std::move( Value );
        }

      protected:
        template< typename DataType >
        void SetData_Trivial_Forced( const DataType Value, const Size_T Index ) NoExcept
        {
          DebugAssert( !Index, "Use the no-index SetData_Trivial for the MemoryPtr insted!", );

          DebugAssert( sizeof( DataType ) > PtrSize,
                        "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

          DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                        "The Index you are trying to Set in a MemoryPtr is too far for the size of memory it contains!", return );

          *( reinterpret_cast< DataType* >( Ptr ) + Index ) = Value;
        }

        template< typename DataType >
        void SetData_Move_Forced( DataType &&Value, const Size_T Index ) NoExcept
          {
            DebugAssert( !Index, "Use the no-index SetData for the MemoryPtr insted!", );

            DebugAssert( sizeof( DataType ) > PtrSize,
                         "The Type you are trying to Set in a MemoryPtr is too large for the size of memory it contains!", return );

            DebugAssert( sizeof( DataType ) * ( Index + 1 ) > PtrSize,
                         "The Index you are trying to Set in a MemoryPtr is too far for the size of memory it contains!", return );

            *( reinterpret_cast< DataType* >( Ptr ) + Index ) = std::move( Value );
          }

      protected:
        template< typename DataType >
        void SetData_Trivial_Forced( const DataType Value, Size_T Index, Size_T Count ) NoExcept
        {
          //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

          DebugAssert( Count < 2, "Use the no-count SetData_Trivial for the MemoryPtr insted!", return );

          DebugAssert( Index * sizeof( DataType ) + Count * sizeof( DataType ) > PtrSize,
                        "The Count you are trying to Set in a MemoryPtr is too many for the size of memory it contains!", return );

          if( Index )
          {
            for( Count += Index; Index < Count; ++Index )
            {
              SetData_Trivial_Forced( Value, Index );
            }
          }
          else
          {
            SetData_Trivial_Forced( Value );

            for( ++Index; Index < Count; ++Index )
            {
              SetData_Trivial_Forced( Value, Index );
            }
          }
        }

        template< typename DataType >
        void SetData_Move_Forced( DataType &&Value, Size_T Index, Size_T Count ) NoExcept
        {
          //DebugAssert( !Index, "Use the no-index SetData_Memset for the MemoryPtr insted!", ); // Can't have since 0 index is needed to fill array

          DebugAssert( Count < 2, "Use the no-count SetData_Move for the MemoryPtr insted!", return );

          DebugAssert( Index * sizeof( DataType ) + Count * sizeof( DataType ) > PtrSize,
                        "The Count you are trying to Set in a MemoryPtr is too many for the size of memory it contains!", return );

          if( Index )
          {
            for( Count += Index - 1; Index < Count; ++Index )
            {
              SetData_Copy_Forced( Value, Index );
            }

            SetData_Move_Forced( std::move( Value ), Index );
          }
          else
          {
            SetData_Copy_Forced( Value );

            for( --Count, ++Index; Index < Count; ++Index )
            {
              SetData_Copy_Forced( Value, Index );
            }

            SetData_Move_Forced( std::move( Value ), Index );
          }
        }

      private:
        void *const Ptr;
        const size_t PtrSize;

        DebugOnly( size_t NegitivePtrSize = 0 ); // Used to check for underflow in debugging

        MemoryManager *const Manager;

        const bool UserCreated = false; // This allows for faster findings, debug checking, and temp MemPtrs

      private:
        friend MemoryManager;
        friend DynamicSortContainer;

        template< class _Alloc >
        friend struct std::_Default_allocator_traits; // This needs access to the Ptr's Move Ctor
    };

  private:
    template< typename MemoryData >
    class StaticMemoryPtrBase : public MemoryPtr
    {
      public:     
        MemoryData GetData() const NoExcept { return MemoryPtr::GetData< MemoryData >(); }
        
        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted so do not save the reference!
        const MemoryData & GetData_Unsafe() const NoExcept { return MemoryPtr::GetData_Unsafe< MemoryData >(); }
        
        MemoryData && GetData_Move() NoExcept { return MemoryPtr::GetData_Move< MemoryData >(); }

        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted so usee at your own risk!        
        MemoryData * GetData_Ptr() NoExcept { return MemoryPtr::GetData_Ptr< MemoryData >(); }

      public:
        //! \brief The data returned from this will NOT be corrupted if the Ptr gets corrupted.
        MemoryData GetData( const Size_T Index ) const NoExcept { return MemoryPtr::GetData< MemoryData >( Index ); }
        
        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted.
        const MemoryData & GetData_Unsafe( const Size_T Index ) const NoExcept { return MemoryPtr::GetData_Unsafe< MemoryData >( Index ); }
        
        MemoryData && GetData_Move( const Size_T Index ) NoExcept { return MemoryPtr::GetData_Move< MemoryData >( Index ); }

        //! \brief If the Ptr gets invalidated, the data returned from this will be corrupted so usee at your own risk!        
        MemoryData * GetData_Ptr( const Size_T Index ) NoExcept { return MemoryPtr::GetData_Ptr< MemoryData >( Index ); }

      public:       
        void SetData( FuncArgType < MemoryData > Value ) NoExcept
        {
          using MemoryPtrType = MemoryPtr;

          using TrivialFuncType = PtrToMemberFunction< void, MemoryPtrType, const MemoryData   >;
          using CopyFuncType    = PtrToMemberFunction< void, MemoryPtrType, const MemoryData & >;

          ( static_cast< MemoryPtrType* >( this )->*( RunSetFuncFunction< SetFuncType< MemoryData >::value, TrivialFuncType, CopyFuncType >
                                                      { TrivialFuncType{ &MemoryPtrType::SetData_Trivial_Forced }, 
                                                        CopyFuncType{ &MemoryPtrType::SetData_Copy_Forced } }.RunFunc.MemberFunc ) )( Value );
        }

        void SetData_Move( MemoryData &&Value ) NoExcept { MemoryPtr::SetData_Move( std::move( Value ) ); }
      
        void SetData_Copy_Forced( const MemoryData &Value ) NoExcept { MemoryPtr::SetData_Copy_Forced( Value ); }
      
      public:
        void SetData( const MemoryData Value, const Size_T Index ) NoExcept
        { 
          using MemoryPtrType = MemoryPtr;

          using TrivialFuncType = PtrToMemberFunction< void, MemoryPtrType, const MemoryData  , Size_T >;
          using CopyFuncType    = PtrToMemberFunction< void, MemoryPtrType, const MemoryData &, Size_T >;

          ( static_cast< MemoryPtrType* >( this )->*( RunSetFuncFunction< SetFuncType< MemoryData >::value, TrivialFuncType, CopyFuncType >
                                                      { TrivialFuncType{ &MemoryPtrType::SetData_Trivial_Forced },
                                                        CopyFuncType{ &MemoryPtrType::SetData_Copy_Forced } }.RunFunc.MemberFunc ) )( Value, Index );
        }
        
        void SetData_Move( MemoryData &&Value, const Size_T Index ) NoExcept { MemoryPtr::SetData_Move( std::move( Value ), Index ); }
        
        void SetData_Copy_Forced( const MemoryData &Value, const Size_T Index ) NoExcept { MemoryPtr::SetData_Copy_Forced( Value, Index ); }

      public:
        void SetData( const MemoryData Value, const Size_T Index, const Size_T Count ) NoExcept
        {
          using MemoryPtrType = MemoryPtr;

          using TrivialFuncType = PtrToMemberFunction< void, MemoryPtrType, const MemoryData  , Size_T, Size_T >;
          using CopyFuncType    = PtrToMemberFunction< void, MemoryPtrType, const MemoryData &, Size_T, Size_T >;

          ( static_cast< MemoryPtrType* >( this )->*( RunSetFuncFunction< SetFuncType< MemoryData >::value, TrivialFuncType, CopyFuncType >
                                                      { TrivialFuncType{ &MemoryPtrType::SetData_Trivial_Forced },
                                                        CopyFuncType{ &MemoryPtrType::SetData_Copy_Forced } }.RunFunc.MemberFunc ) )( Value, Index, Count );
        }

        void SetData_Move( MemoryData &&Value, const Size_T Index, const Size_T Count ) NoExcept 
        { MemoryPtr::SetData_Move( std::move( Value ), Index, Count ); }

        void SetData_Copy_Forced( const MemoryData &Value, const Size_T Index, const Size_T Count ) NoExcept
        { MemoryPtr::SetData_Copy_Forced( Value, Index, Count ); }

      protected:
        void SetData_Trivial_Forced( const MemoryData Value ) NoExcept { MemoryPtr::SetData_Trivial_Forced( Value ); }

        void SetData_Move_Forced( MemoryData &&Value ) NoExcept { MemoryPtr::SetData_Move_Forced( std::move( Value ) ); }

      protected:
        void SetData_Trivial_Forced( const MemoryData Value, const Size_T Index ) NoExcept { MemoryPtr::SetData_Trivial_Forced( Value, Index ); }

        void SetData_Move_Forced( MemoryData &&Value, const Size_T Index ) NoExcept { MemoryPtr::SetData_Move_Forced( std::move( Value ), Index ); }

      protected:
        void SetData_Trivial_Forced( const MemoryData Value, const Size_T Index, const Size_T Count ) NoExcept
        { MemoryPtr::SetData_Trivial_Forced( Value, Index, Count ); }

        void SetData_Move_Forced( MemoryData &&Value, const Size_T Index, const Size_T Count ) NoExcept
        { MemoryPtr::SetData_Move_Forced( std::move( Value ), Index, Count ); }

      public:
        void SetData_Memcpy_Array( const MemoryData *const Array, const Size_T Index, const Size_T Count ) NoExcept
        { MemoryPtr::SetData_Memcpy_Array( Array, Index, Count ); }

        void SetData_Move_Array( const MemoryData *const Array, const Size_T Index, const Size_T Count ) NoExcept 
        { MemoryPtr::SetData_Move_Array( Array, Index, Count ); }

        void SetData_Copy_Array( const MemoryData *const Array, const Size_T Index, const Size_T Count ) NoExcept
        { MemoryPtr::SetData_Copy_Array( Array, Index, Count ); }

      private:
        friend MemoryManager;
    };

  public:
    template< typename MemoryData >
    class StaticMemoryPtr : public StaticMemoryPtrBase< MemoryData >
    {
      private:
        void SetData_Memset( const MemoryData, const Size_T, const Size_T ) NoExcept {}

      private:
        friend MemoryManager;
    };

    template<>
    class StaticMemoryPtr< bool > : public StaticMemoryPtrBase< bool >
    {
      public:
        void SetData_Memset( const bool Value, const Size_T Index, const Size_T Count ) NoExcept { MemoryPtr::SetData_Memset( Value, Index, Count ); }
    };

    template<>
    class StaticMemoryPtr< char > : public StaticMemoryPtrBase< char >
    {
      public:
        void SetData_Memset( const char Value, const Size_T Index, const Size_T Count ) NoExcept { MemoryPtr::SetData_Memset( Value, Index, Count ); }
    };

    template<>
    class StaticMemoryPtr< UChar > : public StaticMemoryPtrBase< UChar >
    {
      public:
        void SetData_Memset( const UChar Value, const Size_T Index, const Size_T Count ) NoExcept { MemoryPtr::SetData_Memset( Value, Index, Count ); }
    };

  public:
    MemoryManager() NoExcept;


    MemoryManager( const MemoryManager  &Copy ) NoExcept;
    MemoryManager(       MemoryManager &&Move ) NoExcept; // This is not your normal move, MemPtrs are NOT moved!

    MemoryManager( const void *DefualtValue ) NoExcept;
    MemoryManager( const void *DefualtValue, size_t ValueSize ) NoExcept;

    MemoryManager( Size_T Capacity ) NoExcept;
    MemoryManager( Size_T Capacity, size_t RegularDataSize ) NoExcept;
    MemoryManager( Size_T Capacity, size_t RegularDataSize, const void *DefualtValue ) NoExcept;
    MemoryManager( Size_T Capacity, size_t RegularDataSize, const void *DefualtValue, size_t ValueSize ) NoExcept;


    ~MemoryManager() NoExcept;

  public:
    void operator=( const MemoryManager  &Copy ) NoExcept;
    void operator=(       MemoryManager &&Move ) NoExcept; // This is not your normal move, MemPtrs are NOT moved!

  public:
    MemoryPtrSet::iterator begin() NoExcept;
    MemoryPtrSet::const_iterator cbegin() const NoExcept;

    MemoryPtrSet::iterator end() NoExcept;
    MemoryPtrSet::const_iterator cend() const NoExcept;

  public:
    MemoryPtr & Allocate() NoExcept;
    MemoryPtr & Allocate( size_t DataSize ) NoExcept;

    MemoryPtr & Allocate( const void *DefaultValue ) NoExcept;
    MemoryPtr & Allocate( const void *DefaultValue, size_t ValueSize ) NoExcept;

    MemoryPtr & Allocate( size_t DataSize, const void *DefaultValue ) NoExcept;
    MemoryPtr & Allocate( size_t DataSize, const void *DefaultValue, size_t ValueSize ) NoExcept;

    MemoryPtr & Allocate( size_t DataSize, Size_T DataCount, const void *ValueArray ) NoExcept;

  public:
    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate() NoExcept { return Allocate_Internal( sizeof( MemoryType ) ); } // RVO

    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate( const Size_T ArrayCount ) NoExcept
    {
      DebugAssert( !ArraySize, "Use the no-arg template Allocate for the MemoryManager instead!", return NullPtr; );

      return Allocate_Internal( sizeof( MemoryType ) * ArrayCount ); // RVO 
    }

    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate_Init( FuncArgType< MemoryType > DefaultValue ) NoExcept
    {
      using MemoryPtrType = StaticMemoryPtr< MemoryType >;

      using TrivialFuncType = PtrToMemberFunction< void, MemoryPtrType, const MemoryType   >;
      using CopyFuncType    = PtrToMemberFunction< void, MemoryPtrType, const MemoryType & >;

      auto &Ptr = static_cast< MemoryPtrType & >( Allocate_Internal( sizeof( MemoryType ) ) );

      ( Ptr.*( RunSetFuncFunction< SetFuncType< MemoryType >::value, TrivialFuncType, CopyFuncType >
               { TrivialFuncType{ &MemoryPtrType::SetData_Trivial_Forced }, CopyFuncType{ &MemoryPtrType::SetData_Copy_Forced } }.
                 RunFunc.MemberFunc ) )( DefaultValue );

      return Ptr; // NRVO 
    }

    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate_Init( MemoryType &&DefaultValue ) NoExcept
    {
      auto &Ptr = static_cast< StaticMemoryPtr< MemoryType > & >( Allocate_Internal( sizeof( MemoryType ) ) );

      Ptr.SetData_Move_Forced( std::move( DefaultValue ) );

      return Ptr; // NRVO 
    }

    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate( const Size_T ArrayCount, FuncArgType< MemoryType > DefaultValue ) NoExcept
    {
      using MemoryPtrType = StaticMemoryPtr< MemoryType >;

      DebugAssert( !ArrayCount, "Use the no-ArraySize template Allocate for the MemoryManager instead!",
                   return static_cast< MemoryPtrType& >( NullPtr ); );

      using TrivialFuncType = PtrToMemberFunction< void, MemoryPtrType, const MemoryType  , Size_T, Size_T >;
      using CopyFuncType    = PtrToMemberFunction< void, MemoryPtrType, const MemoryType &, Size_T, Size_T >;

      auto &Ptr = static_cast< MemoryPtrType & >( Allocate_Internal( sizeof( MemoryType ) * ArrayCount ) );

      ( Ptr.*( RunMemsetFuncFunction< MemsetFuncType< MemoryType >::value, TrivialFuncType, TrivialFuncType, CopyFuncType >
               { TrivialFuncType{ &MemoryPtrType::SetData_Memset }, TrivialFuncType{ &MemoryPtrType::SetData_Trivial_Forced },
                 CopyFuncType{ &MemoryPtrType::SetData_Copy_Forced } }.RunFunc.MemberFunc ) )( DefaultValue, 0, ArrayCount );

      return Ptr; // NRVO 
    }

    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate( const Size_T ArrayCount, const MemoryType *const ValueArray ) NoExcept
    {
      using MemoryPtrType = StaticMemoryPtr< MemoryType >;

      DebugAssert( ArraySize < 2, "ArraySize cannot be less then two when initializing with another array!", 
                   return static_cast< MemoryPtrType & >( NullPtr ); );

      using MemcpyFuncType  = PtrToMemberFunction< void, MemoryPtrType, const MemoryType, Size_T, Size_T >;
      using CopyFuncType    = PtrToMemberFunction< void, MemoryPtrType, const MemoryType &, Size_T, Size_T >;

      auto &Ptr = static_cast< MemoryPtrType & >( Allocate_Internal( sizeof( MemoryType ) * ArrayCount ) );

      ( Ptr.*( RunMemcpyFuncFunction< MemcpyFuncType< MemoryType >::value, MemcpyFuncType, CopyFuncType >
               { MemcpyFuncType{ &MemoryPtrType::SetData_Memcpy_Array }, CopyFuncType{ &MemoryPtrType::SetData_Copy_Array } }.
               RunFunc.MemberFunc ) )( ValueArray, 0, ArrayCount );

      return Ptr; // NRVO 
    }

    template< typename MemoryType >
    StaticMemoryPtr< MemoryType > & Allocate( const Size_T ArrayCount, MemoryType* &&ValueArray ) NoExcept
    {
      using MemoryPtrType = StaticMemoryPtr< MemoryType >;

      DebugAssert( ArraySize < 2, "ArraySize cannot be less then two when initializing with another array!",
                   return static_cast< MemoryPtrType & >( NullPtr ); );

      auto &Ptr = static_cast< MemoryPtrType & >( Allocate_Internal( sizeof( MemoryType ) * ArrayCount ) );

      Ptr.SetData_Move_Array( ValueArray, 0, ArrayCount );

      return Ptr; // NRVO 
    }

  public:
    void Free( MemoryPtr &DataPtr ) NoExcept;
    void Free( MemoryPtr *DataPtr ) NoExcept;

    //void ShrinkToFit() NoExcept;

  public:
    static constexpr Size_T DefaultSize = sizeof( void* ); // Using sizeof( void* ) to get efficient DefaultSize
    static constexpr Size_T STLDefaultCapacity = 15;       // 15 = stl default size for vector

  public:
    static MemoryPtr NullPtr;

  private:
    static const std::multimap< size_t, void * >::iterator * BinarySearch( const std::vector< std::multimap< size_t, void * >::iterator* > &Vector,
                                                                           const void *Value ) NoExcept;
  private:
    MemoryPtr & Allocate_Internal( size_t DataSize ) NoExcept;
    bool Compress() NoExcept;
    void Grow() NoExcept;

  private:
    static constexpr Size_T STLDefaultGrow = 2; // 2 = stl default grow for vector
    
  private:
    const size_t RegularDataSize;
          Size_T RegularCapacity;

          void *Data;
    const void *DataEnd;

    void *FreeIndex = Data;

    DynamicSortContainer PtrArray;

    MemoryPtrSet UsedData; // Data that is created from the MemoryManager

    std::multimap< size_t, void * > FreedDataBySize;
    std::unordered_map< void *, size_t > FreedDataByPtr;

    MemoryPtrSet PtrSpawnedData; // Data that is created from exisiting MemoryPtrs

  private:
    friend MemoryPtr;
};
