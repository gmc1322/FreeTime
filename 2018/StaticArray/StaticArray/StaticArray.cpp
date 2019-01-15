
#include "StaticArray.h"

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::StaticArray( const StaticArray &Copy ) noexcept
{
  *this = Copy;
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack...>::StaticArray( StaticArray &&Move ) noexcept
{
  *this = std::move( Move );
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::StaticArray( const std::initializer_list< Type > &Args ) ExceptDebug
{
  *this = Args;
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::StaticArray( const StaticArrayInitType &Args ) ExceptDebug
{
  *this = Args;
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::StaticArray( StaticArrayInitType &&Args ) ExceptDebug
{
  *this = std::move( Args );
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
void StaticArray< StaticArrayType, FirstSize, SizePack... >::operator=( const StaticArray &Copy ) noexcept
{
  for( size_t i = 0; i < FirstSize; ++i )
  {
    Array[ i ] = Copy.Array[ i ];
  }
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
void StaticArray< StaticArrayType, FirstSize, SizePack... >::operator=( StaticArray &&Move ) noexcept
{
  for( size_t i = 0; i < FirstSize; ++i )
  {
    Array[ i ] = std::move( Move[ i ] );
  }
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
void StaticArray< StaticArrayType, FirstSize, SizePack... >::operator=( const std::initializer_list< Type > &Args ) ExceptDebug
{
  AssertDebug( Args.size() > FirstSize, ( TooManyInitializers{ Args.size() } ) );

  size_t i = 0;

  for( auto Iter : Args )
  {
    Array[ i ] = Iter;

    ++i;
  }

  for( ; i < FirstSize; ++i )
  {
    Array[ i ] = Type();
  }
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
void StaticArray< StaticArrayType, FirstSize, SizePack... >::operator=( const StaticArrayInitType &Args ) ExceptDebug
{
  AssertDebug( Args.size() > FirstSize, ( TooManyInitializers{ Args.size() } ) );

  size_t i = 0;

  for( auto Iter : Args )
  {
    Array[ i ] = Iter;

    ++i;
  }

  for( ; i < FirstSize; ++i )
  {
    Array[ i ] = Type();
  }
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
void StaticArray< StaticArrayType, FirstSize, SizePack... >::operator=( StaticArrayInitType &&Args ) ExceptDebug
{
  AssertDebug( Args.size() > FirstSize, ( TooManyInitializers{ Args.size() } ) );

  size_t i = 0;

  for( auto Iter = Args.begin(), End = Args.end(); Iter != End; ++Iter, ++i )
  {
    Array[ i ] = std::move( *Iter );
  }

  for( ; i < FirstSize; ++i )
  {
    Array[ i ] = Type();
  }
}

#ifdef _DEBUG
template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
typename StaticArray< StaticArrayType, FirstSize, SizePack... >::Type &
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= FirstSize, ( IndexOutOfBounds{ Index } ) );

  return Array[ Index ];
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
const typename StaticArray< StaticArrayType, FirstSize, SizePack... >::Type & 
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator[]( size_t Index ) const
{
  AssertDebug( Index >= FirstSize, ( IndexOutOfBounds{ Index } ) );

  return Array[ Index ];
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
typename StaticArray< StaticArrayType, FirstSize, SizePack... >::ArrayPtr< 
  typename StaticArray< StaticArrayType, FirstSize, SizePack... >::Type, FirstSize >
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator+( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= FirstSize, ( IndexOutOfBounds{ Index } ) );

  DebugElse(
    return ( ArrayPtr< Type, FirstSize >{ Array + Index, Index } );

  ,

    return Array + Index;
  )
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
typename StaticArray< StaticArrayType, FirstSize, SizePack... >::ConstArrayPtr< 
  const typename StaticArray< StaticArrayType, FirstSize, SizePack... >::Type, FirstSize >
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator+( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= FirstSize, ( IndexOutOfBounds{ Index } ) );

  DebugElse(
    return ( ConstArrayPtr< const Type, FirstSize >{ Array + Index, Index } );

  ,

    return Array + Index;
  )
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
int StaticArray< StaticArrayType, FirstSize, SizePack... >::operator-( const StaticArray &Rhs ) const noexcept
{
  const int Return = Array - Rhs.Array;
  return Return; // Split for Named RVO
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
typename StaticArray< StaticArrayType, FirstSize, SizePack... >::Type &
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator*() noexcept
{
  return *Array;
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
const typename StaticArray< StaticArrayType, FirstSize, SizePack... >::Type &
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator*() const noexcept
{
  return *Array;
}
#endif

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator
ArrayPtr< StaticArray< StaticArrayType, SizePack... >, FirstSize >() noexcept
{
  DebugElse(
    return ( ArrayPtr< Type, FirstSize >{ Array, 0 } );

  ,

    return Array;
  )
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::operator 
ConstArrayPtr< const StaticArray< StaticArrayType, SizePack... >, FirstSize >() const noexcept
{
  DebugElse(
    return ( ConstArrayPtr< const Type, FirstSize >{ Array, 0 } );

  ,

    return Array;
  )
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
StaticArray< StaticArrayType, SizePack... > * StaticArray< StaticArrayType, FirstSize, SizePack... >::begin() noexcept
{
  return Array;
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
const StaticArray< StaticArrayType, SizePack... > * StaticArray< StaticArrayType, FirstSize, SizePack... >::begin() const noexcept
{
  return Array;
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
StaticArray< StaticArrayType, SizePack... > * StaticArray< StaticArrayType, FirstSize, SizePack... >::end() noexcept
{
  return Array + FirstSize;
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
const StaticArray< StaticArrayType, SizePack... > * StaticArray< StaticArrayType, FirstSize, SizePack... >::end() const noexcept
{
  return Array + FirstSize;
}

template< typename StaticArrayType, size_t FirstSize, size_t... SizePack >
template< size_t Dimension >
constexpr size_t StaticArray< StaticArrayType, FirstSize, SizePack... >::Size() noexcept
{
  return Dimension == 1 ? FirstSize : StaticArray< StaticArrayType, SizePack... >::template Size< Dimension - 1 >();
}

///////////////////////////////////////////////////////////////////////////////

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::TooManyInitializers::TooManyInitializers( size_t Count ) noexcept
{
  std::cerr << "Tried to initialize the array with " << Count << " initializers, but the array is size: " << FirstSize << std::endl;
}

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
StaticArray< StaticArrayType, FirstSize, SizePack... >::IndexOutOfBounds::IndexOutOfBounds( size_t Index ) noexcept
{
  std::cerr << "Tried to get index: " << Index << ", but the array is size: " << FirstSize << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::StaticArray( const StaticArray &Copy ) noexcept
{
  *this = Copy;
}

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::StaticArray( StaticArray &&Move ) noexcept
{
  *this = std::move( Move );
}

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::StaticArray( const std::initializer_list< ArrayType > &Args ) ExceptDebug
{
  *this = Args;
}

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::StaticArray( const StaticArrayInitType &Args ) ExceptDebug
{
  *this = Args;
}

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::StaticArray( StaticArrayInitType &&Args ) ExceptDebug
{
  *this = std::move( Args );
}

template< typename ArrayType, size_t LastSize >
void StaticArray< ArrayType, LastSize >::operator=( const StaticArray &Copy ) noexcept
{
  for( size_t i = 0; i < LastSize; ++i )
  {
    Array[ i ] = Copy[ i ];
  }
}

template< typename ArrayType, size_t LastSize >
void StaticArray< ArrayType, LastSize >::operator=( StaticArray &&Move ) noexcept
{
  for( size_t i = 0; i < LastSize; ++i )
  {
    Array[ i ] = std::move( Move[ i ] );
  }
}

template< typename ArrayType, size_t LastSize >
void StaticArray< ArrayType, LastSize >::operator=( const std::initializer_list< ArrayType > &Args ) ExceptDebug
{
  AssertDebug( Args.size() > LastSize, ( TooManyInitializers{ Args.size() } ) );

  size_t i = 0;

  for( auto Iter : Args )
  {
    Array[ i ] = Iter;

    ++i;
  }

  for( ; i < LastSize; ++i )
  {
    Array[ i ] = ArrayType();
  }
}

template < typename ArrayType, size_t LastSize >
void StaticArray< ArrayType, LastSize >::operator=( const StaticArrayInitType &Args ) ExceptDebug
{
  AssertDebug( Args.size() > LastSize, ( TooManyInitializers{ Args.size() } ) );

  size_t i = 0;

  for( auto Iter : Args )
  {
    Array[ i ] = Iter;

    ++i;
  }

  for( ; i < LastSize; ++i )
  {
    Array[ i ] = ArrayType();
  }
}

template< typename ArrayType, size_t LastSize >
void StaticArray< ArrayType, LastSize >::operator=( StaticArrayInitType &&Args ) ExceptDebug
{
  AssertDebug( Args.size() > LastSize, ( TooManyInitializers{ Args.size() } ) );

  size_t i = 0;

  for( auto Iter = Args.begin(), End = Args.end(); Iter != End; ++Iter, ++i )
  {
    Array[ i ] = std::move( *Iter );
  }

  for( ; i < LastSize; ++i )
  {
    Array[ i ] = ArrayType();
  }
}

#ifdef _DEBUG
template< typename ArrayType, size_t LastSize >
ArrayType & StaticArray< ArrayType, LastSize >::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= LastSize, ( IndexOutOfBounds{ Index } ) );

  return Array[ Index ];
}

template< typename ArrayType, size_t LastSize >
const ArrayType & StaticArray< ArrayType, LastSize >::operator[]( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= LastSize, ( IndexOutOfBounds{ Index } ) );

  return Array[ Index ];
}

template< typename ArrayType, size_t LastSize >
typename StaticArray< ArrayType, LastSize >::ArrayPtr< ArrayType, LastSize >
StaticArray< ArrayType, LastSize >::operator+( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= LastSize, ( IndexOutOfBounds{ Index } ) );

  DebugElse(
    return ( ArrayPtr< ArrayType, LastSize >{ Array + Index, Index } );

    ,

    return Array + Index;
  )
}

template< typename ArrayType, size_t LastSize >
typename StaticArray< ArrayType, LastSize >::ConstArrayPtr< const ArrayType, LastSize >
StaticArray< ArrayType, LastSize >::operator+( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= LastSize, ( IndexOutOfBounds{ Index } ) );

  DebugElse(
    return ( ConstArrayPtr< const ArrayType, LastSize >{ Array + Index, Index } );

    ,

    return Array + Index;
  )
}


template< typename ArrayType, size_t LastSize >
int StaticArray< ArrayType, LastSize >::operator-( const StaticArray &Rhs ) const noexcept
{
  const int Return = Array - Rhs.Array;
  return Return; // Split for Named RVO
}

template< typename ArrayType, size_t LastSize >
ArrayType & StaticArray< ArrayType, LastSize >::operator*() noexcept
{
  return *Array;
}

template< typename ArrayType, size_t LastSize >
const ArrayType & StaticArray< ArrayType, LastSize >::operator*() const noexcept
{
  return *Array;
}
#endif

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::operator ArrayPtr< ArrayType, LastSize >() noexcept
{
  DebugElse(
    return ( ArrayPtr< ArrayType, LastSize >{ Array, 0 } );

    ,

    return Array;
  )
}

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::operator ConstArrayPtr< const ArrayType, LastSize >() const noexcept
{
  DebugElse(
    return ( ConstArrayPtr< const ArrayType, LastSize >{ Array, 0 } );

  ,

    return Array;
  )
}

template< typename ArrayType, size_t LastSize >
ArrayType * StaticArray< ArrayType, LastSize >::begin() noexcept
{
  return Array;
}

template< typename ArrayType, size_t LastSize >
const ArrayType * StaticArray< ArrayType, LastSize >::begin() const noexcept
{
  return Array;
}

template< typename ArrayType, size_t LastSize >
ArrayType * StaticArray< ArrayType, LastSize >::end() noexcept
{
  return Array + LastSize;
}

template< typename ArrayType, size_t LastSize >
const ArrayType * StaticArray< ArrayType, LastSize >::end() const noexcept
{
  return Array + LastSize;
}

template< typename ArrayType, size_t LastSize >
template< size_t Dimension >
constexpr size_t StaticArray< ArrayType, LastSize >::Size() noexcept
{
  return LastSize;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::TooManyInitializers::TooManyInitializers( size_t Count ) noexcept
{
  std::cerr << "Tried to initialize the array with " << Count << " initializers, but the array is size: " << LastSize << std::endl;
}

template< typename ArrayType, size_t LastSize >
StaticArray< ArrayType, LastSize >::IndexOutOfBounds::IndexOutOfBounds( size_t Index ) noexcept
{
  std::cerr << "Tried to get index: " << Index << ", but the array is size: " << LastSize << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
