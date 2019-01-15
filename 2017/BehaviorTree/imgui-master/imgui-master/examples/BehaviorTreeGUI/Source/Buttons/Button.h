#pragma once

#include <tuple>
#include <string>

#include "imgui.h"

typedef void ( *ButtonFunctionPtr )( void* );

#define DefaultButtonColors ImColor( 0.0f, 0.0f, 0.5f ), ImColor( 0.0f, 0.0f, 0.65f ), ImColor( 0.0f, 0.0f, 0.8f )

class Button
{
  public:
    typedef std::tuple< ImColor, ImColor, ImColor > ButtonColorHolder;

  public:
    /**
     * \brief Creates a new button
     *
     * \param Name         - The text for the button to display
     * \param Run          - The function for the button to run
     * \param ButtonColors - First(0) is the RGB color for the button. Second(1) is the color when hovered over. Third(2) is the color when clicked. 0.0f to 1.0f
     */
    Button( const char *Name = "Button", ButtonFunctionPtr Run = nullptr,
            const ButtonColorHolder &ButtonColors = std::make_tuple( DefaultButtonColors ) ) noexcept;

    /**
     * \brief Creates a new button
     * 
     * \param ButtonColorType - The type of color for the button. RGB  Ex: ImColor( 0.0f, 0.0f, 1.0f ) = Blue
     * \param Name            - The text for the Button to display
     * \param Run             - The function for the Button to run when clicked
     */
    explicit Button( const ImColor &ButtonColorType = ImColor( 0.0f, 0.0f, 1.0f ), const char *Name = "Button",
                     ButtonFunctionPtr Run = nullptr ) noexcept;

    explicit Button( const Button &CopyButton ) noexcept;

  public:
    bool RunButton( void *Data = nullptr ) const noexcept;

  private:
    const std::string Name_;

    const ButtonFunctionPtr Function;

    const ButtonColorHolder ButtonColors_;
};

#undef DefaultButtonColors
