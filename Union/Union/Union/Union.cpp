/*!------------------------------------------------------------------------------
\file   Union.cpp

\author Garrett Conti

\par    Project: Union
\par    Course:  FUN
   ------------------------------------------------------------------------------ */

#include "Union.h"

#include <utility>
#include <algorithm>
#include <execution>

template< typename LargestType >
Union< LargestType >::Union() noexcept
{
  std::memset( BitArray, 0, sizeof( LargestType ) );
}

template< typename LargestType >
template< typename Type >
Union< LargestType >::Union( FuncArgType< Type > Value ) noexcept
{
  static_assert( sizeof( Type ) <= sizeof( LargestType ), "Type can't be larger than the Union!" );

  if constexpr( IsTypeMemsetAble< Type >::value )
  {
    std::memset( BitArray, Value, sizeof( LargestType ) );
  }
  else if constexpr( IsTypeMemcpyAble< LargestType >::value )
  {
    if constexpr( sizeof( Type ) != sizeof( LargestType ) )
    {
      std::for_each_n( std::execution::parallel_unsequenced_policy{}, reinterpret_cast< Type * >( BitArray ), sizeof( LargestType ) / sizeof( Type ),
                       [ &Value ]( const Type *BitPtr )noexcept->void { std::memcpy( BitPtr, &Value, sizeof( Type ) ); } );

      if constexpr( sizeof( Type ) % sizeof( LargestType ) )
      {
        // Get how many bits to move over, then how many to set
        std::memset( BitArray + ( sizeof( LargestType ) / sizeof( Type ) ) * sizeof( Type ), 0, sizeof( LargestType ) - ( sizeof( Type ) % sizeof( LargestType ) ) );
      }
    }
    else // Just directly call memcpy
    {
      std::memcpy( BitArray, &Value, sizeof( Type ) );
    }
  }
  else if constexpr( IsTypeMoveAble< LargestType >::value )
  {
    if constexpr( sizeof( Type ) != sizeof( LargestType ) )
    {
      std::for_each_n( std::execution::parallel_unsequenced_policy{}, reinterpret_cast< Type * >( BitArray ) + 1, sizeof( LargestType ) / sizeof( Type ) - 1,
                       [ &Value ]( const Type *BitPtr )noexcept->void { *BitPtr = Value; } );

      if constexpr( sizeof( Type ) % sizeof( LargestType ) )
      {
        // Get how many bits to move over, then how many to set
        std::memset( BitArray + ( sizeof( LargestType ) / sizeof( Type ) ) * sizeof( Type ), 0, sizeof( LargestType ) - ( sizeof( Type ) % sizeof( LargestType ) ) );
      }
    }

    *( reinterpret_cast< LargestType * >( BitArray ) ) = std::move( Value );
  }
  else
  {
    if constexpr( sizeof( Type ) != sizeof( LargestType ) )
    {
      std::for_each_n( std::execution::parallel_unsequenced_policy{}, reinterpret_cast< Type * >( BitArray ), sizeof( LargestType ) / sizeof( Type ),
                       [ &Value ]( const Type *BitPtr )noexcept->void { *BitPtr = Value; } );

      if constexpr( sizeof( Type ) % sizeof( LargestType ) )
      {
        // Get how many bits to move over, then how many to set
        std::memset( BitArray + ( sizeof( LargestType ) / sizeof( Type ) ) * sizeof( Type ), 0, sizeof( LargestType ) - ( sizeof( Type ) % sizeof( LargestType ) ) );
      }
    }
    else // Directly copy the Value
    {
      *( reinterpret_cast< Type * >( BitArray ) ) = Value;
    }
  }
}

template< typename LargestType >
Union< LargestType >::Union( bool Value ) noexcept
{
  static_assert( Value, "Use the default constructor insted!" );

  // 0b1111'1111 sets all 8 bits in a char to true
  std::memset( BitArray, 0b1111'1111, sizeof( LargestType ) );  // Don't need UnionBitCount, since memset uses chars
}

template< typename LargestType >
Union< LargestType >::Union( FuncArgType< LargestType > Copy ) noexcept
{
  if constexpr( IsTypeMoveAble< LargestType >::value )
  {
    *( reinterpret_cast< LargestType * >( BitArray ) ) = std::move( Copy );
  }
  else if constexpr( IsTypeMemcpyAble< LargestType >::value )
  {
    std::memcpy( BitArray, &Copy, sizeof( LargestType ) ); // Don't need UnionBitCount, since memcpy uses chars
  }
  else
  {
    *( reinterpret_cast< LargestType * >( BitArray ) ) = Copy;
  }
}

template< typename LargestType >
Union< LargestType >::Union( LargestType &&Move ) noexcept
{
  *( reinterpret_cast< LargestType * >( BitArray ) ) = std::move( Move );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename LargestType >
template< typename Type >
typename Union< LargestType >::template FuncReturnType< Type > Union< LargestType >::GetType() const noexcept
{
  if constexpr( std::is_same< bool, Type >::value )
  {
    return ( ( *BitArray & 1 ) != 0 );
  }
  else if constexpr( std::is_same< char, Type >::value )
  {
    return *BitArray;
  }
  else
  {
    static_assert( sizeof( Type ) <= sizeof( LargestType ), "Type can't be larger than the Union!" );

    return *( reinterpret_cast< const Type * >( BitArray ) );
  }
}

template< typename LargestType >
template< typename Type >
typename Union< LargestType >::template FuncReturnType< Type > Union< LargestType >::GetType( const size_t Index ) const noexcept
{
  if constexpr( std::is_same< bool, Type >::value )
  {
    return ( ( BitArray[ Index / BitCount ] & ( 1 << ( Index % BitCount ) ) ) != 0 );
  }
  else if constexpr( std::is_same< char, Type >::value )
  {
    return *( BitArray + Index );
  }
  else
  {
    static_assert( sizeof( Type ) <= sizeof( LargestType ), "Type can't be larger then the Union!" );
    static_assert( sizeof( Type ) != sizeof( LargestType ), "Use the GetType without an Index!" );

    return *( reinterpret_cast< const Type * >( BitArray ) + Index );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename LargestType >
template< typename Type >
void Union< LargestType >::SetType( FuncArgType< Type > Value ) noexcept
{
  if constexpr( std::is_same< bool, Type >::value )
  {
    Value ? *BitArray |= 1 : *BitArray &= ~1;
  }
  else
  {
    static_assert( sizeof( Type ) <= sizeof( LargestType ), "Type can't be larger than the Union!" );

    if constexpr( IsTypeMemsetAble< Type >::value )
    {
      std::memset( BitArray, Value, sizeof( Type ) );
    }
    else if constexpr( IsTypeMemcpyAble< LargestType >::value )
    {
      std::memcpy( BitArray, &Value, sizeof( Type ) );
    }
    else if constexpr( IsTypeMoveAble< LargestType >::value )
    {
      *( reinterpret_cast< LargestType * >( BitArray ) ) = std::move( Value );
    }
    else
    {
      *( reinterpret_cast< Type * >( BitArray ) ) = Value;
    }
  }
}

template< typename LargestType >
template< typename Type >
void Union< LargestType >::SetType( FuncArgType< Type > Value, const size_t Index ) noexcept
{
  if constexpr( std::is_same< bool, Type >::value )
  {
    if( Value )
    {
      BitArray[ Index / BitCount ] |= ( 1 << Index % BitCount );
    }
    else
    {
      BitArray[ Index / BitCount ] &= ~( 1 << Index % BitCount );
    }
  }
  else
  {
    static_assert( sizeof( Type ) <= sizeof( LargestType ), "Type can't be larger then the Union!" );
    static_assert( sizeof( Type ) != sizeof( LargestType ), "Use the GetType without an Index!" );

    if constexpr( IsTypeMemsetAble< Type >::value )
    {
      std::memset( BitArray + sizeof( Type ) * Index, Value, sizeof( Type ) );
    }
    else if constexpr( IsTypeMemcpyAble< LargestType >::value )
    {
      std::memcpy( BitArray + sizeof( Type ) * Index, &Value, sizeof( Type ) );
    }
    else if constexpr( IsTypeMoveAble< LargestType >::value )
    {
      *( reinterpret_cast< LargestType * >( BitArray ) + Index ) = std::move( Value );
    }
    else
    {
      *( reinterpret_cast< Type * >( BitArray )+ Index ) = Value;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
