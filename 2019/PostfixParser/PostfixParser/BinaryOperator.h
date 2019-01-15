/*!------------------------------------------------------------------------------
\file   BinaryOperator.h

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#pragma once

class BinaryOperator
{
  public:
    virtual ~BinaryOperator() noexcept = default;

  public:
    virtual float operator()( const float &Lhs, const float &Rhs ) noexcept = 0;
};
