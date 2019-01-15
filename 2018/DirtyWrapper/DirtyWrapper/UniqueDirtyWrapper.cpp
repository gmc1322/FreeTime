/*!------------------------------------------------------------------------------
\file   UniqueDirtyWrapper.cpp

\author Garrett Conti

\par    Project: DirtyWrapper
\par    Course:  
   ------------------------------------------------------------------------------ */

#include "UniqueDirtyWrapper.h"

template< typename DataType >
UniqueParentDirtyWrapper< DataType >::UniqueParentDirtyWrapper( const DataType &Data ) noexcept : 
Data{ new DataType{ Data } }
{
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType >::UniqueParentDirtyWrapper( DataType &&Data ) noexcept :
Data{ new DataType{ std::move( Data ) } }
{
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType >::UniqueParentDirtyWrapper( DataType* *Data_ ) noexcept :
Data{ *Data_ }
{
  *Data_ = nullptr;
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType >::UniqueParentDirtyWrapper( UniqueParentDirtyWrapper &&Move ) noexcept :
Data{ Move->Data }, DirtyBools( std::move( Move->DirtyBools ) )
{
  Move->Data = nullptr;
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType >::~UniqueParentDirtyWrapper() noexcept
{
  delete Data;

  if( DirtyBools.size() )
  {
    Data = nullptr;

    SetDirtyBools();
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
UniqueParentDirtyWrapper< DataType > & UniqueParentDirtyWrapper< DataType >::operator=( const DataType &Data_ ) noexcept
{
  SetData( Data_ );

  return *this;
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType > & UniqueParentDirtyWrapper< DataType >::operator=( DataType &&Data_ ) noexcept
{
  SetData( std::move( Data_ ) );

  return *this;
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType > & UniqueParentDirtyWrapper< DataType >::operator=( DataType* *Data_ ) noexcept
{
  SetData( Data_ );

  return *this;
}

template< typename DataType >
UniqueParentDirtyWrapper< DataType > & UniqueParentDirtyWrapper< DataType >::operator=( UniqueParentDirtyWrapper &&Move ) noexcept
{
  SetData( &( Move->Data ) );

  return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
bool UniqueParentDirtyWrapper< DataType >::IsDataDirty() const noexcept
{
  return IsDirty;
}

template< typename DataType >
const DataType & UniqueParentDirtyWrapper< DataType >::GetData() noexcept
{
  IsDirty = false; 

  return *Data;
}

template< typename DataType >
const DataType & UniqueParentDirtyWrapper< DataType >::GetData() const noexcept
{
  return *Data;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template< typename DataType >
void UniqueParentDirtyWrapper< DataType >::SetData( const DataType &Data_ ) noexcept
{
  *Data = Data_;

  SetDirtyBools();
}

template< typename DataType >
void UniqueParentDirtyWrapper< DataType >::SetData( DataType &&Data_ ) noexcept
{
  *Data = std::move( Data_ );

  SetDirtyBools();
}

template< typename DataType >
void UniqueParentDirtyWrapper< DataType >::SetData( DataType* *Data_ ) noexcept
{
  delete Data;

  Data = *Data_;

  *Data_ = nullptr;

  SetDirtyBools();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
void UniqueParentDirtyWrapper< DataType >::SetDirtyBools() noexcept
{
  std::vector< bool* * > UpdatedDirtys;

  UpdatedDirtys.reserve( DirtyBools.size() );

  for( bool* *Iter : DirtyBools )
  {
    if( *Iter )
    {
      **Iter = true;

      UpdatedDirtys.push_back( Iter );
    }
  }

  DirtyBools = std::move( UpdatedDirtys );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
UniqueChildDirtyWrapper< DataType >::UniqueChildDirtyWrapper( UniqueChildDirtyWrapper &&Move ) noexcept : Data( Move.Data ), IsDirty( Move.IsDirty )
{
  Move.Data = nullptr;
  Move.IsDirty = nullptr;
}

template< typename DataType >
UniqueChildDirtyWrapper< DataType >::UniqueChildDirtyWrapper( const UniqueParentDirtyWrapper< DataType > &Parent ) noexcept :
Data( &( Parent.Data ) ), IsDirty( new bool( false ) )
{
  Parent.DirtyBools.push_back( &IsDirty );
}

template< typename DataType >
UniqueChildDirtyWrapper< DataType >::~UniqueChildDirtyWrapper() noexcept
{
  delete IsDirty;

  IsDirty = nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
UniqueChildDirtyWrapper< DataType > & UniqueChildDirtyWrapper< DataType >::operator=( UniqueChildDirtyWrapper &&Move ) noexcept
{
  Data = Move.Data;
  Move.Data = nullptr;

  delete IsDirty;
  IsDirty = Move.IsDirty;
  Move.IsDirty = nullptr;

  return *this;
}

template< typename DataType >
UniqueChildDirtyWrapper< DataType > & 
UniqueChildDirtyWrapper< DataType >::operator=( const UniqueParentDirtyWrapper< DataType > &Parent ) noexcept
{
  Data = &( Parent.Data );

  delete IsDirty;
  IsDirty = new bool( false );
  Parent.DirtyBools.push_back( &IsDirty );

  return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
const DataType & UniqueChildDirtyWrapper< DataType >::GetData() const noexcept
{
  *IsDirty = false;

  return **Data;
}

template< typename DataType >
bool UniqueChildDirtyWrapper< DataType >::GetIsDirty() const noexcept
{
  return *IsDirty;
}

template< typename DataType >
bool UniqueChildDirtyWrapper< DataType >::GetIsValidData() const noexcept
{
  return *Data;
}
