//---------------------------------------------------------
// file:	CP_Random.h
// author:	Justin Chambers
// brief:	Header for all random helper functions
//
//	DO NOT INCLUDE THIS HEADER IN YOUR PROJECTS DIRECTLY
//	SEE: C_Processing.h
//
// Copyright © 2018 DigiPen, All rights reserved.
//---------------------------------------------------------

#pragma once

#include "CP_Export.h"
#include <stdint.h>

//-----------------------------------------------
// randomSeed()
//
// Sets the seed value for random().
// By default, random() produces different results each time the program is run.
// Set the seed parameter to a constant to return the same pseudo - random numbers each time the software is run.
DLL_EXPORT void randomSeed(int seed);

// random - returns a value in the range [0, 1.0]
DLL_EXPORT double random();

// randomRange - returns a value in the range [min, max]
DLL_EXPORT double randomRange(double min, double max);

// randomInt - returns a value in the range [0, MAX_RAND_INT]
DLL_EXPORT unsigned int randomInt();

// randomRangeInt - returns a value in the range [min, max]
DLL_EXPORT unsigned int randomRangeInt(unsigned int min, unsigned int max);

// randomGaussian - returns a normally distributed value where the mean is 0 and the standard deviation is 1.0
DLL_EXPORT double randomGaussian();



