/*!------------------------------------------------------------------------------
\file   TrunkWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

typedef class BehaviorTreeWindow_Trunk *BehaviorTreeWindow_TrunkPtr;
typedef class BehaviorTreeWindow_Trunk const * const BehaviorTreeWindow_TrunkDoubleConstPtr;

class BehaviorTreeWindow_Trunk : public BehaviorTreeWindow_Level2
{
  public: 
    BehaviorTreeWindow_Trunk() noexcept;

    ~BehaviorTreeWindow_Trunk() override = default;

  public:
    const char * GetOnFinishName() const noexcept;
    const char * GetOnResetName() const noexcept;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  public:
    void SetOnFinish( const std::string &Text ) const noexcept;
    void SetOnReset( const std::string &Text ) const noexcept;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables Sizes;

  private:
    Input OnFinish;

    Input OnReset;
};
