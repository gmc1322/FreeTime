/*!------------------------------------------------------------------------------
\file   CheckWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

typedef class BehaviorTreeWindow_Check *BehaviorTreeWindow_CheckPtr;
typedef class BehaviorTreeWindow_Check const * const BehaviorTreeWindow_CheckDoubleConstPtr;

class BehaviorTreeWindow_Check : public BehaviorTreeWindow_Level2
{
  public:
    BehaviorTreeWindow_Check( size_t ID = 0 ) noexcept;

    ~BehaviorTreeWindow_Check() override = default;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables Sizes;
};
