#include "Button.h"

template< typename Functor >
Button< Functor >::Button( const char *Name_, Functor &&Run, ButtonColorHolder &&ButtonColors ) noexcept :
Name( new char[ strlen( Name_ ) + 1 ] ), Function{ std::move( Run ) }, ButtonColors{ ButtonColors }
{
  strcpy_s( const_cast< char* >( Name ), strlen( Name_ ) + 1, Name_ );
}

template< typename Functor >
Button< Functor >::Button( const ImColor &ButtonColorType, const char *Name_, Functor &&Run ) noexcept :
Name( new char[ strlen( Name_ ) + 1 ] ), Function{ std::move( Run ) },
ButtonColors{ MakeTriple( ImColor( ButtonColorType.Value.x * 0.5f, ButtonColorType.Value.y * 0.5f, ButtonColorType.Value.z * 0.5f ),
                          ImColor( ButtonColorType.Value.x * 0.65f, ButtonColorType.Value.y * 0.65f, ButtonColorType.Value.z * 0.65f ),
                          ImColor( ButtonColorType.Value.x * 0.8f, ButtonColorType.Value.y * 0.8f, ButtonColorType.Value.z * 0.8f ) ) }
{
  strcpy_s( const_cast< char* >( Name ), strlen( Name_ ) + 1, Name_ );
}

template< typename Functor >
Button< Functor >::Button( const Button &Copy ) noexcept :
Name( new char[ strlen( Copy.Name ) + 1 ] ), Function{ Copy.Function }, ButtonColors{ Copy.ButtonColors }
{
  strcpy_s( const_cast< char* >( Name ), strlen( Copy.Name ) + 1, Copy.Name );
}

template< typename Functor >
Button< Functor >::Button( Button &&Move ) noexcept :
Name( nullptr ), Function{ std::move( Move.Function ) }, ButtonColors{ std::move( Move.ButtonColors ) }
{
  std::swap( Name, Move.Name );
}

template< typename Functor >
Button< Functor >::~Button() noexcept
{
  delete[] Name;
}

template< typename Functor >
std::pair< bool, typename Functor::ReturnType >  Button< Functor >::RunButton( typename Functor::ArgType *Data ) const noexcept
{
  ImGui::PushStyleColor( ImGuiCol_Button, ButtonColors.First );
  ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ButtonColors.Second );
  ImGui::PushStyleColor( ImGuiCol_ButtonActive, ButtonColors.Third );

  if( ImGui::Button( Name ) )
  {
    ImGui::PopStyleColor( 3 );

    return std::make_pair( true, Function( *Data ) );
  }

  ImGui::PopStyleColor( 3 );

  return std::make_pair( false, Functor::ReturnType() );
}

///////////////////////////////////////////////////////////////////////////////

template< typename Functor >
constexpr UnrefedButton< Functor > MakeButton( const char *Name, Functor &&Run, ButtonColorHolder< Functor > &&ButtonColors ) noexcept
{
  return UnrefedButton< Functor >{ Name, std::move( Run ), std::move( ButtonColors )  };
}

template< typename Functor >
constexpr UnrefedButton< Functor > MakeButton( const ImColor &ButtonColorType, const char *Name_, Functor &&Run ) noexcept
{
  return UnrefedButton< Functor >{ ButtonColorType, Name_, std::move( Run )  };
}
