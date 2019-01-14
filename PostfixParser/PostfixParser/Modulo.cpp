/*!------------------------------------------------------------------------------
\file   Modulo.cpp

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#include "Modulo.h"

#include <cmath>

float Modulo::operator()( const float &Lhs, const float &Rhs ) noexcept
{
  return std::fmodf( Lhs, Rhs );
}
