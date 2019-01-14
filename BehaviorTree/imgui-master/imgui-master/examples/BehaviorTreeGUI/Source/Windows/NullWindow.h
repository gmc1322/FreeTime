/*!------------------------------------------------------------------------------
\file   NullWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright
------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

typedef class BehaviorTreeWindow_Null *BehaviorTreeWindow_NullPtr;
typedef class BehaviorTreeWindow_Null const * const BehaviorTreeWindow_NullDoubleConstPtr;

class BehaviorTreeWindow_Null : public BehaviorTreeWindow_Base
{
  public:
    BehaviorTreeWindow_Null( size_t ID = 0 ) noexcept;

    ~BehaviorTreeWindow_Null() override = default;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables Sizes;
};
