#pragma once

#include "Vector.h"

template< typename Type >
constexpr long double NumberToPercent( const typename std::enable_if< std::is_arithmetic< Type >::value, Type >::type &Number,
                            const Vector< Type, 2 > &InRange ) noexcept;

constexpr long double NumberToPercent( const long double &Number, const Vector < long double, 2 > &InRange ) noexcept;

/**
 * \brief Maps a Number from the InRange to the OutRange.
 *
 * \tparam InRangeType  - The Type for the Number and InRange.
 * \tparam OutRangeType - The Type for the Result and OutRange.
 *
 * \param Number   - The Number to map.
 * \param InRange  - The Range the number is in.
 * \param OutRange - The Range the resulting number is in.
 *
 * \return OutRangeType::Type - The resulting Mapped Number.
 */
template< typename InRangeType, typename OutRangeType >
constexpr typename std::enable_if< std::is_arithmetic< typename OutRangeType::Type >::value, typename OutRangeType::Type >::type
MapRange( const typename std::enable_if< std::is_arithmetic< InRangeType >::value, InRangeType >::type &Number,
          const Vector< InRangeType, 2 > &InRange, const OutRangeType &OutRange ) noexcept;

/**
 * \brief Maps a Number from the InRange to the OutRange.
 * 
 * \tparam InRangeType  - The Type for the Number and InRange.
 *
 * \param Number   - The Number to map.
 * \param InRange  - The Range the number is in.
 * \param OutRange - The Range the resulting number is in.
 *
 * \return long double - The resulting Mapped Number.
 */
template< typename InRangeType >
constexpr long double MapRange( const typename std::enable_if< std::is_arithmetic< InRangeType >::value, InRangeType >::type &Number,
                                const Vector< InRangeType, 2 > &InRange, const Vector< long double, 2 > &OutRange ) noexcept;

/**
 * \brief Maps a Number from the InRange to the OutRange.
 *
 * \param Number   - The Number to map.
 * \param InRange  - The Range the number is in.
 * \param OutRange - The Range the resulting number is in.
 *
 * \return long double - The resulting Mapped Number.
 */
constexpr long double MapRange( const long double &Number, const Vector< long double, 2 > &InRange, 
                                const Vector< long double, 2 > &OutRange ) noexcept;

#include "MapRange.cpp"
