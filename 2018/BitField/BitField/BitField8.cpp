
#include "BitField8.h"

template< MaxBits Count >
BitField< uchar_BitCount >::BitField( const BitField< Count > &Copy ) ExceptDebug
{
  *this = Copy;
}

template< MaxBits Count >
BitField< uchar_BitCount >::BitField( BitField< Count > &&Move ) ExceptDebug
{
  *this = std::move( Move );
}

BitField< uchar_BitCount >::BitField( std::initializer_list< bool > &&Bits_ ) noexcept
{
  *this = std::move( Bits_ );
}

BitField< uchar_BitCount >::BitField( std::initializer_list< bool > &&Bits_, MaxBits Start ) noexcept
{
  if( Start < ushort_BitCount )
  {
    *this = std::move( Bits_ );
  }
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator^=( const BitField< Count > &Rhs ) noexcept
{
  Bits ^= Rhs.Number();

  return *this;
}

template< MaxBits Count >
BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator&=( const BitField< Count > &Rhs ) noexcept
{
  Bits &= Rhs.Number();

  return *this;
}

template< MaxBits Count >
BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator|=( const BitField< Count > &Rhs ) noexcept
{
  Bits |= Rhs.Number();

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
const BitField< uchar_BitCount > BitField< uchar_BitCount >::operator^( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return ^= Rhs;

  return Return; // Named RVO
}

template< MaxBits Count >
const BitField< uchar_BitCount > BitField< uchar_BitCount >::operator&( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return &= Rhs;

  return Return; // Named RVO
}

template< MaxBits Count >
const BitField< uchar_BitCount > BitField< uchar_BitCount >::operator|( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return |= Rhs;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator~() noexcept
{
  Bits = ~Bits;

  return *this;
}

const BitField< uchar_BitCount > BitField< uchar_BitCount >::operator!() const noexcept
{
  BitField Return( *this );

  ~Return;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

std::ostream & operator<<( std::ostream &os, const BitField< uchar_BitCount > &Rhs ) noexcept
{
  for( uchar i = 0; i < uchar_BitCount; ++i )
  {
    os << Rhs.Get( i );
  }

  return os;
}

const BitField< uchar_BitCount > BitField< uchar_BitCount >::operator<<( MaxBits Shift ) const noexcept
{
  BitField< uchar_BitCount > Return( *this );

  Return.Bits <<= Shift;

  return Return; // Named RVO
}

const BitField< uchar_BitCount > BitField< uchar_BitCount >::operator>>( MaxBits Shift ) const noexcept
{
  BitField< uchar_BitCount > Return( *this );

  Return.Bits >>= Shift;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator<<=( MaxBits Shift ) noexcept
{
  Bits <<= Shift;

  return *this;
}

BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator>>=( MaxBits Shift ) noexcept
{
  Bits >>= Shift;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
const BitField< uchar_BitCount + Count > BitField< uchar_BitCount >::operator+( const BitField< Count > &Rhs ) const noexcept
{
  BitField< uchar_BitCount + Count > Return{ Rhs };

  Return <<= Count;

  Return |= *this;

  return Return; // Named RVO
}

template< MaxBits Count >
const BitField< uchar_BitCount + Count > BitField< uchar_BitCount >::operator+( BitField< Count > &&Rhs ) const noexcept
{
  BitField< uchar_BitCount + Count > Return{ std::move( Rhs ) };

  Return <<= Count;

  Return |= *this;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator=( const BitField< Count > &Rhs ) ExceptDebug
{
  AssertDebug( Count > uchar_BitCount, BitFieldTooLarge( Count ) );

  Bits = Rhs.Number();

  return *this;
}

template< MaxBits Count >
BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator=( BitField< Count > &&Rhs ) ExceptDebug
{
  AssertDebug( Count > uchar_BitCount, BitFieldTooLarge( Count ) );

  Bits = Rhs.Number();

  return *this;
}

BitField< uchar_BitCount > & BitField< uchar_BitCount >::operator=( std::initializer_list< bool > &&Bits_ ) noexcept
{
  const size_t Size = Bits_.size();

  for( size_t i = 0, End = ( Size < ullong_BitCount ? Size : ullong_BitCount ); i < End; ++i )
  {
    Bits |= ( *( Bits_.begin() + i ) << i );
  }

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

void BitField< uchar_BitCount >::Set( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > uchar_BitCount, IndexTooLarge( Index ) );

  Bits |= ( 1 << Index );
}

void BitField< uchar_BitCount >::Set( MaxBits Index, bool Bit ) ExceptDebug
{
  AssertDebug( Index > uchar_BitCount, IndexTooLarge( Index ) );

  Bits ^= ( -static_cast< int >( Bit ) ^ Bits ) & ( 1 << Index );
}

bool BitField< uchar_BitCount >::Get( MaxBits Index ) const ExceptDebug
{
  AssertDebug( Index > uchar_BitCount, IndexTooLarge( Index ) );

  return Bits & ( 1 << Index );
}

void BitField< uchar_BitCount >::Clear( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > uchar_BitCount, IndexTooLarge( Index ) );

  Bits &= ~( 1 << Index );
}

void BitField< uchar_BitCount >::Negate( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > uchar_BitCount, IndexTooLarge( Index ) );

  Bits ^= 1 << Index;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Start, MaxBits End >
constexpr BitField< End - Start + 1 > BitField< uchar_BitCount >::Get() const ExceptDebug
{
  BitField< End - Start + 1 > Return;

  for( auto i = Start; i <= End; ++i )
  {
    Return.Set( i - Start, Get( i ) );
  }

  return Return; // Named RVO
}

const BitField< uchar_BitCount > BitField< uchar_BitCount >::Get( MaxBits Start, MaxBits End ) const ExceptDebug
{
  BitField< uchar_BitCount > Return;

  for( auto i = Start; i <= End; ++i )
  {
    Return.Set( i - Start, Get( i ) );
  }

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

ushort BitField< uchar_BitCount >::Number() const noexcept
{
  return Bits;
}

const std::vector< ushort > BitField< uchar_BitCount >::Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) const ExceptDebug
{
  std::vector< ushort > Return{};

  Return.reserve( Intervals.size() );

  for( auto Iter : Intervals )
  {
    Return.push_back( Get( static_cast< MaxBits >( Iter.first ), static_cast< MaxBits >( Iter.second ) ).Number() );
  }

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

constexpr ushort BitField< uchar_BitCount >::Sizeof() noexcept
{
  return uchar_BitCount;
}

///////////////////////////////////////////////////////////////////////////////

BitField< uchar_BitCount >::IndexTooLarge::IndexTooLarge( MaxBits Index ) noexcept
{
  std::cerr << "Tried to Set/Get/Clear Index: " << Index << ", but the BitField only has " << uchar_BitCount << " bits!" << std::endl;
}

BitField< uchar_BitCount >::BitFieldTooLarge::BitFieldTooLarge( MaxBits Size ) noexcept
{
  std::cerr << "Tried to Copy/Move a BitField with size: " << Size << ", but the resulting BitField can only have " << uchar_BitCount;
  std::cerr << " bits!" << std::endl;
}
