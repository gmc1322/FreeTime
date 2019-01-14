//------------------------------------------------------------------------------
//   file:	CP_Image.h
// author:	Daniel Hamilton
//   date:  5/31/2018
//  brief:	API for loading and displaying images
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

typedef struct PImage_struct* PImage;
typedef enum ShapeMode ShapeMode;

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Load the given image from a filepath and return a filepath
DLL_EXPORT PImage loadImage(const char * filepath);                       

// Draw an image at a specific point (x, y) with a size (stretched)
DLL_EXPORT void   image(PImage img, float x, float y, float w, float h);                   

// Draw an image and also the alpha with [0, 1.0] range
DLL_EXPORT void   imageAlpha(PImage img, float x, float y, float w, float h, float alpha); 

// Fill out two variables with the width and height of a given image
DLL_EXPORT void   imageSize(PImage img, int* w, int* h);

// How to draw the image (drawn similar to a rectangle)
DLL_EXPORT void   imageMode(ShapeMode mode);                                              
