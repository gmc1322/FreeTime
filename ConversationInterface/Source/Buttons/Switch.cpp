/*!----------------------------------------------------------------------------
\file   Switch.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "Switch.h"

#include "imgui.h"
#include "Input.h"

Switch::Switch( const char *Name, std::initializer_list< const char * >Strings ) noexcept :
  Name_( Name ), Items_( new const char * [ Strings.size() ] ), ItemsCount_( Strings.size() )
{
  auto iter = Strings.begin();

  for( size_t start = 0, end = Strings.size(); start < end; ++start )
  {
    Items_[ start ] = *iter;

    const auto Length = ( strlen( *iter ) + 3 ) * TextXSize;

    if( Length > LongestItem )
    {
      LongestItem = Length;
    }

    ++iter;
  }
}

Switch::~Switch()
{
  delete [] Items_;
}

void Switch::RunSwitch() noexcept
{
  ImGui::PushItemWidth( LongestItem );

  ImGui::ListBox( Name_.c_str(), &SelectedItem, Items_, ItemsCount_, 1 );

  ImGui::PopItemWidth();
}

int Switch::GetSelected() const noexcept
{
  return SelectedItem;
}

const char * Switch::GetSelectedString() const noexcept
{
  return Items_[ SelectedItem ];
}

void Switch::SetSwitch( const std::string& String ) noexcept
{
  for( size_t i = 0; i < ItemsCount_; ++i )
  {
    if( !strcmp( Items_[ i ], String.c_str() ) )
    {
      SelectedItem = i;

      break;
    }
  }
}
