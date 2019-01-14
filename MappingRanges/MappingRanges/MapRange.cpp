#include "MapRange.h"

#include <type_traits>

template< typename Type >
constexpr long double NumberToPercent( const typename std::enable_if< std::is_arithmetic< Type >::value, Type >::type &Number,
                                      const Vector< Type, 2 > &InRange ) noexcept
{
  return static_cast< long double >( ( Number - InRange.x ) ) / ( InRange.y - InRange.x );
}

constexpr long double NumberToPercent( const long double &Number, const Vector< long double, 2 > &InRange ) noexcept
{
  return ( Number - InRange.x ) / ( InRange.y - InRange.x );
}

template< typename InRangeType, typename OutRangeType >
constexpr typename std::enable_if< std::is_arithmetic< typename OutRangeType::Type >::value, typename OutRangeType::Type >::type
MapRange( const typename std::enable_if< std::is_arithmetic< InRangeType >::value, InRangeType >::type &Number,
          const Vector< InRangeType, 2 > &InRange, const OutRangeType &OutRange ) noexcept
{
  return static_cast< typename OutRangeType::Type >( NumberToPercent( Number, InRange ) * ( OutRange.y - OutRange.x ) + OutRange.x );
}

template< typename InRangeType >
constexpr long double MapRange( const typename std::enable_if< std::is_arithmetic< InRangeType >::value, InRangeType >::type &Number,
                                const Vector< InRangeType, 2 > &InRange, const Vector< long double, 2 > &OutRange) noexcept
{
  return NumberToPercent( Number, InRange ) * ( OutRange.y - OutRange.x ) + OutRange.x;
}

constexpr long double MapRange( const long double &Number, const Vector< long double, 2 > &InRange, 
                                const Vector< long double, 2 > &OutRange ) noexcept
{
  return NumberToPercent( Number, InRange ) * ( OutRange.y - OutRange.x ) + OutRange.x;
}
