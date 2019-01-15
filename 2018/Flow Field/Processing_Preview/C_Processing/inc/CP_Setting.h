//------------------------------------------------------------------------------
//   file:	CP_Setting.h
// author:	Daniel Hamilton
//   date:  5/20/2018
//  brief:	Settings header to lay out all states (colors, fill, background, 
//          frame, cursor, fullscreen, etc)
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

// Forward declaraction
typedef struct PColor PColor;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Set the background with a color or by specifying RGBA
DLL_EXPORT void background(int red, int green, int blue, int alpha);
DLL_EXPORT void backgroundColor(PColor c);

// Specify the fill color of any drawn shape
DLL_EXPORT void fill(int red, int green, int blue, int alpha);
DLL_EXPORT void fillColor(PColor c);

// Don't fill any shapes
DLL_EXPORT void noFill();

// Set the stroke color using a provided Color or RGBA
DLL_EXPORT void stroke(int red, int green, int blue, int alpha);
DLL_EXPORT void strokeColor(PColor c);

// Don't draw stroke on any shape
DLL_EXPORT void noStroke();

// Specify the width of the stroke
DLL_EXPORT void strokeWeight(float weight);

// Set the size of the canvas/window
DLL_EXPORT void size(int new_width, int new_height);

// Set the window fullscreen
DLL_EXPORT void fullscreen();

// Specify whether to show the cursor or not
DLL_EXPORT void cursor();
DLL_EXPORT void noCursor();


//////////////////////////////////////////////
//             Camera Controls              //
//////////////////////////////////////////////

// Build your own matrix to apply it as the camera view matrix
DLL_EXPORT void applyMatrix(mat3_t matrix);

// Reset the camera to default
DLL_EXPORT void resetMatrix();

// Scale the canvas
DLL_EXPORT void scale(float scalar);
DLL_EXPORT void scaleX(float x_scale);
DLL_EXPORT void scaleY(float y_scale);

// Rotate the canvas
DLL_EXPORT void rotate(float degrees);

// Translate the canvas
DLL_EXPORT void translate(float x, float y);

// Convert coordinates from screen to world
DLL_EXPORT void screenToWorld(float* x, float* y);

// Convert coordinates from world to screen
DLL_EXPORT void worldToScreen(float* x, float* y);


//////////////////////////////////////////////
//                 Timing                   //
//////////////////////////////////////////////

// The framerate in frames per second
DLL_EXPORT float getframeRate();
#define frameRate getframeRate()

// Set the framerate
DLL_EXPORT void setFrameRate(float fps);

// The time between frames
DLL_EXPORT float dt();

// The total elapsed time since the start of the program in milliseconds 
DLL_EXPORT float millis();

// The total elapsed time since the start of the program in seconds 
DLL_EXPORT float second();