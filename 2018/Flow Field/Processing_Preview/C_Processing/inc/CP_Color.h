//------------------------------------------------------------------------------
//   file:	CP_Color.h
// author:	Daniel Hamilton
//   date:  5/17/2018
//  brief:	Create and edit colors
// 
// Copyright © 2018 DigiPen, All rights reserved.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "CP_Export.h"

//------------------------------------------------------------------------------
// Public Structures:
//------------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#endif

// Create and change colors as an array or accessing r, g, b, a individually
struct PColor
{
    union {
        float rgba[4];
        struct { float r, g, b, a; };
    };
};
typedef struct PColor PColor;

//------------------------------------------------------------------------------
// Public Enums:
//------------------------------------------------------------------------------

// Color modes, not currently supported
enum ColorMode
{
    RGB = 0, // Red, Green, Blue mode
    HSB,     // Hue, Saturation, Brightness mode (NOT SUPPORTED)

    // tracking
    CM_MAX
};
typedef enum ColorMode ColorMode;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Create a color given RGBA values
DLL_EXPORT PColor color(int r, int g, int b, int a);

// TODO: Check if this works
// Get the hue value [0, 255] of the provided color
DLL_EXPORT int hue(PColor c);

// Get the red value of the given color
DLL_EXPORT int red(PColor c);

// Get the green value of the given color
DLL_EXPORT int green(PColor c);

// Get the blue value of the given color
DLL_EXPORT int blue(PColor c);

// Get the alpha value of the given color
DLL_EXPORT int alpha(PColor c);

// Linearly Interpolate between two colors given a t value [0, 1]
// 0.0 -> Returns 100% color a
// 1.0 -> Returns 100% color b
// 0.5 -> Returns 50% a 50% b
DLL_EXPORT PColor lerpColor(PColor a, PColor b, float t);


////////////////////////////////////////////////
// --NOT SUPPORTED--
// DLL_EXPORT int saturation(Color c);
// DLL_EXPORT int brightness(Color c);
// DLL_EXPORT void colorMode(ColorMode mode);
////////////////////////////////////////////////