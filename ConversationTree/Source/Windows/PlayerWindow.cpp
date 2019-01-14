/*!----------------------------------------------------------------------------
\file   PlayerWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "PlayerWindow.h"

#include "ImVec.h"

Static_NoInstance_Variables ConversationTreeWindow_Player::DefaultSize{ ImVec2{ 265, 85 } };
Static_NoInstance_Variables ConversationTreeWindow_Player::ExtraSize{ ImVec2{ 265, 110 } };
Static_NoInstance_Variables ConversationTreeWindow_Player::ExpandedSizes{ ImVec2{ 0,0 } };

ConversationTreeWindow_Player::ConversationTreeWindow_Player( const ImVec2 &ParentLocation, const ImVec2 &ParentSize,
                                                              size_t ParentID ) noexcept :
  ConversationTreeWindow_Base( Player, ParentLocation, ParentSize, DefaultSize.WindowSize_.x, ParentID ), PlayerText( "Player Response" ),
  ResponseType( "Response Type", { "Default Response", "Dialogue Ender", "Dialogue Event" } ), CustomResponseNumber( "Event ID", 10, true )
{
  ResponseType.SetSwitch( "Dialogue Ender" );

  CustomResponseNumber.SetInput( "0" );
}

ConversationTreeWindow_Player::~ConversationTreeWindow_Player()
{
  if( Child )
  {
    Child->SetGarbage();
  }
}

Static_NoInstance_Variables & ConversationTreeWindow_Player::GetSizes() const noexcept
{
  return BigText ? ExpandedSizes : ( MoreSize ? ExtraSize : DefaultSize );
}

ConversationTreeWindow_NPCDoubleConstPtr ConversationTreeWindow_Player::GetChild() const noexcept
{
  return Child;
}

const char * ConversationTreeWindow_Player::GetText() const noexcept
{
  return PlayerText.GetInput();
}

const char * ConversationTreeWindow_Player::GetResponseType() const
{
  switch( ResponseType.GetSelected() )
  {
    case Response:
      return "DefaultResponseOptions::ContinueConversation";

    case Exit:
      return "DefaultResponseOptions::EndConversation";

    case Event:
      return ( std::string{ "DefaultResponseOptions::CustomOption + " } + CustomResponseNumber.GetInput() ).c_str();
  }

  return "";
}

void ConversationTreeWindow_Player::SetLocation( const ImVec2 &Loc ) noexcept
{
  ConversationTreeWindow_Base::SetLocation( Loc );

  if( Child )
  {
    Child->SetLocation( Child->GetLocation() + ( Loc - GetLocation() ) );
  }
}

int ConversationTreeWindow_Player::CheckPosition( const std::map< size_t, ConversationTreeWindow_BasePtr > &AllWindows_ ) noexcept
{
  if( Child )
  {
    const auto Result = Child->CheckPosition( AllWindows_ );

    return Result;
  }

  const auto &Loc = GetLocation();

  const auto HalfSize = DefaultSize.WindowSize_.x / 2;
  
  for( auto Iter : AllWindows_ )
  {
    const auto &IterLoc = Iter.second->GetLocation();

    if( Iter.second->GetWindowType() == NPC || Iter.second == this || Loc.y != IterLoc.y )
    {
      continue;
    }

    if( Loc.x + HalfSize >= IterLoc.x - HalfSize && Loc.x - IterLoc.x < DefaultSize.WindowSize_.x )
    {
      return -1;
    }

    if( Loc.x - HalfSize <= IterLoc.x + HalfSize && IterLoc.x - Loc.x < DefaultSize.WindowSize_.x )
    {
      return 1;
    }
  }

  return 0;
}

void ConversationTreeWindow_Player::RunWindow() noexcept
{
  static ConversationTreeWindow_PlayerPtr ExpandedRunner = nullptr;
 
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

    ConversationTreeWindow_Base::SetLocation( ImVec2{ 10, 20 } - GetDraggedLoc() );
  }
  else if( TempLocation.x )
  {
    ConversationTreeWindow_Base::SetLocation( TempLocation );

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
      Child = new ConversationTreeWindow_NPC{ GetLocation(), DefaultSize.WindowSize_, GetID() };

      MoreSize = false;
    }
    else if( Type == Event )
    {
      MoreSize = true;
      
      CustomResponseNumber.RunInput();
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
    }
    else
    {
      MoreSize = false;
    }
  }
  
  ImGui::End();
}
