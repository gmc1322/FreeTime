/*!------------------------------------------------------------------------------
\file   Minus.h

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#pragma once

#include "BinaryOperator.h"

class Minus : public BinaryOperator
{
  public:
    virtual ~Minus() noexcept = default;

  public:
    float operator()( const float &Lhs, const float &Rhs ) noexcept override;
};
