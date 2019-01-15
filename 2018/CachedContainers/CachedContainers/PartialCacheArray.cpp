/*!------------------------------------------------------------------------------
\file   PartialCacheArray.cpp

\author Garrett Conti

\par    Project: CachedContainers
\par    Course:  Fun
   ------------------------------------------------------------------------------ */

#include "PartialCacheArray.h"

#include <algorithm>

template< typename CacheAmountFunctor >
PartialCacheArray< CacheAmountFunctor >::PartialCacheArray( const size_t ReserveSize ) noexcept : DataArray( new int[ ReserveSize ] ), Size( 0 )
{
}

template< typename CacheAmountFunctor >
void PartialCacheArray< CacheAmountFunctor >::Emplace( const int Data ) noexcept
{
  DataArray[ Size ] = Data;

  ++Size;
}

template< typename CacheAmountFunctor >
void PartialCacheArray< CacheAmountFunctor >::Find( const int Data ) noexcept
{
  for( int *FoundIter = std::find( DataArray, DataArray + Size, Data ), *const End = FoundIter - CacheAmount( static_cast< size_t >( FoundIter - DataArray ) );
       FoundIter != End; --FoundIter )
  {
    const int Temp = *FoundIter;

    int *const Prev = FoundIter - 1;

    *FoundIter = *Prev;

    *Prev = Temp;
  }
}

template< typename CacheAmountFunctor >
void PartialCacheArray< CacheAmountFunctor >::Remove( const int Data ) noexcept
{
  *std::find( DataArray, DataArray + Size, Data ) = *( DataArray + Size - 1 );

  --Size;
}
