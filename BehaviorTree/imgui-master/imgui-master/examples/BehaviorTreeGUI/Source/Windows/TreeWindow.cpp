/*!----------------------------------------------------------------------------
\file   TreeWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "TreeWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Tree::Sizes{ { 345, 80 } };

BehaviorTreeWindow_Tree::BehaviorTreeWindow_Tree( size_t ID ) noexcept : BehaviorTreeWindow_Level1( Tree, "Tree Init Function", ID )
{
}

Static_NoInstance_Variables& BehaviorTreeWindow_Tree::GetSizes() const noexcept
{
  return Sizes;
}

void BehaviorTreeWindow_Tree::RunWindow() noexcept
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
