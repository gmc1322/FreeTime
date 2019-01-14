/*!------------------------------------------------------------------------------
\file   Union.h

\author Garrett Conti

\par    Project: Union
\par    Course:  FUN
   ------------------------------------------------------------------------------ */

#pragma once

#include <type_traits>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type1, typename ... Types >
struct GetLargestType
{
  using Type = typename std::conditional< sizeof( Type1 ) < GetLargestType< Types... >::Size, typename GetLargestType< Types... >::Type, Type1 >::type;

  static constexpr size_t Size = sizeof( Type );
};

template< typename Type1, typename Type2 >
struct GetLargestType< Type1, Type2 >
{
  using Type = typename std::conditional< sizeof( Type1 ) < sizeof( Type2 ), Type2, Type1 >::type;

  static constexpr size_t Size = sizeof( Type );
};

// Handle only 1 type
template< typename Type >
struct GetLargestType< Type >
{
  static constexpr size_t Size = 1;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type, typename Check, typename... OtherChecks >
struct IsContainedIn
{
  static constexpr bool IsContained = std::is_same< Type, Check >::value || IsContainedIn< Type, OtherChecks... >::IsContained;
};

template< typename Type, typename Check >
struct IsContainedIn< Type, Check >
{
  static constexpr bool IsContained = std::is_same< Type, Check >::value;
};

template< typename Type, typename... Checks >
struct AreTypesUnique
{
  static constexpr bool IsUnique = AreTypesUnique< Checks... >::IsUnique && !( IsContainedIn< Type, Checks... >::IsContained );
};

template< typename Type1, typename Type2 >
struct AreTypesUnique< Type1, Type2 >
{
  static constexpr bool IsUnique = !( IsContainedIn< Type1, Type2 >::IsContained );
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename LargestType >
class Union
{
  static_assert( !( std::is_same< bool, LargestType >::value ), "A Union can't have a bool as its largest type!" );
  static_assert( !( std::is_void< LargestType >::value ), "A Union can't have a void type!" );

  private:
  template< typename Type >
  using IsTypeSmall = std::bool_constant< sizeof( Type ) <= sizeof( void* ) >;

  template< typename Type >
  using IsTypeMoveAble = std::is_move_constructible< Type >;

  template< bool TrueFalse, typename T1, typename T2 >
  using TemplateIf = typename std::conditional< TrueFalse, T1, T2 >::type;

  private:
    template< typename Type >
    using FuncArgType = TemplateIf< IsTypeSmall< Type >::value, const Type,
                                    TemplateIf< std::is_pointer< Type >::value, const Type *const,
                                                TemplateIf< IsTypeMoveAble< Type >::value, Type, const Type & > > >;
    template< typename Type >
    using FuncReturnType = typename std::conditional< IsTypeSmall< Type >::value, Type, const Type & >::type;

  private:
    template< typename Type >
    using IsTypeMemsetAble = std::bool_constant< std::is_arithmetic< Type >::value && sizeof( Type ) <= sizeof( int ) >;

    template< typename Type >
    using IsTypeMemcpyAble = std::is_trivially_copyable< Type >;

  public:
    Union() noexcept;

    // Sets as many bits as possible to Value, all others to 0
    template< typename Type >
    explicit Union( FuncArgType< Type > Value ) noexcept;
    // Sets as all bits to 1
    explicit Union( bool Value ) noexcept; 

    explicit Union( FuncArgType< LargestType > Copy ) noexcept;

    explicit Union( LargestType && Move ) noexcept;

  public:
    template< typename Type >
    static constexpr size_t GetEndIndex() noexcept; // Returns the first index UNUSABLE by GetType
  
    template< typename Type >
    static constexpr size_t GetLargestIndex() noexcept; // Returns the largest index usable by GetType, this is NOT one past the end!

  // The explicit return type is required for VS to not whine
  public:
    template< typename Type >
    typename Union< LargestType >::template FuncReturnType< Type > GetType() const noexcept;

    template< typename Type >
    typename Union< LargestType >::template FuncReturnType< Type > GetType( size_t Index ) const noexcept;

  public:
    template< typename Type >
    void SetType( FuncArgType< Type > Value ) noexcept;

    template< typename Type >
    void SetType( FuncArgType< Type > Value, size_t Index ) noexcept;

  private:
    static constexpr size_t BitCount = sizeof( LargestType ) * 8; // 8 bits to a character

  private:
    char BitArray[ sizeof( LargestType ) ];
};

template< typename LargestType >
template< typename Type >
constexpr size_t Union< LargestType >::GetEndIndex() noexcept
{
  if constexpr( std::is_same< bool, Type >::value )
  {
    return sizeof( LargestType ) * 8;
  }
  else
  {
    return sizeof( LargestType ) / sizeof( Type );
  }
}

template < typename LargestType >
template < typename Type >
constexpr size_t Union< LargestType >::GetLargestIndex() noexcept
{
  if constexpr( std::is_same< bool, Type >::value )
  {
    return sizeof( LargestType ) * 8 - 1;
  }
  else
  {
    return sizeof( LargestType ) / sizeof( Type ) - 1;
  }
}

#include "Union.cpp"
