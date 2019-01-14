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

Input::Input( const char * Name, size_t InputMaxCharacters, bool OnlyNumbers ) noexcept :
  InputMaxCharacters_( InputMaxCharacters ), InputBoxSize( ImVec2{ InputMaxCharacters * TextXSize, TextYSize } ), Name_( Name ),
  NumbersOnly( OnlyNumbers ), Protected( false ), InputString( new char[ InputMaxCharacters ]() )
{
}

Input::Input( const std::string& Name, size_t InputMaxCharacters, bool OnlyNumbers ) noexcept :
  InputMaxCharacters_( InputMaxCharacters ), InputBoxSize( ImVec2{ InputMaxCharacters * TextXSize, TextYSize } ), Name_( Name ),
  NumbersOnly( OnlyNumbers ), Protected( false ), InputString( new char[ InputMaxCharacters ]() )
{
}

Input::Input( const char* Name, size_t InputMaxCharacters, const char *InitialText ) noexcept :
  InputMaxCharacters_( InputMaxCharacters ), InputBoxSize( ImVec2{ InputMaxCharacters * TextXSize, TextYSize } ), Name_( Name ),
  NumbersOnly( false ), Protected( true ), InputString( new char[ InputMaxCharacters ] )
{
  strcpy_s( InputString, InputMaxCharacters, InitialText );
}

Input::Input( InputDoubleConstPtr Input ) noexcept :
  InputMaxCharacters_( Input->InputMaxCharacters_ ), InputBoxSize( Input->InputBoxSize ), Name_( Input->Name_ ),
  NumbersOnly( Input->NumbersOnly ), Protected( Input->Protected ), InputString( nullptr )
{
}

Input::~Input()
{
  delete[] InputString;
}

bool Input::RunInput( const ImVec2 &Scale ) const noexcept
{
  auto Done = ImGui::InputTextEx( Name_.c_str(), InputString, InputMaxCharacters_, InputBoxSize * Scale,
                                  ( Protected ? ImGuiInputTextFlags_ReadOnly : ImGuiInputTextFlags_EnterReturnsTrue ) );

  if( NumbersOnly )
  {
    if( ( InputString[ 0 ] < '0' || InputString[ 0 ] > '9' ) && InputString[ 0 ] != '-' )
    {
      InputString[ 0 ] = '0';

      InputString[ 1 ] = '\0';
    }

    for( size_t i = 1; i < InputMaxCharacters_; ++i )
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

const char * Input::GetInput() const noexcept
{
  return InputString;
}

void Input::SetInput( const std::string &String ) const noexcept
{
  strcpy_s( InputString, InputMaxCharacters_, String.c_str() );
}

size_t Input::GetInputMax() const noexcept
{
  return InputMaxCharacters_;
}

const ImVec2 & Input::GetInputBoxSize() const noexcept
{
  return InputBoxSize;
}

bool Input::GetIfNumbersOnly() const noexcept
{
  return NumbersOnly;
}
