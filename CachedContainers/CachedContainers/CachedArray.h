/*!------------------------------------------------------------------------------
\file   CachedArray.h

\author Garrett Conti

\par    Project: CachedContainers
\par    Course:  FUN
   ------------------------------------------------------------------------------ */

#pragma once

class CachedArray
{
  public:
    explicit CachedArray( size_t ReserveSize ) noexcept;

  public:
    void Emplace( int Data ) noexcept;
    void Find( int Data ) noexcept;
    void Remove( int Data ) noexcept;

  private:
    int *DataArray;
    size_t Size;
    size_t Capacity;
};
