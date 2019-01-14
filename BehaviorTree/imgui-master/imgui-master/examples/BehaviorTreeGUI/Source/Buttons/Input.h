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

class Input
{
  public:
    Input( const char * Name, size_t InputCharacterMax = 20, bool OnlyNumbers = false ) noexcept;
    Input( const std::string &Name, size_t InputCharacterMax = 20, bool OnlyNumbers = false ) noexcept;

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
    static const float TextXSize;
    static const float TextYSize;

  private:
    const size_t InputMax;
    const ImVec2 InputBoxSize;
    const std::string Name_;
    const bool NumbersOnly;

  private:
    char *InputString;
};
