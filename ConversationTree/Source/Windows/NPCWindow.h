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

typedef class ConversationTreeWindow_NPC *ConversationTreeWindow_NPCPtr;
typedef class ConversationTreeWindow_NPC const * const ConversationTreeWindow_NPCDoubleConstPtr;

typedef class ConversationTreeWindow_Player *ConversationTreeWindow_PlayerPtr;
typedef class ConversationTreeWindow_Player const * const ConversationTreeWindow_PlayerDoubleConstPtr;

class ConversationTreeWindow_NPC : public ConversationTreeWindow_Base
{
  public: 
    ConversationTreeWindow_NPC() noexcept;
    ConversationTreeWindow_NPC( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, size_t ParentID ) noexcept;

    ~ConversationTreeWindow_NPC() override;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

    const char * GetNPCText() const noexcept;

    bool GetIsTrunk() const noexcept;

    const char * GetTreeName() const noexcept;

    const std::map< size_t, ConversationTreeWindow_PlayerPtr > & GetChildren() const noexcept;

  public:
    void SetLocation( const ImVec2 &Loc ) noexcept override;
  
  public:
    int CheckPosition( const std::map< size_t, ConversationTreeWindow_BasePtr > &AllWindows ) noexcept override;

  private:
    void RunWindow() noexcept override;

  private:
    static void AddResponseWindow( void *This ) noexcept;
    static void RemoveResponseWindow( void *This ) noexcept;

  private:
    static Static_NoInstance_Variables TrunkSize;
    static Static_NoInstance_Variables Sizes;
    static Static_NoInstance_Variables ExpandedSizes;

    static constexpr size_t Padding = 10;

  private:
    const bool Trunk;

  private:
    std::map< size_t, ConversationTreeWindow_PlayerPtr > ChildWindows;

    Input TreeName;

    MultiInput NPCText;

    Button AddResponse;
    Button RemoveResponse;

    bool BigText = false;

    ImVec2 TempLocation = { 0,0 };
};
