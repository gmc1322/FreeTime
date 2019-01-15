/*!----------------------------------------------------------------------------
\file   Switch.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "Switch.h"

#include "imgui.h"

Switch::Switch( const char *Name, std::initializer_list< const char * >Strings ) noexcept :
  Name_( Name ), Items_( new const char * [ Strings.size() ] ), ItemsCount_( Strings.size() )
{
  auto iter = Strings.begin();

  for( size_t start = 0; start < ItemsCount_; ++start )
  {
    Items_[ start ] = *iter;

    ++iter;
  }
}

Switch::~Switch()
{
  delete [] Items_;
}

void Switch::RunSwitch() noexcept
{
  ImGui::ListBox( Name_.c_str(), &SelectedItem, Items_, ItemsCount_, 1 );
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
