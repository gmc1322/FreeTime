
#include "imgui.h"

#include "Button.h"

Button::Button( const char *Name, ButtonFunctionPtr Run, const ButtonColorHolder &ButtonColors ) noexcept : Name_( Name ),
  Function( Run ), ButtonColors_( ButtonColors )
{
}

Button::Button( const ImColor &ButtonColorType, const char *Name, ButtonFunctionPtr Run ) noexcept : Name_( Name ), Function( Run ),
ButtonColors_( 
  std::make_tuple( ImColor( ButtonColorType.Value.x * 0.5f, ButtonColorType.Value.y * 0.5f, ButtonColorType.Value.z * 0.5f ),
                   ImColor( ButtonColorType.Value.x * 0.65f, ButtonColorType.Value.y * 0.65f, ButtonColorType.Value.z * 0.65f ),
                   ImColor( ButtonColorType.Value.x * 0.8f, ButtonColorType.Value.y * 0.8f, ButtonColorType.Value.z * 0.8f ) ) )
{
}

Button::Button( const Button& CopyButton ) noexcept : Name_( CopyButton.Name_ ), Function( CopyButton.Function ),
  ButtonColors_( CopyButton.ButtonColors_ )
{
}

bool Button::RunButton( void *Data ) const noexcept
{
  ImGui::PushID( 0 );

  ImGui::PushStyleColor( ImGuiCol_Button, std::get< 0 >( ButtonColors_ ) );
  ImGui::PushStyleColor( ImGuiCol_ButtonHovered, std::get< 1 >( ButtonColors_ ) );
  ImGui::PushStyleColor( ImGuiCol_ButtonActive, std::get< 2 >( ButtonColors_ ) );

  auto Clicked = ImGui::Button( Name_.c_str() );

  ImGui::PopStyleColor( 3 );

  ImGui::PopID();

  if( Clicked && Function )
  {
    Function( Data );
  }

  return Clicked;
}
