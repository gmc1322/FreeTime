#pragma once

class BitOutput
{
  public:
  enum OutputFormat
  {
    Binary = 1,
    Octal = 3,
    Hex = 4,
  };

  public:
  BitOutput( OutputFormat FormatType ) noexcept;
  explicit BitOutput( MaxBits NumberOfBitsPerSet ) noexcept;

  public:
  friend BitOutput && operator<<( std::ostream &os, BitOutput &&Rhs ) noexcept;

  template< MaxBits N >
  std::ostream & operator<<( const BitField< N > &Rhs ) noexcept;

  private:
  MaxBits BitCount;

  std::ostream *os = nullptr;
};

std::ostream & operator<<( std::ostream &os, const std::vector< std::vector< ullong > > &Rhs ) noexcept;
std::ostream & operator<<( std::ostream &os, const std::vector< ullong > &Rhs ) noexcept;
std::ostream & operator<<( std::ostream &os, const std::vector< ulong > &Rhs ) noexcept;
std::ostream & operator<<( std::ostream &os, const std::vector< ushort > &Rhs ) noexcept;

#include "BitOutput.cpp"
