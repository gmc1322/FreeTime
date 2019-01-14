/*!----------------------------------------------------------------------------
\file   Input.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <iostream>

#include "MultiInput.h"
#include "ImVec.h"
#include "Input.h"

MultiInput::MultiInput( const char * Name) noexcept : Name_( Name ), InputBoxSize( ImVec2{ 20 * TextXSize , TextYSize } )
{
}

MultiInput::MultiInput( const std::string& Name ) noexcept : Name_( Name ), InputBoxSize( ImVec2{ 20 * TextXSize , TextYSize } )
{
}

MultiInput::MultiInput( MultiInputDoubleConstPtr Input ) noexcept : Name_( Input->Name_ ), InputBoxSize( Input->InputBoxSize )
{
}

void MultiInput::RunInput( const ImVec2 &Scale ) noexcept
{
  static const size_t Max = 1000;

  char *TempString = nullptr;

  TempString = new char[ Max ];

  strcpy_s( TempString, InputString.capacity() + 1, InputString.c_str() );

  ImGui::InputTextMultiline( Name_.c_str(), TempString, Max, InputBoxSize * Scale );

  InputString = TempString;

  if( InputString.size() + 1 == InputString.capacity() )
  {
    InputString.reserve( static_cast< size_t >( InputString.capacity() * 1.75 ) );
  }

  delete[] TempString;
}

const char* MultiInput::GetInput() const noexcept
{
  return InputString.c_str();
}

void MultiInput::SetInput( const std::string &String ) noexcept
{
  InputString = String;
}

const ImVec2 & MultiInput::GetInputBoxSize() const noexcept
{
  return InputBoxSize;
}
