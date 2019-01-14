/*!----------------------------------------------------------------------------
\file   NullWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright         
   ---------------------------------------------------------------------------- */

#include "NullWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Null::Sizes{ { 70, 5 } };

BehaviorTreeWindow_Null::BehaviorTreeWindow_Null( size_t ID ) noexcept : BehaviorTreeWindow_Base( Null, ID )
{
}

Static_NoInstance_Variables & BehaviorTreeWindow_Null::GetSizes() const noexcept
{
  return Sizes;
}

void BehaviorTreeWindow_Null::RunWindow() noexcept
{
  RunShowLines();

  ImGui::Begin( GetTempName(), nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
                                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings ) );

  RunFocused();

  RunMakeLines();

  ImGui::End();
}
