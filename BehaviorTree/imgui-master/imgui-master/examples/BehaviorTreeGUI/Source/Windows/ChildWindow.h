/*!------------------------------------------------------------------------------
\file   ChildWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

typedef class BehaviorTreeWindow_Child *BehaviorTreeWindow_ChildPtr;
typedef class BehaviorTreeWindow_Child const * const BehaviorTreeWindow_ChildDoubleConstPtr;

class BehaviorTreeWindow_Child : public BehaviorTreeWindow_Level1
{
  public:
    BehaviorTreeWindow_Child( size_t ID = 0 ) noexcept;

    ~BehaviorTreeWindow_Child() override = default;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables Sizes;
};
