/*!----------------------------------------------------------------------------
\file   CheckWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "CheckWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Check::Sizes{ { 370, 80 } };

BehaviorTreeWindow_Check::BehaviorTreeWindow_Check( size_t ID ) noexcept : BehaviorTreeWindow_Level2( Check, "Checker Function", ID )
{
}

Static_NoInstance_Variables & BehaviorTreeWindow_Check::GetSizes() const noexcept
{
  return Sizes;
}

void BehaviorTreeWindow_Check::RunWindow() noexcept
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

  RunDataInput( InputScale );

  ImGui::SameLine();

  RunNameInput( InputScale );

  ImGui::End();
}