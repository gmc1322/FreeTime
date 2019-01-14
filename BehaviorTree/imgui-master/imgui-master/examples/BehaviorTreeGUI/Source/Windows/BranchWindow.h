/*!------------------------------------------------------------------------------
\file   BranchWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

typedef class BehaviorTreeWindow_Branch *BehaviorTreeWindow_BranchPtr;
typedef class BehaviorTreeWindow_Branch const * const BehaviorTreeWindow_BranchDoubleConstPtr;

class BehaviorTreeWindow_Branch : public BehaviorTreeWindow_Level2
{
  public:
    BehaviorTreeWindow_Branch( size_t ID = 0 ) noexcept;

    ~BehaviorTreeWindow_Branch() override = default;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables Sizes;
};
