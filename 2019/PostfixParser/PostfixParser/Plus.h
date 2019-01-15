/*!------------------------------------------------------------------------------
\file   Plus.h

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#pragma once

#include "BinaryOperator.h"

class Plus : public BinaryOperator
{
  public:
    virtual ~Plus() noexcept = default;

  public:
    float operator()( const float &Lhs, const float &Rhs ) noexcept override;
};
