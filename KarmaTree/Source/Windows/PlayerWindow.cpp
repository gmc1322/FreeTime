/*!----------------------------------------------------------------------------
\file   PlayerWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "PlayerWindow.h"

#include "ImVec.h"

Static_NoInstance_Variables KarmaTreeWindow_Player::DefaultSize{ ImVec2{ 265, 135 } };
Static_NoInstance_Variables KarmaTreeWindow_Player::ExtraSize{ ImVec2{ 265, DefaultSize.WindowSize_.y + 25 } };
Static_NoInstance_Variables KarmaTreeWindow_Player::ExpandedSizes{ ImVec2{ 0,0 } };

KarmaTreeWindow_Player::KarmaTreeWindow_Player( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, size_t ParentID ) noexcept :
  KarmaTreeWindow_Base( Player, ParentLocation, ParentSize, DefaultSize.WindowSize_.x, ParentID ), PlayerText( "Player Response" ),
  ResponseType( "Response Type", { "Default Response", "Dialogue Ender", "Dialogue Event" } ), CustomResponseNumber( "Event ID", 10, true ),
  RequiredKarma( "Required Karma", 10, true ), RequiredKeys( "RequiredKeys" )
{
  ResponseType.SetSwitch( "Dialogue Ender" );

  CustomResponseNumber.SetInput( "1" );
}

KarmaTreeWindow_Player::~KarmaTreeWindow_Player()
{
  if( Child )
  {
    Child->SetGarbage();
  }
}

Static_NoInstance_Variables & KarmaTreeWindow_Player::GetSizes() const noexcept
{
  return BigText ? ExpandedSizes : ( MoreSize ? ExtraSize : DefaultSize );
}

KarmaTreeWindow_NPCDoubleConstPtr KarmaTreeWindow_Player::GetChild() const noexcept
{
  return Child;
}

const char * KarmaTreeWindow_Player::GetText() const noexcept
{
  return PlayerText.GetInput();
}

const char * KarmaTreeWindow_Player::GetResponseType() const
{
  switch( ResponseType.GetSelected() )
  {
    case Response:
      return "DefaultResponseOptions::ContinueConversation";

    case Exit:
      return "DefaultResponseOptions::EndConversation";

    case Event:
      return ( std::string{ "static_cast< DefaultResponseOptions >( " } + CustomResponseNumber.GetInput() + " )" ).c_str();
  }

  return "";
}

const char * KarmaTreeWindow_Player::GetKarma() const
{
  return RequiredKarma.GetInput();
}

const char * KarmaTreeWindow_Player::GetKeys() const
{
  return RequiredKeys.GetInput();
}

void KarmaTreeWindow_Player::RunWindow() noexcept
{
  static KarmaTreeWindow_PlayerPtr ExpandedRunner = nullptr;
 
  if( !ExpandedRunner )
  {
    ExpandedRunner = this;

    ExpandedSizes.SetNewWindowSize( { X - 220.0f, Y - 30.f } );

    ExpandedSizes.CalculateSizes( GetMappedScale() );
  }
  else if( ExpandedRunner == this )
  {
    ExpandedSizes.CalculateSizes( GetMappedScale() );
  }

  if( BigText )
  {
    if( !TempLocation.x )
    {
      TempLocation = GetLocation();
    }

    SetLocation( ImVec2{ 10, 20 } - GetDraggedLoc() );
  }
  else if( TempLocation.x )
  {
    SetLocation( TempLocation );

    TempLocation.x = 0;
  }

  RunSetWindow();

  RunShowLines();
  
  ImGui::Begin( GetTempName(), nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | 
                                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings ) );
  
  RunFocused();
  
  PlayerText.RunInput( BigText ? ImVec2{ ( X - 237.0f ) / ( 7.1f * 20 ) , ( Y - 65.0f ) / 20 } : GetInputScale() );

  BigText = ImGui::IsItemActive();

  ResponseType.RunSwitch();

  const int Type = ResponseType.GetSelected();

  if( !Child )
  {
    if( Type == Response )
    {
      Child = new KarmaTreeWindow_NPC{ GetLocation(), DefaultSize.WindowSize_, GetID() };

      MoreSize = false;
    }
    else if( Type == Event )
    {
      MoreSize = true;
      
      CustomResponseNumber.RunInput();

      if( !atoi( CustomResponseNumber.GetInput() ) )
      {
        CustomResponseNumber.SetInput( "1" );
      }
    }
    else
    {
      MoreSize = false;
    }
  }
  else if( Type != Response )
  {
    Child->SetGarbage();

    Child = nullptr;

    if( Type == Event )
    {
      MoreSize = true;

      CustomResponseNumber.RunInput();

      if( !atoi( CustomResponseNumber.GetInput() ) )
      {
        CustomResponseNumber.SetInput( "1" );
      }
    }
    else
    {
      MoreSize = false;
    }
  }

  RequiredKarma.RunInput();

  RequiredKeys.RunInput();
  
  ImGui::End();
}
