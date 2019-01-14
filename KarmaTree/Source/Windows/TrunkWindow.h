/*!------------------------------------------------------------------------------
\file   NPCWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

#include "MultiInput.h"
#include "Button.h"

typedef class KarmaTreeWindow_Trunk *KarmaTreeWindow_TrunkPtr;
typedef class KarmaTreeWindow_Trunk const * const KarmaTreeWindow_TrunkDoubleConstPtr;

class KarmaTreeWindow_Trunk : public KarmaTreeWindow_Base
{
  public: 
    KarmaTreeWindow_Trunk() noexcept;
    ~KarmaTreeWindow_Trunk() override;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

    const char * GetNPCText() const noexcept;

    const char * GetTreeName() const noexcept;

    const std::map< size_t, KarmaTreeWindow_PlayerPtr > & GetChildren() const noexcept;

    const char * GetBadThreshold() const noexcept;
    const char * GetGoodThreshold() const noexcept;

  private:
    void RunWindow() noexcept override;

  private:
    static void AddResponseWindow( void *This ) noexcept;
    static void RemoveResponseWindow( void *This ) noexcept;

  private:
    static Static_NoInstance_Variables Sizes;
    static Static_NoInstance_Variables ExpandedSizes;

    static constexpr size_t Padding = 10;

  private:
    std::map< size_t, KarmaTreeWindow_PlayerPtr > ChildWindows;

    Input TreeName;

    MultiInput NPCText;

    Input BadKarmaThreshold;
    Input GoodKarmaThreshold;

    Button AddResponse;
    Button RemoveResponse;

    bool BigText = false;

    ImVec2 TempLocation = { 0,0 };
};
