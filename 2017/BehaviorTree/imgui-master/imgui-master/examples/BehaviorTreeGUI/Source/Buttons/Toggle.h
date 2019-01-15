/*!------------------------------------------------------------------------------
\file   Toggle.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "Button.h"

#define DefaultToggleOnColors ImColor( 0.0f, 0.5f, 0.0f ), ImColor( 0.0f, 0.65f, 0.0f ), ImColor( 0.0f, 0.8f, 0.0f )
#define DefaultToggleOffColors ImColor( 0.5f, 0.0f, 0.0f ), ImColor( 0.65f, 0.0f, 0.0f ), ImColor( 0.8f, 0.0f, 0.0f )

class Toggle
{
  public:
    Toggle( const char *OnName = "On", const char *OffName = "Off",
                  ButtonFunctionPtr RunOn = nullptr, ButtonFunctionPtr RunOff = nullptr,
                  Button::ButtonColorHolder OnColors = std::make_tuple( DefaultToggleOnColors ),
                  Button::ButtonColorHolder OffColors = std::make_tuple( DefaultToggleOffColors ) ) noexcept;

    explicit Toggle( ImColor OnColorType = ImColor( 0, 1, 0 ), ImColor OffColorType = ImColor( 1.0f, 0.0f, 0.0f ),
                           const char *OnName = "On", const char *OffName = "Off",
                           ButtonFunctionPtr RunOn = nullptr, ButtonFunctionPtr RunOff = nullptr ) noexcept;

    explicit Toggle( const Button &OnButton, const Button &OffButton ) noexcept;

  public:
    bool RunToggle( void *OnData = nullptr, void *OffData = nullptr ) noexcept;

  public:
    void SetClicked() noexcept;

  private:
    Button OnButton_;
    Button OffButton_;

    bool Clicked = false;
};

#undef DefaultToggleOnColors
#undef DefaultToggleOffColors
