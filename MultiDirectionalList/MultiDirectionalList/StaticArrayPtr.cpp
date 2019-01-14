
#include "StaticArrayPtr.h"

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size >::StaticArrayPtr( PointerType *Ptr, size_t Pos ) noexcept : Ptr( Ptr ), Pos( Pos )
{
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size >::StaticArrayPtr( PointerType* *Ptr_, size_t Pos ) noexcept : Ptr( *Ptr_ ), Pos( Pos )
{
  *Ptr_ = nullptr;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size >::StaticArrayPtr( const StaticArrayPtr &Copy ) noexcept : Ptr( Copy.Ptr ), Pos( Copy.Pos )
{
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size >::StaticArrayPtr( StaticArrayPtr &&Move ) noexcept : Ptr( Move.Ptr ), Pos( Move.Pos )
{
  Move.Ptr = nullptr;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > &
StaticArrayPtr< PointerType, Size >::operator=( const std::pair< PointerType*, size_t > &PtrPair ) noexcept
{
  Ptr = PtrPair.first;
  Pos = PtrPair.second;

  return *this;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > &
StaticArrayPtr< PointerType, Size >::operator=( std::pair< PointerType*, size_t > &&PtrPair ) noexcept
{
  Ptr = PtrPair.first;
  Pos = PtrPair.second;

  PtrPair.first = nullptr;

  return *this;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > & StaticArrayPtr< PointerType, Size >::operator=( const StaticArrayPtr &Copy ) noexcept
{
  Ptr = Copy.Ptr;
  Pos = Copy.Pos;

  return *this;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > & StaticArrayPtr< PointerType, Size >::operator=( StaticArrayPtr &&Move ) noexcept
{
  Ptr = Move.Ptr;
  Pos = Move.Pos;

  Move.Ptr = nullptr;

  return *this;
}

template< typename PointerType, size_t Size >
PointerType& StaticArrayPtr< PointerType, Size >::operator[]( int Index ) ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0, 
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

  return Ptr[ Index ];
}

template< typename PointerType, size_t Size >
const PointerType& StaticArrayPtr< PointerType, Size >::operator[]( int Index ) const ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Size, Pos } ) )

  return Ptr[ Index ];
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size >  StaticArrayPtr< PointerType, Size >::operator+( int Index ) ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

    return StaticArrayPtr{ Ptr + Index, Pos + Index };
}

template< typename PointerType, size_t Size >
StaticArrayPtr< const PointerType, Size > StaticArrayPtr< PointerType, Size >::operator+( int Index ) const ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

  return StaticArrayPtr{ Ptr + Index, Pos + Index };
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > StaticArrayPtr< PointerType, Size >::operator-( int Index ) ExceptDebug
{
  AssertDebug( Pos - Index >= Size || static_cast< int >( Pos ) - Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) - Index, Pos } ) )

    return StaticArrayPtr{ Ptr - Index, Pos - Index };
}

template< typename PointerType, size_t Size >
StaticArrayPtr< const PointerType, Size > StaticArrayPtr< PointerType, Size >::operator-( int Index ) const ExceptDebug
{
  AssertDebug( Pos - Index >= Size || static_cast< int >( Pos ) - Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) - Index, Pos } ) )

  return StaticArrayPtr{ Ptr - Index, Pos - Index };
}

template< typename PointerType, size_t Size >
int StaticArrayPtr< PointerType, Size >::operator-( const StaticArrayPtr &Rhs ) const noexcept
{
  return Ptr - Rhs.Ptr;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > & StaticArrayPtr< PointerType, Size >::operator+=( int Index ) ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) );

  Ptr += Index;
  Pos += Index;

  return *this;
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > & StaticArrayPtr< PointerType, Size >::operator-=( int Index ) ExceptDebug
{
  AssertDebug( static_cast< int >( Pos ) - Index < 0 || Pos - Index >= Size,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) - Index, Pos } ) );

  Ptr -= Index;
  Pos -= Index;

  return *this;
}

template< typename PointerType, size_t Size >
PointerType & StaticArrayPtr< PointerType, Size >::operator*() noexcept
{
  return *Ptr;
}

template< typename PointerType, size_t Size >
const PointerType & StaticArrayPtr< PointerType, Size >::operator*() const noexcept
{
  return *Ptr;
}

template< typename PointerType, size_t Size >
PointerType* * StaticArrayPtr< PointerType, Size >::operator&() noexcept
{
  return &Ptr;
}

template< typename PointerType, size_t Size >
const PointerType* const* StaticArrayPtr< PointerType, Size >::operator&() const noexcept
{
  return &Ptr;
}

///////////////////////////////////////////////////////////////////////////////

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size >::IndexOutOfBounds::IndexOutOfBounds( int Index, size_t Pos ) noexcept
{
  std::cerr << "Tried to get index: " << Index;

  if( Index > 0 )
  {
    std::cerr << ", but the array is size: " << Size << std::endl;
  }
  else
  {
    std::cerr << ", but the pointer is at position: " << Pos << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size >::ConstStaticArrayPtr( PointerType *Ptr, size_t Pos ) noexcept : Ptr( Ptr ), Pos( Pos )
{
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size >::ConstStaticArrayPtr( PointerType* *Ptr_, size_t Pos ) noexcept : Ptr( *Ptr_ ), Pos( Pos )
{
  *Ptr_ = nullptr;
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size >::ConstStaticArrayPtr( const ConstStaticArrayPtr &Copy ) noexcept : Ptr( Copy.Ptr ), Pos( Copy.Pos )
{
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size >::ConstStaticArrayPtr( ConstStaticArrayPtr &&Move ) noexcept : Ptr( Move.Ptr ), Pos( Move.Pos )
{
  Move.Ptr = nullptr;
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size > & 
ConstStaticArrayPtr< PointerType, Size >::operator=( const std::pair< PointerType*, size_t > &PtrPair ) noexcept
{
  Ptr = PtrPair.first;
  Pos = PtrPair.second;

  return *this;
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size > &
ConstStaticArrayPtr< PointerType, Size >::operator=( std::pair< PointerType*, size_t > &&PtrPair ) noexcept
{
  Ptr = PtrPair.first;
  Pos = PtrPair.second;

  PtrPair.first = nullptr;

  return *this;
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size > & ConstStaticArrayPtr< PointerType, Size >::operator=( const ConstStaticArrayPtr &Copy ) noexcept
{
  Ptr = Copy.Ptr;
  Pos = Copy.Pos;

  return *this;
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size > & ConstStaticArrayPtr< PointerType, Size >::operator=( ConstStaticArrayPtr &&Move ) noexcept
{
  Ptr = Move.Ptr;
  Pos = Move.Pos;

  Move.Ptr = nullptr;

  return *this;
}

template< typename PointerType, size_t Size >
PointerType& ConstStaticArrayPtr< PointerType, Size >::operator[]( int Index ) ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

    return Ptr[ Index ];
}

template< typename PointerType, size_t Size >
const PointerType& ConstStaticArrayPtr< PointerType, Size >::operator[]( int Index ) const ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Size, Pos } ) )

    return Ptr[ Index ];
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > ConstStaticArrayPtr< PointerType, Size >::operator+( int Index ) ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

    return StaticArrayPtr< PointerType, Size >{ Ptr + Index, Pos + Index };
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< const PointerType, Size > ConstStaticArrayPtr< PointerType, Size >::operator+( int Index ) const ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

    return ConstStaticArrayPtr{ Ptr + Index, Pos + Index };
}

template< typename PointerType, size_t Size >
StaticArrayPtr< PointerType, Size > ConstStaticArrayPtr< PointerType, Size >::operator-( int Index ) ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

    return StaticArrayPtr< PointerType, Size >{ Ptr - Index, Pos - Index };
}

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< const PointerType, Size > ConstStaticArrayPtr< PointerType, Size >::operator-( int Index ) const ExceptDebug
{
  AssertDebug( Pos + Index >= Size || static_cast< int >( Pos ) + Index < 0,
    ( IndexOutOfBounds{ static_cast< int >( Pos ) + Index, Pos } ) )

    return ConstStaticArrayPtr{ Ptr - Index, Pos - Index };
}

template< typename PointerType, size_t Size >
int ConstStaticArrayPtr< PointerType, Size >::operator-( const ConstStaticArrayPtr &Rhs ) const noexcept
{
  return Ptr - Rhs.Ptr;
}

template< typename PointerType, size_t Size >
PointerType & ConstStaticArrayPtr< PointerType, Size >::operator*() noexcept
{
  return *Ptr;
}

template< typename PointerType, size_t Size >
const PointerType & ConstStaticArrayPtr< PointerType, Size >::operator*() const noexcept
{
  return *Ptr;
}

template< typename PointerType, size_t Size >
PointerType* const* ConstStaticArrayPtr< PointerType, Size >::operator&() noexcept
{
  return &Ptr;
}

template< typename PointerType, size_t Size >
const PointerType* const* ConstStaticArrayPtr< PointerType, Size >::operator&() const noexcept
{
  return &Ptr;
}

///////////////////////////////////////////////////////////////////////////////

template< typename PointerType, size_t Size >
ConstStaticArrayPtr< PointerType, Size >::IndexOutOfBounds::IndexOutOfBounds( int Index, size_t Pos ) noexcept
{
  std::cerr << "Tried to get index: " << Index;

  if( Index > 0 )
  {
    std::cerr << ", but the array is size: " << Size << std::endl;
  }
  else
  {
    std::cerr << ", but the pointer is at position: " << Pos << std::endl;
  }
}
