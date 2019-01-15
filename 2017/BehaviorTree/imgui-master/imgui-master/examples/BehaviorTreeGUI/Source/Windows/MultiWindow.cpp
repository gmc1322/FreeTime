/*!----------------------------------------------------------------------------
\file   MultiWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "MultiWindow.h"

Static_NoInstance_Variables BehaviorTreeWindow_Multi::SmallSize{ { 225, 78 } };
Static_NoInstance_Variables BehaviorTreeWindow_Multi::LargeSize{ { 225, 180 } };

BehaviorTreeWindow_MultiPtr BehaviorTreeWindow_Multi::SizesRunner = nullptr;

BehaviorTreeWindow_Multi::BehaviorTreeWindow_Multi( size_t ID ) noexcept : 
  BehaviorTreeWindow_Base( Multi, ID ), EnableGenerator( "Disable Generator", "Enable Generator" ),
  RegenerationType( "Gen Type", { "Constant", "Dynamic", "Regenerating" } ), GeneratorFunction( "Generator" ),
  GenerateCount( "Generate Count", 11, true ), RunFunction( "Run" )
{
}

BehaviorTreeWindow_Multi::~BehaviorTreeWindow_Multi()
{
  SizesRunner = nullptr;
}

Static_NoInstance_Variables & BehaviorTreeWindow_Multi::GetSizes() const noexcept
{
  return IsGenerating ? LargeSize : SmallSize;
}

bool BehaviorTreeWindow_Multi::GetIsGenerating() const noexcept
{
  return IsGenerating;
}

const char * BehaviorTreeWindow_Multi::GetGenFunction() const noexcept
{
  return GeneratorFunction.GetInput();
}

const char * BehaviorTreeWindow_Multi::GetGenCount() const noexcept
{
  return GenerateCount.GetInput();
}

const char * BehaviorTreeWindow_Multi::GetRunFunction() const noexcept
{
  return RunFunction.GetInput();
}

const char * BehaviorTreeWindow_Multi::GetGenType() const noexcept
{
  return RegenerationType.GetSelectedString();
}

void BehaviorTreeWindow_Multi::RunWindow() noexcept
{
  if( !SizesRunner )
  {
    SizesRunner = this;

    auto MappedScaleTemp = GetMappedScale();

    LargeSize.CalculateSizes( MappedScaleTemp );
    SmallSize.CalculateSizes( MappedScaleTemp );
  }
  else if( SizesRunner == this )
  {
    auto MappedScaleTemp = GetMappedScale();

    LargeSize.CalculateSizes( MappedScaleTemp );
    SmallSize.CalculateSizes( MappedScaleTemp );
  } 

  RunShowLines();

  ImGui::Begin( GetTempName(), nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
                                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings ) );

  RunFocused();

  if( RunMakeLines() )
  {
    ImGui::End();

    return;
  }

  auto InputScale = GetInputScale();

  RunNameInput( InputScale );

  IsGenerating = EnableGenerator.RunToggle();

  if( IsGenerating )
  {
    GeneratorFunction.RunInput( InputScale );

    if( ImGui::IsItemHovered() )
    {
      ImGui::BeginTooltip();

      ImGui::SetTooltip( "The function to generate data for the RUN function" );

      ImGui::EndTooltip();
    }

    GenerateCount.RunInput( InputScale );

    if( ImGui::IsItemHovered() )
    {
      ImGui::BeginTooltip();

      ImGui::SetTooltip( "How many function instances to run" );

      ImGui::EndTooltip();
    }

    RunFunction.RunInput( InputScale );

    if( ImGui::IsItemHovered() )
    {
      ImGui::BeginTooltip();

      ImGui::SetTooltip( "The function receiving the data from the generator" );

      ImGui::EndTooltip();
    }

    RegenerationType.RunSwitch();

    if( ImGui::IsItemHovered() )
    {
      ImGui::BeginTooltip();

      switch( RegenerationType.GetSelected() )
      {
        case Constant:
          ImGui::SetTooltip( "Constant: will only generate a set of data the first time the Multi Node is run." );
          break;

        case Dynamic:
          ImGui::SetTooltip( "Dynamic: will re-generate the data every time the Multi Node is reset." );
          break;

        case Regenerating:
          ImGui::SetTooltip( "Regenerating: will re-generate the data every loop." );
          break;
      }

      ImGui::EndTooltip();
    }
  }

  ImGui::End();
}

void BehaviorTreeWindow_Multi::SetGenerating() noexcept
{
  EnableGenerator.SetClicked();
}

void BehaviorTreeWindow_Multi::SetGenFunction( const std::string& String ) const noexcept
{
  GeneratorFunction.SetInput( String );
}

void BehaviorTreeWindow_Multi::SetGenCount( const std::string& String ) const noexcept
{
  GenerateCount.SetInput( String );
}

void BehaviorTreeWindow_Multi::SetRunFunction( const std::string& String ) const noexcept
{
  RunFunction.SetInput( String );
}

void BehaviorTreeWindow_Multi::SetGenType( const std::string& String ) noexcept
{
  RegenerationType.SetSwitch( String );
}
