#pragma once

#include "imgui.h"

#include "Conversation/Triple.h"

// First is the RGB color for the button. Second is the color when hovered. Third is the color when clicked.
#define DefaultButtonColors ImColor{ 0.0f, 0.0f, 0.5f }, ImColor{ 0.0f, 0.0f, 0.65f }, ImColor{ 0.0f, 0.0f, 0.8f }

class NullFunctor
{
  public:
    void operator()( void * ) const {}
};

template< typename Functor >
class Button
{
  public:
    typedef Triple< ImColor, ImColor, ImColor > ButtonColorHolder;

  public:
    /**
     * \brief Creates a new button
     *
     * \param Name         - The text for the button to display
     * \param Run          - The function for the button to run
     * \param ButtonColors - First is the RGB color for the button. Second is the color when hovered over.
     *                       Third is the color when clicked. ( 0.0f to 1.0f )
     */
    explicit Button( const char *Name = "Button", Functor &&Run = NullFunctor{},
                     ButtonColorHolder &&ButtonColors = ButtonColorHolder{ DefaultButtonColors } ) noexcept;

    /**
     * \brief Creates a new button
     * 
     * \param ButtonColorType - The type of color for the button. RGB  Ex: ImColor{ 0.0f, 0.0f, 1.0f } = Blue
     * \param Name            - The text for the Button to display
     * \param Run             - The function for the Button to run when clicked
     */
    explicit Button( const ImColor &ButtonColorType = ImColor{ 0.0f, 0.0f, 1.0f }, const char *Name = "Button",
                     Functor &&Run = NullFunctor{} ) noexcept;

    explicit Button( const Button &Copy ) noexcept;
             Button(       Button &&Move ) noexcept;

    ~Button() noexcept;

  public:
    std::pair< bool, typename Functor::ReturnType > RunButton( typename Functor::ArgType *Data = nullptr ) const noexcept;

  private:
    const char *Name;

    mutable Functor Function;

    const ButtonColorHolder ButtonColors;
};

template< typename Functor >
using UnrefedButton = Button< UnrefedType< Functor > >;

template< typename Functor >
using ButtonColorHolder = typename UnrefedButton< Functor >::ButtonColorHolder;

template< typename Functor >
constexpr UnrefedButton< Functor > MakeButton( const char *Name = "Button", Functor &&Run = NullFunctor{},
                                               ButtonColorHolder< Functor > &&ButtonColors = MakeTriple( DefaultButtonColors ) ) noexcept;

template< typename Functor >
constexpr UnrefedButton< Functor > MakeButton( const ImColor &ButtonColorType = ImColor{ 0.0f, 0.0f, 1.0f }, const char *Name_ = "Button",
                                               Functor &&Run = NullFunctor{} ) noexcept;

#include "Button.cpp"

#undef DefaultButtonColors
