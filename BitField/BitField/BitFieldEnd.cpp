
#include "BitFieldEnd.h"

template< MaxBits Count >
BitField< 0 >::BitField( const BitField< Count > &Copy ) ExceptDebug
{
  *this = Copy;
}

template< MaxBits Count >
BitField< 0 >::BitField( BitField< Count > &&Move ) ExceptDebug
{
  *this = std::move( Move );
}

BitField< 0 >::BitField( std::initializer_list< bool > && ) noexcept
{
}

BitField< 0 >::BitField( std::initializer_list< bool > &&, MaxBits ) noexcept
{
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
BitField< 0 > & BitField< 0 >::operator|=( const BitField< Count > & ) noexcept
{
  return *this;
}

template< MaxBits Count >
BitField< 0 > & BitField< 0 >::operator&=( const BitField< Count > & ) noexcept
{
  return *this;
}

template< MaxBits Count >
BitField< 0 > & BitField< 0 >::operator^=( const BitField< Count > & ) noexcept
{
  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
constexpr BitField< 0 > BitField< 0 >::operator|( const BitField< Count > & ) const noexcept
{
  return BitField< 0 >{};
}

template< MaxBits Count >
constexpr BitField< 0 > BitField< 0 >::operator&( const BitField< Count > & ) const noexcept
{
  return BitField< 0 >{};
}

template< MaxBits Count >
constexpr BitField< 0 > BitField< 0 >::operator^( const BitField< Count > & ) const noexcept
{
  return BitField< 0 >{};
}

///////////////////////////////////////////////////////////////////////////////

BitField< 0 > & BitField<0>::operator~() noexcept
{
  return *this;
}

constexpr BitField< 0 > BitField< 0 >::operator!() const noexcept
{
  return BitField< 0 >{};
}

///////////////////////////////////////////////////////////////////////////////

std::ostream & operator<<( std::ostream &os, const BitField< 0 > & ) noexcept
{
  return os;
}

constexpr BitField< 0 > BitField< 0 >::operator<<( MaxBits ) const noexcept
{
  return BitField< 0 >{};
}

constexpr BitField< 0 > BitField< 0 >::operator>>( MaxBits ) const noexcept
{
  return BitField< 0 >{};
}


///////////////////////////////////////////////////////////////////////////////

BitField< 0 > & BitField< 0 >::operator<<=( MaxBits ) noexcept
{
  return *this;
}

BitField< 0 > & BitField< 0 >::operator>>=( MaxBits ) noexcept
{
  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
const BitField< 0 + Count > BitField< 0 >::operator+( const BitField< Count > &Rhs ) const noexcept
{
  return BitField< 0 + Count >{ Rhs };
}

template< MaxBits Count >
const BitField< 0 + Count > BitField< 0 >::operator+( BitField< Count > &&Rhs ) const noexcept
{
  return BitField< 0 + Count >{ std::move( Rhs ) };
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
BitField< 0 > & BitField< 0 >::operator=( const BitField< Count > & ) ExceptDebug
{
  AssertDebug( Count > 0, BitFieldTooLarge( Count ) );

  return *this;
}

template< MaxBits Count >
BitField< 0 > & BitField< 0 >::operator=( BitField< Count > && ) ExceptDebug
{
  AssertDebug( Count > 0, BitFieldTooLarge( Count ) );

  return *this;
}

BitField< 0 > & BitField< 0 >::operator=( std::initializer_list< bool > && ) noexcept
{
  return *this;
}

///////////////////////////////////////////////////////////////////////////////

void BitField< 0 >::Set( MaxBits DebugOnly( Index ) ) ExceptDebug
{
  AssertDebug( Index, IndexTooLarge( Index ) );
}

void BitField< 0 >::Set( MaxBits DebugOnly( Index ), bool ) ExceptDebug
{
  AssertDebug( Index, IndexTooLarge( Index ) );
}

constexpr bool BitField< 0 >::Get( MaxBits DebugOnly( Index ) ) ExceptDebug
{
  AssertDebug( Index, IndexTooLarge( Index ) );

  return false;
}

void BitField< 0 >::Clear( MaxBits DebugOnly( Index ) ) ExceptDebug
{
  AssertDebug( Index, IndexTooLarge( Index ) );
}

void BitField< 0 >::Negate( MaxBits DebugOnly( Index ) ) ExceptDebug
{
  AssertDebug( Index, IndexTooLarge( Index ) );
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Start, MaxBits End >
constexpr BitField< 1 > BitField< 0 >::Get() ExceptDebug
{
  AssertDebug( Start > 0, IndexTooLarge( Start ) );
  AssertDebug( End   > 0, IndexTooLarge( End   ) );

  return BitField< 1 >{};
}

const BitField< 1 > BitField< 0 >::Get( MaxBits DebugOnly( Start ), MaxBits DebugOnly( End ) ) ExceptDebug
{
  AssertDebug( Start > 0, IndexTooLarge( Start ) );
  AssertDebug( End   > 0, IndexTooLarge( End ) );

  return BitField< 1 >{};
}

///////////////////////////////////////////////////////////////////////////////

constexpr ushort BitField< 0 >::Sizeof() noexcept
{
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

constexpr ushort BitField< 0 >::Number() noexcept
{ 
  return 0;
}

const std::vector< ushort > BitField< 0 >::Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) ExceptDebug
{
  std::vector< ushort > Return{};

  Return.resize( Intervals.size() );

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

BitField< 0 >::IndexTooLarge::IndexTooLarge( MaxBits Index ) noexcept
{
  std::cerr << "Tried to Set/Get/Clear Index: " << Index << ", but the BitField has " << 0 << " bits!" << std::endl;
}

BitField< 0 >::BitFieldTooLarge::BitFieldTooLarge( MaxBits Size ) noexcept
{
  std::cerr << "Tried to Copy/Move a BitField with size: " << Size << ", but the resulting BitField can only have " << 0;
  std::cerr << " bits!" << std::endl;
}
