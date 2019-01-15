/*!------------------------------------------------------------------------------
\file   Exponent.cpp

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#include "Exponent.h"

#include <cmath>

float Exponent::operator()( const float &Lhs, const float &Rhs ) noexcept
{
  return std::powf( Lhs, Rhs );
}
