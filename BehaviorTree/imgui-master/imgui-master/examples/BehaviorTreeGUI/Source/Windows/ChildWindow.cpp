/*!----------------------------------------------------------------------------
\file   ChildWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "ChildWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Child::Sizes{ { 370, 80 } };


BehaviorTreeWindow_Child::BehaviorTreeWindow_Child( size_t ID ) noexcept : BehaviorTreeWindow_Level1( Child, "Get Tree Function", ID )
{
}

Static_NoInstance_Variables & BehaviorTreeWindow_Child::GetSizes() const noexcept
{
  return Sizes;
}

void BehaviorTreeWindow_Child::RunWindow() noexcept
{
  RunShowLines();

  ImGui::Begin( GetTempName(), nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings ) );

  RunFocused();

  auto InputScale = GetInputScale();

  RunFunctionInput( InputScale );

  if( RunMakeLines() )
  {
    ImGui::End();

    return;
  }

  RunNameInput( InputScale );

  ImGui::End();
}
