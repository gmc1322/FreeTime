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

typedef class KarmaTreeWindow_NPC *KarmaTreeWindow_NPCPtr;
typedef class KarmaTreeWindow_NPC const * const KarmaTreeWindow_NPCDoubleConstPtr;

class KarmaTreeWindow_NPC : public KarmaTreeWindow_Base
{
  public:
    KarmaTreeWindow_NPC( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, size_t ParentID ) noexcept;
    ~KarmaTreeWindow_NPC() override;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

    const char * GetNPCText() const noexcept;

    const std::map< size_t, KarmaTreeWindow_PlayerPtr > & GetChildren() const noexcept;

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

    MultiInput NPCText;

    Button AddResponse;
    Button RemoveResponse;

    bool BigText = false;

    ImVec2 TempLocation = { 0,0 };
};
