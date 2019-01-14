/*!------------------------------------------------------------------------------
\file   WrappingUtils.h

\author Garrett Conti

\par    Project: Golems_Magic
\par    Course:  SUMMER
------------------------------------------------------------------------------ */
#pragma once

#include "Public/Utils/Macros.h"

using uint8 = unsigned char;
using int32 = __int32;

class UWrappingUtils
{

  public:
    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return uint8 - The resulting wrapped value.
     */
    static uint8 WrapByte( uint8 InValue, uint8 Min, uint8 Max ) NoExcept;

    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return int32 - The resulting wrapped value.
     */
    static int32 WrapInt( int32 InValue, int32 Min, int32 Max ) NoExcept;

    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue   - The value to wrap.
     * \param Min       - The min for Value.
     * \param Max       - The max for Value.
     * \param Precision - The required precision for wrapping.
     *
     * \return float - The resulting wrapped value.
     */
    static float WrapFloat( float InValue, float Min, float Max, uint8 Precision = 2 ) NoExcept;

    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return Size_T - The resulting wrapped value.
     *
     * NOTE! This is not a blueprintable function!
     */
    static Size_T WrapSize( Size_T InValue, Size_T Min, Size_T Max ) NoExcept;

  public:
    /**
     * \brief Wraps a number to min or max.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return uint8 - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min or max.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    static uint8 QuickWrapByte( uint8 InValue, uint8 Min, uint8 Max ) NoExcept;

    /**
     * \brief Wraps a number to min or max.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return int32 - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min or max.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    static int32 QuickWrapInt( int32 InValue, int32 Min, int32 Max ) NoExcept;

    /**
     * \brief Wraps a number to min or max.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return float - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min or max.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    static float QuickWrapFloat( float InValue, float Min, float Max ) NoExcept;

    /**
     * \brief Wraps a number to min or max.
     *
     * \tparam ValueType - The Type to wrap.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return ValueType - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min or max.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    template< typename ValueType >
    static constexpr ValueType QuickWrap( const ValueType InValue, const ValueType Min, const ValueType Max ) NoExcept
    {
      return ( InValue < Min ? Max : ( InValue > Max ? Min : InValue ) ); // NRVO
    };

  public:
    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return int32 - The resulting wrapped value.
     */
    static uint8 WrapByte_Min( uint8 InValue, uint8 Min, uint8 Max ) NoExcept;

    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return int32 - The resulting wrapped value.
     */
    static int32 WrapInt_Min( int32 InValue, int32 Min, int32 Max ) NoExcept;

    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue   - The value to wrap.
     * \param Min       - The min for Value.
     * \param Max       - The max for Value.
     * \param Precision - The required precision for wrapping.
     *
     * \return void
     */
    static float WrapFloat_Min( float InValue, float Min, float Max, uint8 Precision = 2 ) NoExcept;

    /**
     * \brief Wraps a number to the range [min, max].
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return Size_T - The resulting wrapped value.
     *
     * NOTE! This is not a blueprintable function!
     */
    static Size_T WrapSize_Min( Size_T InValue, Size_T Min, Size_T Max ) NoExcept;

  public:
    /**
     * \brief Wraps a number to min.
     *
     * \tparam ValueType - The Type to wrap.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return ValueType - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    static uint8 QuickWrapByte_Min( uint8 InValue, uint8 Min, uint8 Max ) NoExcept;

    /**
     * \brief Wraps a number to min.
     *
     * \tparam ValueType - The Type to wrap.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return ValueType - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    static int32 QuickWrapInt_Min( int32 InValue, int32 Min, int32 Max ) NoExcept;

    /**
     * \brief Wraps a number to min.
     *
     * \tparam ValueType - The Type to wrap.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return ValueType - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     */
    static float QuickWrapFloat_Min( float InValue, float Min, float Max ) NoExcept;

    /**
     * \brief Wraps a number to min.
     *
     * \tparam ValueType - The Type to wrap.
     *
     * \param InValue  - The value to wrap.
     * \param Min      - The min for Value.
     * \param Max      - The max for Value.
     *
     * \return ValueType - The resulting wrapped value.
     *
     * Does not take into account how far away Value is from min.
     * You can think of it as an Inverse Clamp, Val < Min -> Val = Max.
     *
     * NOTE! This is not a blueprintable function!
     */
    template< typename ValueType >
    static constexpr ValueType QuickWrap_Min( const ValueType InValue, const ValueType Min, const ValueType Max ) NoExcept
    {
      return ( InValue < Min ? Max : InValue ); // NRVO
    };

  public:
    /**
      * \brief Wraps a number to the range [min, max].
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return int32 - The resulting wrapped value.
      */
    static uint8 WrapByte_Max( uint8 InValue, uint8 Min, uint8 Max ) NoExcept;

    /**
      * \brief Wraps a number to the range [min, max].
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return int32 - The resulting wrapped value.
      */
    static int32 WrapInt_Max( int32 InValue, int32 Min, int32 Max ) NoExcept;

    /**
      * \brief Wraps a number to the range [min, max].
      *
      * \param InValue   - The value to wrap.
      * \param Min       - The min for Value.
      * \param Max       - The max for Value.
      * \param Precision - The required precision for wrapping.
      *
      * \return void
      */
    static float WrapFloat_Max( float InValue, float Min, float Max, uint8 Precision = 2 ) NoExcept;

    /**
      * \brief Wraps a number to the range [min, max].
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return Size_T - The resulting wrapped value.
      *
      * NOTE! This is not a blueprintable function!
      */
    static Size_T WrapSize_Max( Size_T InValue, Size_T Min, Size_T Max ) NoExcept;

  public:
    /**
      * \brief Wraps a number to max.
      *
      * \tparam ValueType - The Type to wrap.
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return ValueType - The resulting wrapped value.
      *
      * Does not take into account how far away Value is from Max.
      * You can think of it as an Inverse Clamp, Val > Max -> Val = Min.
      */
    static uint8 QuickWrapByte_Max( uint8 InValue, uint8 Min, uint8 Max ) NoExcept;

    /**
      * \brief Wraps a number to min.
      *
      * \tparam ValueType - The Type to wrap.
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return ValueType - The resulting wrapped value.
      *
      * Does not take into account how far away Value is from Max.
      * You can think of it as an Inverse Clamp, Val > Max -> Val = Min.
      */
    static int32 QuickWrapInt_Max( int32 InValue, int32 Min, int32 Max ) NoExcept;

    /**
      * \brief Wraps a number to min.
      *
      * \tparam ValueType - The Type to wrap.
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return ValueType - The resulting wrapped value.
      *
      * Does not take into account how far away Value is from Max.
      * You can think of it as an Inverse Clamp, Val > Max -> Val = Min.
      */
    static float QuickWrapFloat_Max( float InValue, float Min, float Max ) NoExcept;

    /**
      * \brief Wraps a number to min.
      *
      * \tparam ValueType - The Type to wrap.
      *
      * \param InValue  - The value to wrap.
      * \param Min      - The min for Value.
      * \param Max      - The max for Value.
      *
      * \return ValueType - The resulting wrapped value.
      *
      * Does not take into account how far away Value is from Max.
      * You can think of it as an Inverse Clamp, Val > Max -> Val = Min.
      *
      * NOTE! This is not a blueprintable function!
      */
    template< typename ValueType >
    static constexpr ValueType QuickWrap_Max( const ValueType InValue, const ValueType Min, const ValueType Max ) NoExcept
    {
      return ( InValue > Max ? Min : InValue ); // NRVO
    };
};
