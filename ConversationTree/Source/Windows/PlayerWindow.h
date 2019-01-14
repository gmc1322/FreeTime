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

class ConversationTreeWindow_Player : public ConversationTreeWindow_Base
{
  public:
    enum ResponseTypes
    {
      Response, // The regular dialogue.
      Exit,     // End dialogue.
      Event,    // End dialogue and start something else.
    };

  public:
    ConversationTreeWindow_Player( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, size_t ParentID ) noexcept;

    ~ConversationTreeWindow_Player() override;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

    ConversationTreeWindow_NPCDoubleConstPtr GetChild() const noexcept;

    const char * GetText() const noexcept;

    const char * GetResponseType() const;

  public:
    void SetLocation( const ImVec2 &Loc ) noexcept override;
  
  public:
    int CheckPosition( const std::map< size_t, ConversationTreeWindow_BasePtr > &AllWindows ) noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables DefaultSize;
    static Static_NoInstance_Variables ExtraSize;
    static Static_NoInstance_Variables ExpandedSizes;

  private:
    ConversationTreeWindow_NPCPtr Child = nullptr;

    MultiInput PlayerText;

    bool BigText = false;

    Switch ResponseType;

    bool MoreSize = false;

    Input CustomResponseNumber;

    ImVec2 TempLocation = { 0,0 };

};
