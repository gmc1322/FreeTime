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

typedef class MultiInput *MultiInputPtr;
typedef class MultiInput const * const MultiInputDoubleConstPtr;

class MultiInput
{
  public:
    MultiInput( const char * Name ) noexcept;
    MultiInput( const std::string &Name ) noexcept;

  public:
    void RunInput( const ImVec2 &Scale = ImVec2( 1.0f, 1.0f ) ) noexcept;
    void SetInput( const std::string &String ) noexcept;

  public:
    const char * GetInput() const noexcept;

  protected:
    MultiInput( MultiInputDoubleConstPtr Input ) noexcept;

  protected:
    const ImVec2 & GetInputBoxSize() const noexcept;

  private:   
    const std::string Name_;

  private:
    std::string InputString;

    ImVec2 InputBoxSize;
};
