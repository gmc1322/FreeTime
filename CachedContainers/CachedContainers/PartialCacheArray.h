/*!------------------------------------------------------------------------------
\file   PartialCacheArray.h

\author Garrett Conti

\par    Project: CachedContainers
\par    Course:  Fun
   ------------------------------------------------------------------------------ */

#pragma once

template< typename CacheAmountFunctor >
class PartialCacheArray
{
  public:
    explicit PartialCacheArray( size_t ReserveSize  ) noexcept;

  public:
    void Emplace( int Data ) noexcept;
    void Find( int Data ) noexcept;
    void Remove( int Data ) noexcept;

  private:
    static constexpr CacheAmountFunctor CacheAmount{};

  private:
    int *DataArray;
    size_t Size;
};

#include "PartialCacheArray.cpp"
