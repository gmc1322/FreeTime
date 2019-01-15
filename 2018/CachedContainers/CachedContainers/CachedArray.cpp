/*!------------------------------------------------------------------------------
\file   CachedArray.cpp

\author Garrett Conti

\par    Project: CachedContainers
\par    Course:  FUN
   ------------------------------------------------------------------------------ */

#include "CachedArray.h"

#include <algorithm>

CachedArray::CachedArray( const size_t ReserveSize ) noexcept : DataArray( new int[ ReserveSize ] ), Size( 0 ), Capacity( ReserveSize )
{
}

void CachedArray::Emplace( const int Data ) noexcept
{
  DataArray[ Size ] = Data;

  ++Size;
}

void CachedArray::Find( const int Data ) noexcept
{
  for( int *FoundIter = std::find( DataArray, DataArray + Size, Data ); FoundIter != DataArray; --FoundIter )
  {
    const int Temp = *FoundIter;

    int *const Prev = FoundIter - 1;

    *FoundIter = *Prev;

    *Prev = Temp;
  }
}

void CachedArray::Remove( const int Data ) noexcept
{
  *std::find( DataArray, DataArray + Size, Data ) = *( DataArray + Size - 1 );

  --Size;
}
