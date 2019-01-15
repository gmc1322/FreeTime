/*!----------------------------------------------------------------------------
\file   TrunkWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "TrunkWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Trunk::Sizes{ { 990, 80 } };

BehaviorTreeWindow_Trunk::BehaviorTreeWindow_Trunk() noexcept :
  BehaviorTreeWindow_Level2( Trunk, ImVec2( X / 2 - 450.0f / 4 - Sizes.WindowSize_.x / 2, 20.0f ),
                             "Behavior Tree Name", "On Run Function"),
  OnFinish( "On Finish Function", 28 ), OnReset( "On Reset Function", 28 )
{
}

const char * BehaviorTreeWindow_Trunk::GetOnFinishName() const noexcept
{
  return OnFinish.GetInput();
}

const char * BehaviorTreeWindow_Trunk::GetOnResetName() const noexcept
{
  return OnReset.GetInput();
}

Static_NoInstance_Variables& BehaviorTreeWindow_Trunk::GetSizes() const noexcept
{
  return Sizes;
}

void BehaviorTreeWindow_Trunk::SetOnFinish( const std::string& Text ) const noexcept
{
  OnFinish.SetInput( Text );
}

void BehaviorTreeWindow_Trunk::SetOnReset( const std::string& Text ) const noexcept
{
  OnReset.SetInput( Text );
}

void BehaviorTreeWindow_Trunk::RunWindow() noexcept
{
  RunShowLines();
  
  ImGui::Begin( "Trunk", nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
                                    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings ) );
  
  RunFocused();

  auto InputScale = GetInputScale();
  
  RunFunctionInput( InputScale );

  ImGui::SameLine();
  
  OnFinish.RunInput( InputScale );
  
  ImGui::SameLine();
  
  OnReset.RunInput( InputScale );

  ImGui::Indent( 320 * InputScale.x );
    
  RunDataInput( InputScale );

  ImGui::SameLine();

  RunNameInput( InputScale );
  
  ImGui::End();
}
