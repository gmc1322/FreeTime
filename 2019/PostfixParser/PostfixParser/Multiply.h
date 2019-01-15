/*!------------------------------------------------------------------------------
\file   Multiply.h

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#pragma once

#include "BinaryOperator.h"

class Multiply : public BinaryOperator
{
  public:
    virtual ~Multiply() noexcept = default;

  public:
    float operator()( const float &Lhs, const float &Rhs ) noexcept override;
};
