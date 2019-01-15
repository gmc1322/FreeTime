/*!----------------------------------------------------------------------------
\file   Input.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "Input.h"

#include "ImVec.h"

#include "imgui_internal.h"

const float Input::TextXSize = 7.1f;
const float Input::TextYSize = 20;

Input::Input( const char * Name, size_t InputCharacterMax, bool OnlyNumbers ) noexcept :
  InputMax( InputCharacterMax ), InputBoxSize( ImVec2{ InputMax * TextXSize, TextYSize } ), Name_( Name ),
  NumbersOnly( OnlyNumbers ), InputString( new char[ InputMax ]() )
{
}

Input::Input( const std::string& Name, size_t InputCharacterMax, bool OnlyNumbers ) noexcept :
  InputMax( InputCharacterMax ), InputBoxSize( ImVec2{ InputMax * TextXSize, TextYSize } ), Name_( Name ),
  NumbersOnly( OnlyNumbers ), InputString( new char[ InputMax ]() )
{
}

Input::Input( InputDoubleConstPtr Input ) noexcept :
  InputMax( Input->InputMax ), InputBoxSize( Input->InputBoxSize ), Name_( Input->Name_ ),
  NumbersOnly( Input->NumbersOnly ), InputString( nullptr )
{
}

Input::~Input()
{
  delete[] InputString;
}

bool Input::RunInput( const ImVec2 &Scale ) const noexcept
{
  auto Done = ImGui::InputTextEx( Name_.c_str(), InputString, InputMax, InputBoxSize * Scale, ImGuiInputTextFlags_EnterReturnsTrue );

  if( NumbersOnly )
  {
    if( InputString[ 0 ] < '0' || InputString[ 0 ] > '9' )
    {
      InputString[ 0 ] = '0';

      InputString[ 1 ] = '\0';
    }

    for( size_t i = 1; i < InputMax; ++i )
    {
      if( InputString[ i ] == '\0' )
      {
        break;
      }

      if( InputString[ i ] < '0' || InputString[ i ] > '9' )
      {
        InputString[ i ] = '\0';

        break;
      }
    }
  }

  return Done;
}

const char* Input::GetInput() const noexcept
{
  return InputString;
}

void Input::SetInput( const std::string &String ) const noexcept
{
  strcpy_s( InputString, InputMax, String.c_str() );
}

size_t Input::GetInputMax() const noexcept
{
  return InputMax;
}

const ImVec2 & Input::GetInputBoxSize() const noexcept
{
  return InputBoxSize;
}

bool Input::GetIfNumbersOnly() const noexcept
{
  return NumbersOnly;
}
