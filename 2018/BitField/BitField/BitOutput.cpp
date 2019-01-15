
#include "BitOutput.h"

BitOutput::BitOutput( OutputFormat FormatType ) noexcept : BitCount( static_cast< MaxBits >( FormatType ) )
{
}

BitOutput::BitOutput( MaxBits NumberOfBitsPerSet ) noexcept : BitCount( NumberOfBitsPerSet )
{
}

BitOutput && operator<<( std::ostream &os, BitOutput &&Rhs ) noexcept
{
  Rhs.os = &os;

  return std::move( Rhs );
}

template< MaxBits N >
std::ostream & BitOutput::operator<<( const BitField< N > &Rhs ) noexcept
{
  MaxBits i = 0;
  for( ; i < std::ceil( ( N / static_cast< float >( BitCount ) ) - 1 ); ++i )
  {
    for( MaxBits j = 0; j < BitCount; ++j )
    {
      *os << Rhs.Get( i * BitCount + j );
    }

    *os << "-";
  }

  for( i *= BitCount; i < N; ++i )
  {
    *os << Rhs.Get( i );
  }

  return *os;
}

std::ostream & operator<<( std::ostream &os, const std::vector< std::vector< ullong > > &Rhs ) noexcept
{
  for( auto OuterIter : Rhs )
  {
    os << "{ ";

    for( auto InnerIter : OuterIter )
    {
      os << InnerIter << " ";
    }

    os << "}";
  }

  return os;
}

std::ostream & operator<<( std::ostream &os, const std::vector< ullong > &Rhs ) noexcept
{
  os << "{ ";

  for( auto Iter : Rhs )
  {
    os << Iter << " ";
  }

  os << "}";

  return os;
}

std::ostream & operator<<( std::ostream &os, const std::vector< ulong > &Rhs ) noexcept
{
  os << "{ ";

  for( auto Iter : Rhs )
  {
    os << Iter << " ";
  }

  os << "}";

  return os;
}

std::ostream & operator<<( std::ostream &os, const std::vector< ushort > &Rhs ) noexcept
{
  os << "{ ";

  for( auto Iter : Rhs )
  {
    os << Iter << " ";
  }

  os << "}";

  return os;
}
