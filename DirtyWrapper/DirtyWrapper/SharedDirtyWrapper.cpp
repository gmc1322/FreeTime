/*!------------------------------------------------------------------------------
\file   SharedDirtyWrapper.cpp

\author Garrett Conti

\par    Project: SharedDirtyWrapper
\par    Course:  
   ------------------------------------------------------------------------------ */

#include "SharedDirtyWrapper.h"

#include <algorithm>

template< typename DataType >
SharedDirtyWrapper< DataType >::SharedDirtyWrapper( const DataType &Data ) noexcept : Data( new DataType{ Data } )
{
}

template< typename DataType >
SharedDirtyWrapper< DataType >::SharedDirtyWrapper( DataType &&Data ) noexcept : Data( new DataType{ std::move( Data ) } )
{
}

template< typename DataType >
SharedDirtyWrapper< DataType >::SharedDirtyWrapper( DataType* *Data_ ) noexcept : Data( *Data_ )
{
  *Data_ = nullptr;
}

template< typename DataType >
SharedDirtyWrapper< DataType >::SharedDirtyWrapper( SharedDirtyWrapper &Copy ) noexcept : 
Data( Copy->Data ), IsDirty( Copy->IsDirty ), References( Copy->References )
{
  CleanReferences();

  References.push_back( Copy );

  AddReferences();
}

template< typename DataType >
SharedDirtyWrapper< DataType >::SharedDirtyWrapper( SharedDirtyWrapper &&Move ) noexcept :
Data( Move.Data ), IsDirty( Move.IsDirty ), References( std::move( Move.References ) )
{
  Move.Data = nullptr;

  FindCleanAndReplace( &Move, this );
}

template< typename DataType >
SharedDirtyWrapper< DataType >::~SharedDirtyWrapper() noexcept
{
  // Erase all lingering nulls
  // Remove our selves from everyone referancing us
  FindCleanAndReplace( this, nullptr );

  if( References.empty() )
  {
    delete Data;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
SharedDirtyWrapper< DataType > & SharedDirtyWrapper< DataType >::operator=( const DataType &Data_ ) noexcept
{
  SetData( Data_ );

  return *this;
}

template< typename DataType >
SharedDirtyWrapper< DataType > & SharedDirtyWrapper< DataType >::operator=( DataType &&Data_ ) noexcept
{
  SetData( std::move( Data_ ) );

  return *this;
}

template< typename DataType >
SharedDirtyWrapper< DataType > & SharedDirtyWrapper< DataType >::operator=( DataType* *const Data_ ) noexcept
{
  SetData( Data_ );

  return *this;
}

template< typename DataType >
SharedDirtyWrapper< DataType > & SharedDirtyWrapper< DataType >::operator=( const SharedDirtyWrapper &Copy ) noexcept
{
  FindAndReplace( this, nullptr );

  Data = Copy->Data;

  Copy->CleanReferences();

  References = Copy->References;

  References.push_back( Copy );

  AddReferences();

  IsDirty = true;

  return *this;
}

template< typename DataType >
SharedDirtyWrapper< DataType > & SharedDirtyWrapper< DataType >::operator=( SharedDirtyWrapper &&Move ) noexcept
{
  FindAndReplace( this, nullptr );
  
  Data = Move.Data;

  Move.Data = nullptr;

  References = std::move( Move.References );

  FindCleanAndReplace( &Move, this );

  IsDirty = true;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
bool SharedDirtyWrapper< DataType >::IsDataDirty() const noexcept
{
  return IsDirty;
}

template< typename DataType >
const DataType & SharedDirtyWrapper< DataType >::GetData() noexcept
{
  IsDirty = false;

  return *Data;
}

template< typename DataType >
const DataType & SharedDirtyWrapper< DataType >::GetData() const noexcept
{
  return *Data;
}

template< typename DataType >
size_t SharedDirtyWrapper< DataType >::GetReferenceCount() const noexcept
{
  CleanReferences();

  return References.size();
}

template< typename DataType >
void SharedDirtyWrapper< DataType >::SetData( const DataType &Data_ ) noexcept
{
  *Data = Data_;

  SetReferencesDirty();
}

template< typename DataType >
void SharedDirtyWrapper< DataType >::SetData( DataType &&Data_ ) noexcept
{
  *Data = std::move( Data_ );

  SetReferencesDirty();
}

template< typename DataType >
void SharedDirtyWrapper< DataType >::SetData( DataType* *const Data_ ) noexcept
{
  delete Data;

  Data = *Data_;

  *Data_ = nullptr;

  SetReferencesDirty( Data );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename DataType >
void SharedDirtyWrapper< DataType >::FindAndReplace( SharedDirtyWrapper *const CheckPtr, SharedDirtyWrapper *const ReplacePtr ) noexcept
{
  for( SharedDirtyWrapper *const OuterIter : References )
  {
    for( auto InnerIter = OuterIter->References.begin(); ; ++InnerIter )
    {
      if( *InnerIter == CheckPtr )
      {
        *InnerIter = ReplacePtr;

        break;
      }
    }
  }
}

template < typename DataType >
void SharedDirtyWrapper< DataType >::FindCleanAndReplace( SharedDirtyWrapper *const CheckPtr, SharedDirtyWrapper *const ReplacePtr ) noexcept
{
  for( size_t i = 0, End = References.size(); i < End; )
  {
    SharedDirtyWrapper* &Temp = References[ i ];

    if( Temp )
    {
      for( auto InnerIter = Temp->References.begin(); ; ++InnerIter )
      {
        if( *InnerIter == CheckPtr )
        {
          *InnerIter = ReplacePtr;

          break;
        }
      }

      ++i;
    }
    else
    {
      Temp = References.back();

      References.pop_back();

      --End;
    }
  }
}

template< typename DataType >
void SharedDirtyWrapper< DataType >::SetReferencesDirty() noexcept
{
  IsDirty = true;

  for( size_t i = 0, End = References.size(); i < End; )
  {
    SharedDirtyWrapper* &Temp = References[ i ];

    if( Temp )
    {
      Temp->IsDirty = true;

      ++i;
    }
    else // Clean-up null
    {
      Temp = References.back();

      References.pop_back();

      --End;
    }
  }
}

template< typename DataType > 
void SharedDirtyWrapper< DataType >::SetReferencesDirty( DataType *const NewDataPtr ) noexcept
{
  IsDirty = true;

  for( size_t i = 0, End = References.size(); i < End; )
  {
    SharedDirtyWrapper* &Temp = References[ i ];

    if( Temp )
    {
      // Don't need to delete TempPtr->Data since it was deleted before the function was called

      Temp->Data = NewDataPtr;

      Temp->IsDirty = true;

      ++i;
    }
    else // Clean-up null
    {
      Temp = References.back();

      References.pop_back();

      --End;
    }
  }
}

template< typename DataType >
void SharedDirtyWrapper< DataType >::CleanReferences() const noexcept
{
  References.erase( std::remove( References.begin(), References.end(), nullptr ), References.end() );
}

template< typename DataType >
void SharedDirtyWrapper< DataType >::AddReferences() const noexcept
{
  for( SharedDirtyWrapper *Iter : References )
  {
    Iter->References.push_back( this );
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
