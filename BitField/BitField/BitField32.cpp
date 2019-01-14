
#include "BitField32.h"

template< MaxBits Count >
BitField< ulong_BitCount >::BitField( const BitField< Count > &Copy ) ExceptDebug
{
  *this = Copy;
}

template< MaxBits Count >
BitField< ulong_BitCount >::BitField( BitField< Count > &&Move ) ExceptDebug
{
  *this = std::move( Move );
}

BitField< ulong_BitCount >::BitField( std::initializer_list< bool > &&Bits_ ) noexcept
{
  *this = std::move( Bits_ );
}

BitField< ulong_BitCount >::BitField( std::initializer_list< bool > &&Bits_, MaxBits Start ) noexcept
{
  if( Start < ulong_BitCount )
  {
    *this = std::move( Bits_ );
  }
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator^=( const BitField< Count > &Rhs ) noexcept
{
  Bits ^= Rhs.Number();

  return *this;
}

template< MaxBits Count >
BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator&=( const BitField< Count > &Rhs ) noexcept
{
  Bits &= Rhs.Number();

  return *this;
}

template< MaxBits Count >
BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator|=( const BitField< Count > &Rhs ) noexcept
{
  Bits |= Rhs.Number();

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
const BitField< ulong_BitCount > BitField< ulong_BitCount >::operator^( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return ^= Rhs;

  return Return; // Named RVO
}

template< MaxBits Count >
const BitField< ulong_BitCount > BitField< ulong_BitCount >::operator&( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return &= Rhs;

  return Return; // Named RVO
}

template< MaxBits Count >
const BitField< ulong_BitCount > BitField< ulong_BitCount >::operator|( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return |= Rhs;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator~() noexcept
{
  Bits = ~Bits;

  return *this;
}

const BitField< ulong_BitCount > BitField< ulong_BitCount >::operator!() const noexcept
{
  BitField Return( *this );

  ~Return;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

std::ostream & operator<<( std::ostream &os, const BitField< ulong_BitCount > &Rhs ) noexcept
{
  for( uchar i = 0; i < ulong_BitCount; ++i )
  {
    os << Rhs.Get( i );
  }

  return os;
}

const BitField< ulong_BitCount > BitField< ulong_BitCount >::operator<<( MaxBits Shift ) const noexcept
{
  BitField< ulong_BitCount > Return( *this );

  Return.Bits <<= Shift;

  return Return; // Named RVO
}

const BitField< ulong_BitCount > BitField< ulong_BitCount >::operator>>( MaxBits Shift ) const noexcept
{
  BitField< ulong_BitCount > Return( *this );

  Return.Bits >>= Shift;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator<<=( MaxBits Shift ) noexcept
{
  Bits <<= Shift;

  return *this;
}

BitField< ulong_BitCount >& BitField< ulong_BitCount >::operator>>=( MaxBits Shift ) noexcept
{
  Bits >>= Shift;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
const BitField< ulong_BitCount + Count > BitField< ulong_BitCount >::operator+( const BitField< Count > &Rhs ) const noexcept
{
  BitField< ulong_BitCount + Count > Return{ Rhs };

  Return <<= Count;

  Return |= *this;

  return Return; // Named RVO
}

template< MaxBits Count >
const BitField< ulong_BitCount + Count > BitField< ulong_BitCount >::operator+( BitField< Count > &&Rhs ) const noexcept
{
  BitField< ulong_BitCount + Count > Return{ std::move( Rhs ) };

  Return <<= Count;

  Return |= *this;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Count >
BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator=( const BitField< Count > &Rhs ) ExceptDebug
{
  AssertDebug( Count > ulong_BitCount, BitFieldTooLarge( Count ) );

  Bits = Rhs.Number();

  return *this;
}

template< MaxBits Count >
BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator=( BitField< Count > &&Rhs ) ExceptDebug
{
  AssertDebug( Count > ulong_BitCount, BitFieldTooLarge( Count ) );

  Bits = Rhs.Number();

  return *this;
}

BitField< ulong_BitCount > & BitField< ulong_BitCount >::operator=( std::initializer_list< bool > &&Bits_ ) noexcept
{
  const size_t Size = Bits_.size();

  for( size_t i = 0, End = ( Size < ulong_BitCount ? Size : ulong_BitCount ); i < End; ++i )
  {
    Bits |= ( *( Bits_.begin() + i ) << i );
  }

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

void BitField< ulong_BitCount >::Set( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > ulong_BitCount, IndexTooLarge( Index ) );

  Bits |= 1 << Index;
}

void BitField< ulong_BitCount >::Set( MaxBits Index, bool Bit ) ExceptDebug
{
  AssertDebug( Index > ulong_BitCount, IndexTooLarge( Index ) );

  Bits ^= ( -static_cast< int >( Bit ) ^ Bits ) & ( 1 << Index );
}

bool BitField< ulong_BitCount >::Get( MaxBits Index ) const ExceptDebug
{
  AssertDebug( Index > ulong_BitCount, IndexTooLarge( Index ) );

  return Bits & ( 1 << Index );
}

void BitField< ulong_BitCount >::Clear( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > ulong_BitCount, IndexTooLarge( Index ) );

  Bits &= ~( 1 << Index );
}

void BitField< ulong_BitCount >::Negate( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > ushort_BitCount, IndexTooLarge( Index ) );

  Bits ^= 1 << Index;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits Start, MaxBits End >
constexpr BitField< End - Start + 1 > BitField< ulong_BitCount >::Get() const ExceptDebug
{
  BitField< End - Start + 1 > Return;

  for( auto i = Start; i <= End; ++i )
  {
    Return.Set( i - Start, Get( i ) );
  }

  return Return; // Named RVO
}

const BitField< ulong_BitCount > BitField< ulong_BitCount >::Get( MaxBits Start, MaxBits End ) const ExceptDebug
{
  BitField< ulong_BitCount > Return;

  for( auto i = Start; i <= End; ++i )
  {
    Return.Set( i - Start, Get( i ) );
  }

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

ulong BitField< ulong_BitCount >::Number() const noexcept
{
  return Bits;
}

const std::vector< ulong > BitField< ulong_BitCount >::Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) const ExceptDebug
{
  std::vector< ulong > Return{};

  Return.reserve( Intervals.size() );

  for( auto Iter : Intervals )
  {
    Return.push_back( Get( static_cast< MaxBits >( Iter.first ), static_cast< MaxBits >( Iter.second ) ).Number() );
  }

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

constexpr ushort BitField< ulong_BitCount >::Sizeof() noexcept
{
  return ulong_BitCount;
}

///////////////////////////////////////////////////////////////////////////////

BitField< ulong_BitCount >::IndexTooLarge::IndexTooLarge( MaxBits Index ) noexcept
{
  std::cerr << "Tried to Set/Get/Clear Index: " << Index << ", but the BitField only has " << ulong_BitCount << " bits!" << std::endl;
}

BitField< ulong_BitCount >::BitFieldTooLarge::BitFieldTooLarge( MaxBits Size ) noexcept
{
  std::cerr << "Tried to Copy/Move a BitField with size: " << Size << ", but the resulting BitField can only have " << ulong_BitCount;
  std::cerr << " bits!" << std::endl;
}
