/*!----------------------------------------------------------------------------
\file   Toggle.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "Toggle.h"

Toggle::Toggle( const char *OnName, const char *OffName, ButtonFunctionPtr RunOn, ButtonFunctionPtr RunOff,
                Button::ButtonColorHolder OnColors, Button::ButtonColorHolder OffColors ) noexcept :
  OnButton_( OnName, RunOn, OnColors ), OffButton_( OffName, RunOff, OffColors )
{
}

Toggle::Toggle( ImColor OnColorType, ImColor OffColorType, const char* OnName, const char* OffName,
                ButtonFunctionPtr RunOn, ButtonFunctionPtr RunOff ) noexcept :
  OnButton_( OnColorType, OnName, RunOn ), OffButton_( OffColorType, OffName, RunOff )
{
}

Toggle::Toggle( const Button& OnButton, const Button& OffButton ) noexcept :
  OnButton_( OnButton ), OffButton_( OffButton ), Clicked( false )
{
}

bool Toggle::RunToggle( void *OnData, void *OffData ) noexcept
{
  if( Clicked )
  {
    Clicked = !( OnButton_.RunButton( OnData ) );
  }
  else
  {
    Clicked = OffButton_.RunButton( OffData );
  }

  return Clicked;
}

void Toggle::SetClicked() noexcept
{
  Clicked = true;
}
