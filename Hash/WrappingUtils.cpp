/*!------------------------------------------------------------------------------
\file   RandUtils.cpp

\author Garrett Conti

\par    Project: Golems_Magic
\par    Course:  SUMMER
------------------------------------------------------------------------------ */

#include "Public/Utils/WrappingUtils.h"

#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 UWrappingUtils::WrapByte( const uint8 InValue, const uint8 Min, const uint8 Max ) NoExcept
{
  return static_cast< uint8 >( WrapInt( InValue, Min, Max ) ); // RVO
}

int32 UWrappingUtils::WrapInt( int32 InValue, const int32 Min, const int32 Max ) NoExcept
{
  if( InValue < Min )
  {
    const int32 Range = Max - Min + 1;

    InValue = Min + ( Range * ( std::lround( static_cast< float >( Min - InValue ) / Range ) + 1 ) + InValue - Min ) % Range;
  }
  else if( InValue > Max )
  {
    InValue = Min + ( InValue - Min ) % ( Max - Min + 1 );
  }

  return InValue; // NRVO
}

float UWrappingUtils::WrapFloat( float InValue, const float Min, const float Max, const uint8 Precision ) NoExcept
{
  if( InValue < Min )
  {
    int32 PrecisionMulti = 10;

    for( uint8 i = 1; i < Precision; ++i )
    {
      PrecisionMulti *= 10;
    }

    InValue = static_cast< float >( WrapInt( std::lround( InValue * PrecisionMulti + 1 + PrecisionMulti ),
                                             std::lround( Min * PrecisionMulti ),
                                             std::lround( Max * PrecisionMulti ) ) ) / PrecisionMulti;
  }
  else if( InValue > Max )
  {
    int32 PrecisionMulti = 10;

    for( uint8 i = 1; i < Precision; ++i )
    {
      PrecisionMulti *= 10;
    }

    InValue = static_cast< float >( WrapInt( std::lround( InValue * PrecisionMulti - 1 - PrecisionMulti ),
                                             std::lround( Min * PrecisionMulti ),
                                             std::lround( Max * PrecisionMulti ) ) ) / PrecisionMulti;
  }

  return InValue; // NRVO
}

Size_T UWrappingUtils::WrapSize( Size_T InValue, const Size_T Min, const Size_T Max ) NoExcept
{
  // This only works becouse there are no negitive/positive number combinations

  if( InValue < Min )
  {
    const Size_T MaxPlusOne = Max + 1;

    do
    {
      InValue = Max - InValue - MaxPlusOne;
    }
    while( InValue < Min );
  }
  else if( InValue > Max )
  {
    const Size_T MaxPlusOne = Max + 1;

    do
    {
      InValue = Min + InValue - MaxPlusOne;
    }
    while( InValue > Max );
  }

  return InValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 UWrappingUtils::QuickWrapByte( const uint8 InValue, const uint8 Min, const uint8 Max ) NoExcept
{
  return QuickWrap( InValue, Min, Max ); // RVO
}

int32 UWrappingUtils::QuickWrapInt( const int32 InValue, const int32 Min, const int32 Max ) NoExcept
{
  return QuickWrap( InValue, Min, Max ); // RVO
}

float UWrappingUtils::QuickWrapFloat( const float InValue, const float Min, const float Max ) NoExcept
{
  return QuickWrap( InValue, Min, Max ); // RVO
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 UWrappingUtils::WrapByte_Min( const uint8 InValue, const uint8 Min, const uint8 Max ) NoExcept
{
  return static_cast< uint8 >( WrapInt_Min( InValue, Min, Max ) ); // RVO
}

int32 UWrappingUtils::WrapInt_Min( int32 InValue, const int32 Min, const int32 Max ) NoExcept
{
  if( InValue < Min )
  {
    const int32 Range = Max - Min + 1;

    InValue = Min + ( Range * ( std::lround( static_cast< float >( Min - InValue ) / Range ) + 1 ) + InValue - Min ) % Range;
  }

  return InValue; // NRVO
}

float UWrappingUtils::WrapFloat_Min( float InValue, const float Min, const float Max, const uint8 Precision ) NoExcept
{
  if( InValue < Min )
  {
    int32 PrecisionMulti = 10;

    for( uint8 i = 1; i < Precision; ++i )
    {
      PrecisionMulti *= 10;
    }

    InValue = static_cast< float >( WrapInt_Min( std::lround( InValue * PrecisionMulti + 1 + PrecisionMulti ),
                                                 std::lround( Min * PrecisionMulti ),
                                                 std::lround( Max * PrecisionMulti ) ) ) / PrecisionMulti;
  }

  return InValue; // NRVO
}

Size_T UWrappingUtils::WrapSize_Min( Size_T InValue, const Size_T Min, const Size_T Max ) NoExcept
{
  // This only works becouse there are no negitive/positive number combinations

  if( InValue < Min )
  {
    const Size_T MaxPlusOne = Max + 1;

    do
    {
      InValue = Max - InValue - MaxPlusOne;
    }
    while( InValue < Min );
  }

  return InValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 UWrappingUtils::QuickWrapByte_Min( const uint8 InValue, const uint8 Min, const uint8 Max ) NoExcept
{
  return QuickWrap_Min( InValue, Min, Max ); // RVO
}

int32 UWrappingUtils::QuickWrapInt_Min( const int32 InValue, const int32 Min, const int32 Max ) NoExcept
{
  return QuickWrap_Min( InValue, Min, Max ); // RVO
}

float UWrappingUtils::QuickWrapFloat_Min( const float InValue, const float Min, const float Max ) NoExcept
{
  return QuickWrap_Min( InValue, Min, Max ); // RVO
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 UWrappingUtils::WrapByte_Max( const uint8 InValue, const uint8 Min, const uint8 Max ) NoExcept
{
  return static_cast< uint8 >( WrapInt_Max( InValue, Min, Max ) ); // RVO
}

int32 UWrappingUtils::WrapInt_Max( int32 InValue, const int32 Min, const int32 Max ) NoExcept
{
  if( InValue > Max )
  {
    InValue = Min + ( InValue - Min ) % ( Max - Min + 1 );
  }

  return InValue; // NRVO
}

float UWrappingUtils::WrapFloat_Max( float InValue, const float Min, const float Max, const uint8 Precision ) NoExcept
{
  if( InValue > Max )
  {
    int32 PrecisionMulti = 10;

    for( uint8 i = 1; i < Precision; ++i )
    {
      PrecisionMulti *= 10;
    }

    InValue = static_cast< float >( WrapInt_Max( std::lround( InValue * PrecisionMulti - 1 - PrecisionMulti ),
                                                 std::lround( Min * PrecisionMulti ),
                                                 std::lround( Max * PrecisionMulti ) ) ) / PrecisionMulti;
  }

  return InValue; // NRVO
}

Size_T UWrappingUtils::WrapSize_Max( Size_T InValue, const Size_T Min, const Size_T Max ) NoExcept
{
  // This only works becouse there are no negitive/positive number combinations

  if( InValue > Max )
  {
    const Size_T MaxPlusOne = Max + 1;

    do
    {
      InValue = Min + InValue - MaxPlusOne;
    }
    while( InValue > Max );
  }

  return InValue;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8 UWrappingUtils::QuickWrapByte_Max( const uint8 InValue, const uint8 Min, const uint8 Max ) NoExcept
{
  return QuickWrap_Max( InValue, Min, Max ); // RVO
}

int32 UWrappingUtils::QuickWrapInt_Max( const int32 InValue, const int32 Min, const int32 Max ) NoExcept
{
  return QuickWrap_Max( InValue, Min, Max ); // RVO
}

float UWrappingUtils::QuickWrapFloat_Max( const float InValue, const float Min, const float Max ) NoExcept
{
  return QuickWrap_Max( InValue, Min, Max ); // RVO
}
