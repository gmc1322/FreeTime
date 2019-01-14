
#include "BitField.h"

template< MaxBits N >
template< MaxBits Count >
BitField< N >::BitField( const BitField< Count > &Copy ) ExceptDebug
{
  AssertDebug( Count > N, BitFieldTooLarge( Count ) );

  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits = Copy.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest = Copy.template Get< End, Count - 1 >() ) : ( Rest = BitField< 0 >{} );
}

template< MaxBits N >
template< MaxBits Count >
BitField< N >::BitField( BitField< Count > &&Move ) ExceptDebug
{
  AssertDebug( Count > N, BitFieldTooLarge( Count ) );

  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits = Move.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest = Move.template Get< End, Count - 1 >() ) : ( Rest = BitField< 0 >{} );
}

template< MaxBits N >
BitField< N >::BitField( std::initializer_list< bool > &&Bits_ ) noexcept : Bits( std::move( Bits_ ) ),
                                                                            Rest( std::move( Bits_ ), BitCount )
{
}

template< MaxBits N >
BitField< N >::BitField( std::initializer_list< bool > &&Bits_, MaxBits Start ) noexcept : Bits( std::move( Bits_ ), Start ),
                                                                                           Rest( std::move( Bits_ ), Start + BitCount )
{
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
template< MaxBits Count >
BitField< N > & BitField< N >::operator^=( const BitField< Count > &Rhs ) noexcept
{
  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits ^= Rhs.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest ^= Rhs.template Get< End, Count - 1 >() ) : Rest;

  return *this;
}

template< MaxBits N >
template< MaxBits Count >
BitField< N > & BitField< N >::operator&=( const BitField< Count > &Rhs ) noexcept
{
  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits &= Rhs.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest &= Rhs.template Get< End, Count - 1 >() ) : Rest;

  return *this;
}

template< MaxBits N >
template< MaxBits Count >
BitField< N > & BitField< N >::operator|=( const BitField< Count > &Rhs ) noexcept
{
  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits |= Rhs.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest |= Rhs.template Get< End, Count - 1 >() ) : Rest;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
template< MaxBits Count >
const BitField< N > BitField< N >::operator^( const BitField< Count > &Rhs ) const noexcept
{
  AssertDebug( Count > N, BitFieldTooLarge( Count ) );

  BitField Return( *this );
  
  Return ^= Rhs;

  return Return; // Named RVO
}

template< MaxBits N >
template< MaxBits Count >
const BitField< N > BitField< N >::operator&( const BitField< Count > &Rhs ) const noexcept
{
  AssertDebug( Count > N, BitFieldTooLarge( Count ) );

  BitField Return( *this );

  Return &= Rhs;

  return Return; // Named RVO
}

template< MaxBits N >
template< MaxBits Count >
const BitField< N > BitField< N >::operator|( const BitField< Count > &Rhs ) const noexcept
{
  BitField Return( *this );

  Return |= Rhs;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
BitField< N > & BitField< N >::operator~() noexcept
{
  Bits = ~Bits;
  Rest = ~Rest;

  return *this;
}

template< MaxBits N >
const BitField< N > BitField< N >::operator!() const noexcept
{
  BitField Return( *this );

  ~Return;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
std::ostream & operator<<( std::ostream &os, const BitField< N > &Rhs ) noexcept
{ 
  return ( N < BitField< N >::BitCount ) ? Rhs.Print1( os ) : Rhs.Print2( os );
}

template< MaxBits N >
const BitField< N > BitField< N >::operator<<( MaxBits Shift ) const noexcept
{
  BitField< N > Return( *this );

  MaxBits Index = Bits.BitCount;

  if( Bits.BitCount > Shift )
  {
    Return.Bits <<= Shift;

    Index -= Shift;
  }
  else
  {
    Return.Bits <<= Bits.BitCount;

    Return.Rest <<= Shift - Bits.BitCount;

    Index -= Bits.BitCount;
  }

  const MaxBits RestLeft = N - Bits.BitCount;
  const MaxBits Bitsleft = Bits.BitCount - Index;

  for( MaxBits i = 0, End = ( Bitsleft > RestLeft ? RestLeft : Bitsleft ); i != End; ++i, ++Index )
  {
    Return.Rest.Set( i, Bits.Get( Index ) );
  }

  return Return; // Named RVO
}

template< MaxBits N >
const BitField< N > BitField< N >::operator>>( MaxBits Shift ) const noexcept
{
  BitField< N > Return( *this );

  Return.Bits >>= Return.Bits.BitCount < Shift ? Return.Bits.BitCount : Shift;
  Return.Rest >>= Return.Bits.BitCount < Shift ? Shift - Return.Bits.BitCount : 0;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
BitField< N > & BitField< N >::operator<<=( MaxBits Shift ) noexcept
{
  const MaxBits Index = Bits.BitCount < Shift ? Bits.BitCount : Shift;

  auto Temp = Bits.Get( Bits.BitCount - Index, Bits.BitCount );

  Bits <<= Index;

  Rest <<= Bits.BitCount < Shift ? Shift - Bits.BitCount : 0;

  Rest |= Temp;

  return *this;
}

template< MaxBits N >
BitField< N >& BitField< N >::operator>>=( MaxBits Shift ) noexcept
{
  Bits >>= Bits.BitCount < Shift ? Bits.BitCount : Shift;
  Rest >>= Bits.BitCount < Shift ? Shift - Bits.BitCount : 0;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
template< MaxBits Count >
const BitField< N + Count > BitField< N >::operator+( const BitField< Count > &Rhs ) const noexcept
{
  BitField< N + Count > Return{ Rhs };

  Return <<= N;

  Return |= *this;

  return Return; // Named RVO
}

template< MaxBits N >
template< MaxBits Count >
const BitField< N + Count > BitField< N >::operator+( BitField< Count > &&Rhs ) const noexcept
{
  BitField< N + Count > Return{ std::move( Rhs ) };

  Return <<= Count;

  Return |= *this;

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
template< MaxBits Count >
BitField< N > & BitField< N >::operator=( const BitField< Count > &Rhs ) ExceptDebug
{
  AssertDebug( Count > N, BitFieldTooLarge( Count ) );

  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits = Rhs.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest = Rhs.template Get< End, Count - 1 >() ) : ( Rest = BitField< 0 >{} );

  return *this;
}

template< MaxBits N >
template< MaxBits Count >
BitField< N > & BitField< N >::operator=( BitField< Count > &&Rhs ) ExceptDebug
{
  AssertDebug( Count > N, BitFieldTooLarge( Count ) );

  constexpr MaxBits End = ( ( Bits.BitCount > Count ) ? Count : Bits.BitCount );

  Bits = Rhs.template Get< 0, End ? End - 1 : 0 >();

  ( End < Count ) ? ( Rest = Rhs.template Get< End, Count - 1 >() ) : ( Rest = BitField< 0 >{} );

  return *this;
}

template< MaxBits N >
BitField< N > & BitField< N >::operator=( std::initializer_list< bool > &&Bits_ ) ExceptDebug
{
  AssertDebug( Bits_.size() > ullong_BitCount, BitFieldTooLarge( static_cast< MaxBits >( Bits_.size() ) ) );

  for( size_t i = 0, End = Bits_.size(); i < End; ++i )
  {
    Bits |= ( *( Bits_.begin() + i ) << i );
  }

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
void BitField< N >::Set( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > N, IndexTooLarge( Index ) );

  ( Index < BitCount ) ? Bits.Set( Index ) : Rest.Set( Index - BitCount );
}

template< MaxBits N >
void BitField< N >::Set( MaxBits Index, bool Bit ) ExceptDebug
{
  AssertDebug( Index > N, IndexTooLarge( Index ) );

  ( Index < BitCount ) ? ( Bit ? Bits.Set( Index ) : Bits.Clear( Index ) ) : ( Bit ? Rest.Set( Index - BitCount ) : Rest.Clear( Index - BitCount ) );
}

template< MaxBits N >
bool BitField< N >::Get( MaxBits Index ) const ExceptDebug
{
  AssertDebug( Index > N, IndexTooLarge( Index ) );

  return ( Index < BitCount ) ? Bits.Get( Index  ) : Rest.Get( Index - BitCount );
}

template< MaxBits N >
void BitField< N >::Clear( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > N, IndexTooLarge( Index ) );

  ( Index < BitCount ) ? Bits.Clear( Index ) : Rest.Clear( Index - BitCount );
}

template< MaxBits N >
void BitField< N >::Negate( MaxBits Index ) ExceptDebug
{
  AssertDebug( Index > N, IndexTooLarge( Index ) );

  ( Index < BitCount ) ? Bits.Negate( Index ) : Rest.Negate( Index - BitCount );
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
template< MaxBits Start, MaxBits End >
constexpr BitField< End - Start + 1 > BitField< N >::Get() const ExceptDebug
{
  BitField< End - Start + 1 > Return{};

  for( auto i = Start; i <= End; ++i )
  {
    Return.Set( i - Start, Get( i ) );
  }

  return Return; // Named RVO
}

template< MaxBits N >
constexpr BitField< N > BitField< N >::Get( MaxBits Start, MaxBits End ) const ExceptDebug
{
  BitField< N > Return{};

  for( auto i = Start; i <= End; ++i )
  {
    Return.Set( i - Start, Get( i ) );
  }

  return Return; // Named RVO
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N>
constexpr MaxBits BitField< N >::Sizeof() noexcept
{
  return N;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
ullong BitField< N >::Number() const noexcept
{
  ullong Pow = 1;

  for( MaxBits i = 0; i < Bits.BitCount; ++i )
  {
    Pow *= 2;
  }

  return Bits.Number() + Rest.Number() * Pow;
}

template< MaxBits N >
const std::vector< std::vector< ullong > > BitField< N >::Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) const ExceptDebug
{
  std::vector< std::vector< ullong > > Return{};

  Return.resize( Intervals.size() );

  MaxBits Index = 0;
  for( auto Iter = Intervals.begin(), End = Intervals.end(); Iter != End; ++Iter, ++Index )
  {
    const MaxBits LoopCount = std::ceil( ( static_cast< float >( Iter->second ) - Iter->first ) / ullong_BitCount );

    Return[ Index ].reserve( LoopCount );

    MaxBits Position = static_cast< MaxBits >( Iter->first );
    for( MaxBits i = 0; i < LoopCount - 1; ++i, Position += ullong_BitCount )
    {
      Return[ Index ].push_back( Get( Position, Position + ullong_BitCount - 1 ).Number() );
    }

    Return[ Index ].push_back( Get( Position, static_cast< MaxBits >( Iter->second ) ).Number() );
  }

  return Return; // Named RVO
}

template< MaxBits N >
std::ostream & BitField< N >::Print1( std::ostream &os ) const noexcept
{
  for( MaxBits i = 0; i < N; ++i )
  {
    os << Get( i );
  }

  return os;
}

template< MaxBits N >
std::ostream & BitField< N >::Print2( std::ostream &os ) const noexcept
{
  return os << Bits << "-" << Rest;
}

///////////////////////////////////////////////////////////////////////////////

template< MaxBits N >
BitField< N >::IndexTooLarge::IndexTooLarge( MaxBits Index ) noexcept
{
  std::cerr << "Tried to Set/Get/Clear Index: " << Index << ", but the BitField only has " << N << " bits!" << std::endl;
}

template< MaxBits N >
BitField< N >::BitFieldTooLarge::BitFieldTooLarge( MaxBits Size ) noexcept
{
  std::cerr << "Tried to Copy/Move a BitField with size: " << Size << ", but the resulting BitField can only have " << N << " bits!";
  std::cerr << std::endl;
}
