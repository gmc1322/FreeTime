/*!------------------------------------------------------------------------------
\file   TreeWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

typedef class BehaviorTreeWindow_Tree *BehaviorTreeWindow_TreePtr;
typedef class BehaviorTreeWindow_Tree const * const BehaviorTreeWindow_TreeDoubleConstPtr;

class BehaviorTreeWindow_Tree : public BehaviorTreeWindow_Level1
{
  public:
    BehaviorTreeWindow_Tree( size_t ID = 0 ) noexcept;

    ~BehaviorTreeWindow_Tree() override = default;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables Sizes;
};
