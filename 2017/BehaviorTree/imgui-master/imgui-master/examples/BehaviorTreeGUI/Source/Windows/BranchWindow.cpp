/*!----------------------------------------------------------------------------
\file   BranchWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "BranchWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Branch::Sizes{ { 370, 80 } };

BehaviorTreeWindow_Branch::BehaviorTreeWindow_Branch( size_t ID ) noexcept : BehaviorTreeWindow_Level2( Branch, "On Run Function", ID )
{
}

Static_NoInstance_Variables & BehaviorTreeWindow_Branch::GetSizes() const noexcept
{
  return Sizes;
}

void BehaviorTreeWindow_Branch::RunWindow() noexcept
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
