//------------------------------------------------------------------------------
//   file:	CP_Stub.h
// author:	Daniel Hamilton
//   date:  5/17/2018
//  brief:	Math functions for use in the processing environment
//
// Copyright © 2018 DigiPen, All rights reserved.
//------------------------------------------------------------------------------

#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "CP_Export.h"
#include <math.h>

//------------------------------------------------------------------------------
// Defines:
//------------------------------------------------------------------------------

// Math defines
#define TAU        6.28318530717958647692f
#define TWO_PI     6.28318530717958647692f
#define PI         3.14159265358979323846f
#define HALF_PI    1.57079632679489661923f
#define QUARTER_PI 0.785398163397448309616f

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// clamp a value to a range
DLL_EXPORT float constrain_f(float value, float min, float max); // float
DLL_EXPORT int   constrain_i(int value, int min, int max);       // int

// distance between two points
DLL_EXPORT float dist_f(float x1, float y1, float x2, float y2); // float
DLL_EXPORT float dist_i(int x1, int y1, int x2, int y2);         // int

// linear interpolation between two values (a, b) given a 
// factor on the interval: [0.0, 1.0]
DLL_EXPORT float lerp_f(float a, float b, float lerp_factor); // float
DLL_EXPORT int   lerp_i(int a, int b, float lerp_factor);     // int

// magnitude of a vector
DLL_EXPORT float mag(float x, float y);

// re-maps a value from one range to another
// For example, the number 25 is converted from a value in 
// the range of 0 to 100 into a value that ranges from the 
// left edge of the window (0) to the right edge (width).
DLL_EXPORT float map(float value, float min_old, float max_old, float min_new, float max_new);

// squares a number, returning: value * value
DLL_EXPORT float sq(float value);

// Convert radians to degrees
DLL_EXPORT float degrees(float radians);

// Convert degrees to radians
DLL_EXPORT float radians(float degrees);


/////////////////////////////////////
//   MATRIX AND VECTOR FUCNTIONS   //
/////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)  // nonstandard extension used : nameless struct/union
#pragma warning(push)
#pragma warning(disable: 4204)  // nonstandard extension used : non-constant aggregate initializer
#endif

// 2D vector structure
typedef struct { float x, y; } vec2_t;

// 3D matrix structure represented as individual floats or as an array to index
typedef union {
    float m[3][3];
    struct {
        float m00, m01, m02;
        float m10, m11, m12;
        float m20, m21, m22;
    };
} mat3_t;

DLL_EXPORT vec2_t vec2();          // fill out default values
DLL_EXPORT mat3_t mat3();          // fill out default values
DLL_EXPORT mat3_t mat3_identity(); // identity matrix


// Create vector or matrix from a set of values
DLL_EXPORT mat3_t mat3_set(
    float m00, float m01, float m02,
    float m10, float m11, float m12,
    float m20, float m21, float m22
);

// Create a matrix from column vectors
DLL_EXPORT mat3_t mat3_from_vector(vec2_t col1, vec2_t col2, vec2_t col3);


// Matrix Operations
DLL_EXPORT mat3_t mat3_scale(vec2_t scale);               // compute the matrix for a given scale
DLL_EXPORT mat3_t mat3_translate(vec2_t offset);          // compute the matrix for a given translation
DLL_EXPORT mat3_t mat3_rotate(float degrees);             // rotate the matrix (using degrees)
DLL_EXPORT mat3_t mat3_rotate_rad(float radians);         // rotate the matrix (using radians)
DLL_EXPORT mat3_t mat3_transpose(mat3_t original);        // transpose the matrix
DLL_EXPORT mat3_t mat3_inverse(mat3_t original);          // invert the matrix
DLL_EXPORT mat3_t mat3_concat(mat3_t a, mat3_t b);        // a * b in this order
DLL_EXPORT vec2_t mat3_mult_vec2(mat3_t mat, vec2_t vec); // Multiplies a point by a matrix


// Vector Operations
DLL_EXPORT vec2_t vec2_zero();                         // Sets the vector to <0, 0>
DLL_EXPORT vec2_t vec2_set(float x, float y);          // Sets the vector to these values
DLL_EXPORT vec2_t vec2_neg(vec2_t vec);                // negates all values in the vector
DLL_EXPORT vec2_t vec2_add(vec2_t a, vec2_t b);        // a + b
DLL_EXPORT vec2_t vec2_sub(vec2_t a, vec2_t b);        // a - b
DLL_EXPORT vec2_t vec2_norm(vec2_t vec);               // normalize the vector (convert it to a unit vector)
DLL_EXPORT vec2_t vec2_scale(vec2_t vec, float scale); // scale the vector by a given value
DLL_EXPORT float  vec2_length(vec2_t vec);             // length (magnitude) of the vector
DLL_EXPORT float  vec2_distance(vec2_t a, vec2_t b);   // distance between two points
DLL_EXPORT float  vec2_dot(vec2_t a, vec2_t b);        // returns the dot product of two vectors
DLL_EXPORT float  vec2_cross(vec2_t a, vec2_t b);      // cross product of two vectors
DLL_EXPORT float  vec2_angle(vec2_t a, vec2_t b);      // angle (in degrees) between two vectors



#ifdef _MSC_VER
#pragma warning(pop)
#pragma warning(pop)
#endif