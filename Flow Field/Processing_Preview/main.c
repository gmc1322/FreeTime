//---------------------------------------------------------
// file:	main.c
// author:	Justin Chambers
// brief:	Main entry point for the sample usage project
//			of the C_Processing library
//
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

// system headers
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// C Procesing header
#include "C_Processing.h"

// DEFS
#define WindowSizeX 800
#define WindowSizeY 600

#define DotXY( Vec2 ) Vec2.x, Vec2.y

// STRUCTS
typedef struct FlowFieldPointData
{
  vec2_t Point;
  float  Strength;
}FlowFieldPointData;

typedef struct FlowFieldVectorData
{
  FlowFieldPointData StartPoint;
  FlowFieldPointData EndPoint;
}FlowFieldVectorData;

typedef struct FlowFieldData
{
  vec2_t Scale;

  // 0 -> 359, ( x >= 360 ) = no angle.
  // A start or an end is required.
  // The Start is a Source with inf Strength.
  // The End is a Dest with inf Strength
  struct
  {
    unsigned StartAngle : 9; // 9 bits = max 511
    unsigned EndAngle   : 9;
  };

  FlowFieldPointData *SourcePoints; // Flow goes away from point
  size_t SourceCount;

  FlowFieldPointData *DestinationPoints; // Flow goes towards point
  size_t DestinationCount;

  FlowFieldVectorData *VectorPoints; // Flow goes in the direction of the points
  size_t VectorCount;
}FlowFieldData;

// FILE GLOBS
static const float WindowCenterX = WindowSizeX / 2.0f;
static const float WindowCenterY = WindowSizeY / 2.0f;

//Static Funcs

static float UnitCircleToRect( const double Angle, const float RectLength ) 
{
  const float BetterAngle = ( float )( int )( Angle * 10'000 );
  const float SqrtTwo     = ( float )( int )( ( 1.414213562373095 / 2.0 ) * 10'000 );

  return ( ( BetterAngle > SqrtTwo || BetterAngle < -SqrtTwo ) ? RectLength :
           map( BetterAngle, -SqrtTwo, SqrtTwo, -RectLength, RectLength ) );
}

static float absf( const float Value ) { return ( Value < 0 ? -Value : Value ); }

static vec2_t RotatePoint( const vec2_t RotPoint, vec2_t Point, const float Degrees )
{
  const float SinAngle = sinf( Degrees );
  const float CosAngle = cosf( Degrees );

  Point.x -= RotPoint.x;
  Point.y -= RotPoint.y;

  {
    float TempX = Point.x * CosAngle - Point.y * SinAngle;
    Point.y = Point.x * SinAngle + Point.y * CosAngle;
    Point.x = TempX;
  }

  Point.x += RotPoint.x;
  Point.y += RotPoint.y;

  return Point;
}

static void ArrowMesh( vec2_t Size, vec2_t Position, float Degrees )
{
  beginShape();

  vec2_t Vert;

  {
    Vert.x = Position.x + Size.x * 0;
    Vert.y = Position.y + Size.y * -2.0f;

    Vert = RotatePoint( Position, Vert, Degrees );

    vertex( DotXY( Vert ) );
  }

  {
    Vert.x = Position.x + Size.x * 0.5f;
    Vert.y = Position.y + Size.y * 0.5f;

    Vert = RotatePoint( Position, Vert, Degrees );

    vertex( DotXY( Vert ) );
  }

  {
    Vert.x = Position.x + Size.x * -0.5f;
    Vert.y = Position.y + Size.y * 0.5f;

    Vert = RotatePoint( Position, Vert, Degrees );

    vertex( DotXY( Vert ) );
  }

  endShape();
}

static void FlowPass( vec2_t StartPoint, const vec2_t FlowPoint, const float HardRadius, const float SoftRadius, const float Width,
                      const unsigned XAmount, const unsigned YAmount, const vec2_t DoubleScale, const float ThreeFourthsScaleX, float *const Rads,
                      BOOL InverseFlow )
{
  for( unsigned i = 0; ; )
  {
    const unsigned YIndex = ( unsigned )( i * XAmount );

    for( unsigned j = 0; ; )
    {
      {
        float Rad;
        float Strength;

        {
          vec2_t Distance;
          Distance.x = StartPoint.x - FlowPoint.x;
          Distance.y = StartPoint.y - FlowPoint.y;

          Rad      = atan2f( Distance.y, Distance.x ) - PI / 2.0f;
          Strength = 1.0f / constrain_f( ( vec2_length( Distance ) - HardRadius ) / SoftRadius, 1.0f, Width );
        }

        if( Rad < -PI && Rad > -3.0f * PI / 2.0f ) { Rad += 2.0f * PI; }

        if( InverseFlow ) { Rad += PI; }

        {
          float *const RadPtr = Rads + YIndex + j;

          const float CurrentRad = *RadPtr;

          // Account for lerp being linear and not circular
          if( CurrentRad - Rad > PI )
          {
            Rad += 2.0f * PI;
          }

          *RadPtr = lerp_f( CurrentRad, Rad, Strength );
        }
      }

      if( ++j < XAmount )
      {
        StartPoint.x += DoubleScale.x;
      }
      else break;
    }

    if( ++i < YAmount )
    {
      StartPoint.x = ThreeFourthsScaleX;
      StartPoint.y += DoubleScale.y;
    }
    else break;
  }
}

static float * FlowFieldGenerator( const FlowFieldData *const DataPtr )
{
  const vec2_t Scale = DataPtr->Scale;

  vec2_t DoubleScale = Scale;

  const float ThreeFourthsScaleX = DoubleScale.x / 4.0f * 3.0f;

  DoubleScale.x *= 2.0f;
  DoubleScale.y *= 2.0f;

  const float Width  = ( float )width;
  const float Height = ( float )height;

  const unsigned XAmount = ( unsigned )( Width  / DoubleScale.x );
  const unsigned YAmount = ( unsigned )( Height / DoubleScale.y );

  const float HardRadius = Scale.x * 2.0f;


  float *Rads = calloc( YAmount * XAmount, sizeof( float ) );

  vec2_t Point;
  Point.x = ThreeFourthsScaleX;
  Point.y = Scale.y / 4.0f * 3.0f;

  float Angle = ( float )( DataPtr->StartAngle );


  if( Angle < 360.0f )
  {
    Angle = radians( Angle > 180 ? Angle - 180 : -Angle ); // Negate angle since Y is down

    vec2_t FlowPoint;

    FlowPoint.x = UnitCircleToRect( cosf( Angle ), Width  );
    FlowPoint.y = UnitCircleToRect( sinf( Angle ), Height );

    FlowPass( Point, FlowPoint, Width * Height, 1, Width, XAmount, YAmount, DoubleScale, ThreeFourthsScaleX, Rads, TRUE );

    if( ( Angle = ( float )( DataPtr->EndAngle ) ) < 360.0f )
    {
      Angle = radians( Angle > 180 ? Angle - 180 : -Angle ); // Negate angle since Y is down

      FlowPoint.x = UnitCircleToRect( cosf( Angle ), Width  );
      FlowPoint.y = UnitCircleToRect( sinf( Angle ), Height );

      FlowPass( Point, FlowPoint, HardRadius, 1, Width, XAmount, YAmount, DoubleScale, ThreeFourthsScaleX, Rads, FALSE );
    }
  }
  else if( ( Angle = ( float )( DataPtr->EndAngle ) ) < 360.0f )
  {
    Angle = radians( Angle > 180 ? Angle - 180 : -Angle ); // Negate angle since Y is down

    vec2_t FlowPoint;

    FlowPoint.x = UnitCircleToRect( cos( Angle ), Width  );
    FlowPoint.y = UnitCircleToRect( sin( Angle ), Height );

    FlowPass( Point, FlowPoint, Width * Height, 1, Width, XAmount, YAmount, DoubleScale, ThreeFourthsScaleX, Rads, FALSE );
  }
  
  for( size_t Index = 0, End = DataPtr->DestinationCount; Index < End; ++Index )
  {
    const FlowFieldPointData CurrentPointData = DataPtr->DestinationPoints[ Index ];

    FlowPass( Point, CurrentPointData.Point, HardRadius, HardRadius * CurrentPointData.Strength, Width, XAmount, YAmount, DoubleScale, 
              ThreeFourthsScaleX, Rads, FALSE );
  }

  return Rads;
}

// Other Funcs
static const float * FlowAngles;

// setup() is called once at the beginning of the program
void setup()
{
  size( WindowSizeX, WindowSizeY );
  rectMode( CENTER );
  noStroke();
  fillColor( color( 255, 0, 10, 255 ) );
}

// draw() is called once per frame repeatedly while the program is running
void draw()
{
  background( 255, 255, 255, 255 );

  static BOOL GeneratedOnce = FALSE;

  vec2_t Scale;
  Scale.x = 10.0f;
  Scale.y = 10.0f;

  if( !GeneratedOnce ) // Can't put in setup...
  {
    FlowFieldPointData Test[ 3 ];

    {
      FlowFieldPointData *TestPtr = Test;

      TestPtr->Point.x = TestPtr->Point.y = 100.0f;
      TestPtr->Strength = 1.0f;

      ++TestPtr;

      TestPtr->Point.x = TestPtr->Point.y = 300.0f;
      TestPtr->Strength = 2.0f;

      ++TestPtr;

      TestPtr->Point.x = TestPtr->Point.y = 500.0f;
      TestPtr->Strength = 1.0f;
    }

    FlowFieldData Data;

    Data.Scale = Scale;

    Data.DestinationPoints = Test;
    Data.DestinationCount  = 3; sizeof( Test ) / sizeof( Test[ 0 ] );

    Data.StartAngle = 0;
    Data.EndAngle   = 360;

    FlowAngles = FlowFieldGenerator( &Data );

    GeneratedOnce = TRUE;
  }

  {
    const float ThreeFourthsScaleX = Scale.x / 4.0f * 3.0f;

    vec2_t DoubleScale = Scale;
    DoubleScale.x *= 2.0f;
    DoubleScale.y *= 2.0f;

    const float Width  = ( float )width;
    const float Height = ( float )height;

    const unsigned XAmount = ( unsigned )( Width / DoubleScale.x );
    const unsigned YAmount = ( unsigned )( Height / DoubleScale.y );

    vec2_t Point;
    Point.x = ThreeFourthsScaleX;
    Point.y = Scale.y / 4.0f * 3.0f;

    for( unsigned i = 0, j; ; )
    {
      const unsigned YIndex = i * XAmount;

      for( j = 0; ; )
      {
        ArrowMesh( Scale, Point, FlowAngles[ YIndex + j ] );

        if( ++j < XAmount )
        {
          Point.x += DoubleScale.x;
        }
        else break;
      }

      if( ++i < YAmount )
      {
        Point.x = ThreeFourthsScaleX;
        Point.y += DoubleScale.y;
      }
      else break;
    }
  }
}

// main() the starting point for the program
// Run() is used to tell the program which setup and draw functions to use.
int main( void )
{
  Run( setup, draw );

  free( ( float* )FlowAngles );

  return 0;
}