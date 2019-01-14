/*!------------------------------------------------------------------------------
\file   Input.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include <string>

#include "imgui.h"

typedef class Input *InputPtr;
typedef class Input const * const InputDoubleConstPtr;

constexpr float TextXSize = 7.1f;
constexpr float TextYSize = 20;

class Input
{
  public:
    Input( const char *Name, size_t InputMaxCharacters, const char *InitialText ) noexcept;
    Input( const char *Name, size_t InputMaxCharacters = 20, bool OnlyNumbers = false ) noexcept;
    Input( const std::string &Name, size_t InputMaxCharacters = 20, bool OnlyNumbers = false ) noexcept;

    ~Input();

  public:
    bool RunInput( const ImVec2 &Scale = ImVec2( 1.0f, 1.0f ) ) const noexcept;
    const char * GetInput() const noexcept;
    void SetInput( const std::string &String ) const noexcept;

  protected:
    Input( InputDoubleConstPtr Input ) noexcept;

  protected:
    size_t GetInputMax() const noexcept;
    const ImVec2 & GetInputBoxSize() const noexcept;
    bool GetIfNumbersOnly() const noexcept;

  private:
    const size_t InputMaxCharacters_;
    const ImVec2 InputBoxSize;
    const std::string Name_;
    const bool NumbersOnly;
    const bool Protected;

  private:
    char *InputString;
};
