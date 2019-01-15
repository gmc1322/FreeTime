/*!------------------------------------------------------------------------------
\file   PlayerWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

#include "Switch.h"
#include "MultiInput.h"
#include "NPCWindow.h"

class KarmaTreeWindow_Player : public KarmaTreeWindow_Base
{
  public:
    enum ResponseTypes
    {
      Response, // The regular dialogue.
      Exit,     // End dialogue.
      Event,    // End dialogue and start something else.
    };

  public:
    KarmaTreeWindow_Player( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, size_t ParentID ) noexcept;

    ~KarmaTreeWindow_Player() override;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

    KarmaTreeWindow_NPCDoubleConstPtr GetChild() const noexcept;

    const char * GetText() const noexcept;

    const char * GetResponseType() const;

    const char * GetKarma() const;

    const char * GetKeys() const;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables DefaultSize;
    static Static_NoInstance_Variables ExtraSize;
    static Static_NoInstance_Variables ExpandedSizes;

  private:
    KarmaTreeWindow_NPCPtr Child = nullptr;

    MultiInput PlayerText;

    bool BigText = false;

    Switch ResponseType;

    bool MoreSize = false;

    Input CustomResponseNumber;

    Input RequiredKarma;
    MultiInput RequiredKeys;

    ImVec2 TempLocation = { 0,0 };
};
