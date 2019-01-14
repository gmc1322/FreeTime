/*****************************************************************************/
/*!
\file   ObjectAllocator.cpp
\author Garrett Conti
\date   9/9/2017
*/
/*****************************************************************************/

#include "ObjectAllocator.h"

template< typename ObjectType_ >
ObjectAllocator< ObjectType_ >::ObjectPtr::ObjectPtr( ObjectType *Object ) noexcept : Object( Object )
{
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType &
ObjectAllocator< ObjectType_ >::ObjectPtr::operator*() noexcept
{
  return *Object;
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType &
ObjectAllocator< ObjectType_ >::ObjectPtr::operator*() const noexcept
{
  return *Object;
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType *
ObjectAllocator< ObjectType_ >::ObjectPtr::operator->() noexcept
{
  return Object;
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType *
ObjectAllocator< ObjectType_ >::ObjectPtr::operator->() const noexcept
{
  return Object;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ObjectType_ >
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::ArrayPtr( size_t Index, ObjectArray &Array ) noexcept :
Index( Index ), Array( Array )
{
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType &
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator*() noexcept
{
  return Array[ Index ];
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType & 
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator*() const noexcept
{
  return Array[ Index ];
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType * 
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator->() noexcept
{
  return &( Array[ Index ] );
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType * 
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator->() const noexcept
{
  return &( Array[ Index ] );
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator+( size_t Index_ ) const ExceptDebug
{
  AssertDebug( Index + Index_ > static_cast< size_t >( Array.End - Array.Start ),
    ( IndexOutOfRange{ Index + Index_, static_cast< size_t >( Array.End - Array.Start ), true } ) );

  return ArrayPtr{ Index + Index_, Array };
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr & 
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator+=( size_t Index_ ) ExceptDebug
{
  AssertDebug( Index + Index_ > static_cast< size_t >( Array.End - Array.Start ),
    ( IndexOutOfRange{ Index + Index_, static_cast< size_t >( Array.End - Array.Start ), true } ) );

  Index += Index_;

  return *this;
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr 
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator-( size_t Index_ ) const ExceptDebug
{
  AssertDebug( Index_ > Index, ( IndexOutOfRange{ Index_, Index, false } ) );

  return ArrayPtr{ Index - Index_, Array };
}

template< typename ObjectType_ >
int ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator-( const ArrayPtr &Rhs ) const noexcept
{
  return ( Array.Start + Index ) - ( Rhs.Array.Start + Rhs.Index );
}

template< typename ObjectType_ >
int ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator-( const ObjectArray &Rhs ) const noexcept
{
  return ( Array.Start + Index ) - Rhs.Start;
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr & 
ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr::operator-=( size_t Index_ ) ExceptDebug
{
  AssertDebug( Index_ > Index, ( IndexOutOfRange{ Index_, Index, false } ) );

  Index -= Index_;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ObjectType_ >
ObjectAllocator< ObjectType_ >::ObjectArray::ObjectArray( ObjectType *Start, ObjectType *End ) noexcept :
Start( Start ), End( End )
{
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType & 
ObjectAllocator< ObjectType_ >::ObjectArray::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index > static_cast< size_t >( End - Start ), ( IndexOutOfRange{ Index, static_cast< size_t >( End - Start ), true } ) );

  return *( Start + Index );
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType &
ObjectAllocator< ObjectType_ >::ObjectArray::operator[]( size_t Index ) const ExceptDebug
{
  AssertDebug( Index > static_cast< size_t >( End - Start ), ( IndexOutOfRange{ Index, static_cast< size_t >( End - Start ), true } ) );

  return *( Start + Index );
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType &
ObjectAllocator< ObjectType_ >::ObjectArray::operator*() noexcept
{
  return *Start;
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType &
ObjectAllocator< ObjectType_ >::ObjectArray::operator*() const noexcept
{
  return *Start;
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectType *
ObjectAllocator< ObjectType_ >::ObjectArray::operator->() noexcept
{
  return Start;
}

template< typename ObjectType_ >
const typename ObjectAllocator< ObjectType_ >::ObjectType *
ObjectAllocator< ObjectType_ >::ObjectArray::operator->() const noexcept
{
  return Start;
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectArray::ArrayPtr
ObjectAllocator< ObjectType_ >::ObjectArray::operator+( size_t Index ) ExceptDebug
{
  AssertDebug( Index > static_cast< size_t >( End - Start ), ( IndexOutOfRange{ Index, static_cast< size_t >( End - Start ), true } ) );

  return ArrayPtr{ Index, *this };
}

template< typename ObjectType_ >
int ObjectAllocator< ObjectType_ >::ObjectArray::operator-( const ObjectArray &Rhs ) const noexcept
{
  return Start - Rhs.Start;
}

template< typename ObjectType_ >
int ObjectAllocator< ObjectType_ >::ObjectArray::operator-( const ArrayPtr &Rhs ) const noexcept
{
  return Start - ( Rhs.Array.Start + Rhs.Index );
}

template< typename ObjectType_ >
ObjectAllocator< ObjectType_ >::ObjectArray::IndexOutOfRange::IndexOutOfRange( size_t Index, size_t MaxIndex,
                                                                                               bool Forward ) noexcept
{
  if( Forward )
  {
    std::cerr << "You tried to go to index '" << Index << "' but the max index is '" << MaxIndex << "'!" << std::endl;
  }
  else
  {
    std::cerr << "You tried to go back '" << Index << "' indexes, but the current index is '" << MaxIndex << "'!" << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename ObjectType_ >
ObjectAllocator< ObjectType_ >::ObjectAllocator( size_t ObjectsPerPage_ = 15 ) noexcept :
ObjectsPerPage( ObjectsPerPage_ ), PageSize( ObjectsPerPage * ObjectSize + PtrSize + HBISize ),
PageList( reinterpret_cast< ListPtr * >( new char[ PageSize ] ) )
{
  char *const TempPtr = reinterpret_cast< char * >( PageList );

  HeaderBlockInfo *HBI = reinterpret_cast< HeaderBlockInfo * >( TempPtr + PtrSize );

  char *ObjectIter = TempPtr + PtrSize + HBISize;

  ListPtr* *Iter = &( HBI->FreePtrs );

  for( char *const End = TempPtr + PageSize; ObjectIter < End; ObjectIter += ObjectSize, Iter = &( ( *Iter )->Next ) )
  {
    *Iter = reinterpret_cast< ListPtr * >( ObjectIter );
  }

  *Iter = nullptr;

  PageList->Next = nullptr;
}

template< typename ObjectType_ >
ObjectAllocator< ObjectType_ >::~ObjectAllocator() noexcept
{
  ListPtr *PageIter = PageList;

  while( PageIter )
  {
    ListPtr *const NextPtr = PageIter->Next;

    delete[] PageIter;

    PageIter = NextPtr;
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectPtr ObjectAllocator< ObjectType_ >::New() noexcept
{
  ListPtr *PageIter = PageList;

  for( ; ; )
  {
    HeaderBlockInfo *HBI = reinterpret_cast< HeaderBlockInfo * >( reinterpret_cast< char * >( PageIter ) + PtrSize );

    if( HBI->FreePtrs ) // Make sure there are objects left
    {
      ObjectPtr Return{ reinterpret_cast< ObjectType * >( HBI->FreePtrs ) };

      HBI->FreePtrs = HBI->FreePtrs->Next;

      return Return; // Named RVO
    }

    if( !( PageIter->Next ) )
    {
      AllocateNewPage( PageIter );
    }

    PageIter = PageIter->Next;
  }
}

template< typename ObjectType_ >
typename ObjectAllocator< ObjectType_ >::ObjectArray 
ObjectAllocator< ObjectType_ >::New( size_t Count ) noexcept
{
  ListPtr *PageIter = PageList;

  for( ; ; )
  {
    HeaderBlockInfo *HBI = reinterpret_cast< HeaderBlockInfo * >( reinterpret_cast< char * >( PageIter ) + PtrSize );

    ListPtr *PrevPtr = nullptr;
    ListPtr *FreeIter = HBI->FreePtrs;

    while( FreeIter ) // Make sure there are objects left
    {
      ListPtr *OuterIter = FreeIter->Next;

      bool Found = true;

      for( size_t i = 1; ; OuterIter = OuterIter->Next )
      {
        if( !OuterIter || reinterpret_cast< char * >( OuterIter ) > reinterpret_cast< char* >( FreeIter ) + ObjectSize * i )
        {
          Found = false;

          break;
        }

        if( ++i == Count )
        {
          break;
        }
      }

      if( Found )
      {
        ObjectArray Return{ reinterpret_cast< ObjectType * >( FreeIter ), reinterpret_cast< ObjectType * >( OuterIter ) };

        if( PrevPtr )
        {
          PrevPtr->Next = OuterIter->Next;
        }
        else
        {
          HBI->FreePtrs = OuterIter->Next;
        }

        return Return; // Named RVO
      }
           
      PrevPtr = FreeIter;

      FreeIter = FreeIter->Next;
    }

    if( !( PageIter->Next ) )
    {
      AllocateNewPage( PageIter );
    }

    PageIter = PageIter->Next;
  }
}

template< typename ObjectType_ >
void ObjectAllocator< ObjectType_ >::Delete( ObjectPtr &Object ) noexcept
{
  ListPtr *PageIter = PageList;

  char *const ObjectAddress = reinterpret_cast< char * >( Object.Object );

  while( ObjectAddress < reinterpret_cast< char* >( PageIter ) ||
         ObjectAddress > reinterpret_cast< char * >( PageIter ) + PageSize )
  {
    PageIter = PageIter->Next;
  }

  HeaderBlockInfo *HBI = reinterpret_cast< HeaderBlockInfo * >( reinterpret_cast< char * >( PageIter ) + PtrSize );

  ListPtr *PrevIter = nullptr;
  ListPtr *FreeIter = HBI->FreePtrs;

  while( FreeIter && reinterpret_cast< char * >( FreeIter ) < ObjectAddress )
  {
    PrevIter = FreeIter;
    FreeIter = FreeIter->Next;
  }

  if( PrevIter )
  {
    PrevIter->Next = reinterpret_cast< ListPtr * >( ObjectAddress );
    PrevIter->Next->Next = FreeIter;
  }
  else
  {
    HBI->FreePtrs = reinterpret_cast< ListPtr * >( ObjectAddress );
    HBI->FreePtrs->Next = FreeIter;
  }
}

template< typename ObjectType_ >
void ObjectAllocator< ObjectType_ >::Delete( ObjectArray &Object ) noexcept
{
  ListPtr *PageIter = PageList;

  char *ObjectAddress = reinterpret_cast< char * >( Object.Start );

  while( ObjectAddress < reinterpret_cast< char * >( PageIter ) ||
         ObjectAddress > reinterpret_cast< char * >( PageIter ) + PageSize )
  {
    PageIter = PageIter->Next;
  }

  HeaderBlockInfo *HBI = reinterpret_cast< HeaderBlockInfo * >( reinterpret_cast< char * >( PageIter ) + PtrSize );

  ListPtr *PrevIter = nullptr;
  ListPtr *FreeIter = HBI->FreePtrs;

  while( FreeIter && reinterpret_cast< char * >( FreeIter ) < ObjectAddress )
  {
    PrevIter = FreeIter;
    FreeIter = FreeIter->Next;
  }

  char *const Last = reinterpret_cast< char * >( Object.End );

  if( PrevIter )
  {
    while( ObjectAddress <= Last )
    {
      PrevIter->Next = reinterpret_cast< ListPtr * >( ObjectAddress );

      ObjectAddress += ObjectSize;

      PrevIter = PrevIter->Next;
    }

    PrevIter->Next = FreeIter;
  }
  else
  {
    ListPtr* *Iter = &( HBI->FreePtrs );

    while( ObjectAddress <= Last )
    {
      *Iter = reinterpret_cast< ListPtr * >( ObjectAddress );

      ObjectAddress += ObjectSize;

      Iter = &( ( *Iter )->Next );
    }

    *Iter = FreeIter;
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename ObjectType_ >
void ObjectAllocator< ObjectType_ >::AllocateNewPage( ListPtr *PagePtr ) noexcept
{
  char *const TempPtr = new char[ PageSize ];

  HeaderBlockInfo *HBI = reinterpret_cast< HeaderBlockInfo * >( TempPtr + PtrSize );

  char *ObjectIter = TempPtr + PtrSize + HBISize;

  ListPtr* *Iter = &( HBI->FreePtrs );

  for( char *const End = TempPtr + PageSize; ObjectIter < End; ObjectIter += ObjectSize, Iter = &( ( *Iter )->Next ) )
  {
    *Iter = reinterpret_cast< ListPtr* >( ObjectIter );
  }

  *Iter = nullptr;

  PagePtr->Next = reinterpret_cast< ListPtr * >( TempPtr );

  PagePtr->Next->Next = nullptr;
}
