/*****************************************************************************/
/*!
\file   ObjectAllocator.h
\author Garrett Conti
\date   9/9/2017
*/
/*****************************************************************************/

#pragma once

#include <string>
#include <type_traits>

#include "Tracer.h"

/*!
 * \brief This memory manager class 
 */
template< typename ObjectType_ >
class ObjectAllocator
{
  public:
    typedef typename std::enable_if< !( std::is_reference< ObjectType_ >::value ), ObjectType_ >::type ObjectType;

  public:
    class ObjectPtr
    {
      public:
        ObjectPtr() noexcept = default;
        explicit ObjectPtr( ObjectType *Object ) noexcept;

      public:
        ObjectType & operator*() noexcept;
        const ObjectType & operator*() const noexcept;

        ObjectType * operator->() noexcept;
        const ObjectType * operator->() const noexcept;

      private:
        ObjectType *Object = nullptr;

      private:
        friend ObjectAllocator;
    };
  
    class ObjectArray
    {
      public:
        class ArrayPtr
        {
          public:
            explicit ArrayPtr( size_t Index, ObjectArray &Array ) noexcept;

          public:
            ObjectType & operator*() noexcept;
            const ObjectType & operator*() const noexcept;

            ObjectType * operator->() noexcept;
            const ObjectType * operator->() const noexcept;

            ArrayPtr operator+( size_t Index ) const ExceptDebug;
            ArrayPtr & operator+=( size_t Index ) ExceptDebug;
          
            ArrayPtr operator-( size_t Index ) const ExceptDebug;
            int operator-( const ArrayPtr &Rhs ) const noexcept;
            int operator-( const ObjectArray &Rhs ) const noexcept;
            ArrayPtr & operator-=( size_t Index ) ExceptDebug;

          private:
            size_t Index;
            ObjectArray &Array;

          private:
            friend class ObjectArray;
        };

      public:
        ObjectArray() noexcept = default;

        /*! 
         * \brief Creates the ObjectArray using inclusive start and end [Start,End]
         */
        explicit ObjectArray( ObjectType *Start, ObjectType *End ) noexcept;

      public:
        ObjectType & operator[]( size_t Index ) ExceptDebug;
        const ObjectType & operator[]( size_t Index ) const ExceptDebug;

        ObjectType & operator*() noexcept;
        const ObjectType & operator*() const noexcept;

        ObjectType * operator->() noexcept;
        const ObjectType * operator->() const noexcept;

        ArrayPtr operator+( size_t Index ) ExceptDebug;

        int operator-( const ObjectArray &Rhs ) const noexcept;
        int operator-( const ArrayPtr &Rhs ) const noexcept;

      private:
        struct IndexOutOfRange
        {
          IndexOutOfRange( size_t Index, size_t MaxIndex, bool Forward ) noexcept;
        };

      private:
        ObjectType *Start = nullptr;
        ObjectType *End = nullptr;

      private:
        friend ObjectAllocator;
    };

  public:
      /*! 
       * \brief Creates the ObjectAllocator
       */
    explicit ObjectAllocator( size_t ObjectsPerPage = 15 ) noexcept;

    ObjectAllocator( const ObjectAllocator &oa ) noexcept = delete;

      /*!
       * \brief Destroys the ObjectManager.
       */
    ~ObjectAllocator() noexcept;

  public:
    ObjectAllocator & operator=( const ObjectAllocator &oa ) noexcept = delete;

  public:
     /*!
      * \brief Allocates an object.
      */
    ObjectPtr New() noexcept;
  
     /*!
      * \brief Allocates an array of objects.
      */
    ObjectArray New( size_t Count ) noexcept;

     /*! 
      * \brief Frees one object.
      */
    void Delete( ObjectPtr &Object ) noexcept;

     /*!
      * \brief Frees an array of objects.
      */
    void Delete( ObjectArray &Object ) noexcept;

  private:
    struct ListPtr
    {
      ListPtr *Next;
    };

    struct HeaderBlockInfo
    {
      ListPtr *FreePtrs;
    };

  private:
    void AllocateNewPage( ListPtr *PagePtr ) noexcept; //!< Allocates another page of objects.

  private:
    static constexpr size_t PtrSize = sizeof( void * );
    static constexpr size_t HBISize = sizeof( HeaderBlockInfo );
    static constexpr size_t ObjectSize = sizeof( ObjectType );

  private:
    const size_t ObjectsPerPage; //!< Number of objects per page
    const size_t PageSize;       //!< Size of a page including all headers, padding, etc.

    ListPtr *const PageList; //!< Beginning of the list of pages
};

#define ClassType( ObjectAllocator ) decltype( ObjectAllocator )

#include "ObjectAllocator.cpp"
