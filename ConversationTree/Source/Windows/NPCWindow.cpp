/*!----------------------------------------------------------------------------
\file   NPCWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "NPCWindow.h"
#include "PlayerWindow.h"
#include "Overloads/ImVec.h"

Static_NoInstance_Variables ConversationTreeWindow_NPC::TrunkSize{ ImVec2{ 223, 125 } };
Static_NoInstance_Variables ConversationTreeWindow_NPC::Sizes{ ImVec2{ 223, 100 } };
Static_NoInstance_Variables ConversationTreeWindow_NPC::ExpandedSizes{ ImVec2{ 0,0 } };

ConversationTreeWindow_NPC::ConversationTreeWindow_NPC() noexcept :
  ConversationTreeWindow_Base( NPC, ImVec2( X / 2 - 450.0f / 4 - TrunkSize.WindowSize_.x / 2, 20.0f ), static_cast< size_t >( -1 ) ),
  Trunk( true ), ChildWindows(), TreeName( "Tree Name"), NPCText( "NPC Text" ), 
  AddResponse( "Add Response", AddResponseWindow ), RemoveResponse( "Remove Response", RemoveResponseWindow )
{
  ChildWindows.emplace( 0, new ConversationTreeWindow_Player{ GetLocation(), TrunkSize.WindowSize_, GetID() } );
}

ConversationTreeWindow_NPC::ConversationTreeWindow_NPC( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, 
                                                        size_t ParentID ) noexcept :
  ConversationTreeWindow_Base( NPC, ParentLocation, ParentSize, Sizes.WindowSize_.x, ParentID ), Trunk( false ),
  ChildWindows(), TreeName(""), NPCText( "NPC Text" ), AddResponse( "Add Response", AddResponseWindow ),
  RemoveResponse( "Remove Response", RemoveResponseWindow )
{
  ChildWindows.emplace( 0, new ConversationTreeWindow_Player{ GetLocation(), Sizes.WindowSize_, GetID() } );
}

ConversationTreeWindow_NPC::~ConversationTreeWindow_NPC()
{
  for( auto Iter : ChildWindows )
  {
    Iter.second->SetGarbage();
  }
}

Static_NoInstance_Variables & ConversationTreeWindow_NPC::GetSizes() const noexcept
{
  return ( BigText ? ExpandedSizes : ( Trunk ? TrunkSize : Sizes ) );
}

const char * ConversationTreeWindow_NPC::GetNPCText() const noexcept
{
  return NPCText.GetInput();
}

bool ConversationTreeWindow_NPC::GetIsTrunk() const noexcept
{
  return Trunk;
}

const char * ConversationTreeWindow_NPC::GetTreeName() const noexcept
{
  return TreeName.GetInput();
}

const std::map< size_t, ConversationTreeWindow_PlayerPtr > & ConversationTreeWindow_NPC::GetChildren() const noexcept
{
  return ChildWindows;
}

void ConversationTreeWindow_NPC::SetLocation( const ImVec2 &Loc ) noexcept
{
  ConversationTreeWindow_Base::SetLocation( Loc );

  for( auto Iter : ChildWindows )
  {
    Iter.second->SetLocation( CalculateLocation( Loc, Trunk ? TrunkSize.WindowSize_ : Sizes.WindowSize_, 
                                                 Iter.second->GetSizes().WindowSize_.x ) );
  }
}

int ConversationTreeWindow_NPC::CheckPosition( const std::map< size_t, ConversationTreeWindow_BasePtr > &AllWindows_ ) noexcept
{
  for( auto Iter : ChildWindows )
  {
    const int Return = Iter.second->CheckPosition( AllWindows_ );

    switch( Return )
    {
      case -1: // move left
      {
        const ImVec2 &Loc = GetLocation();
        SetLocation( ImVec2{ Loc.x - ( Iter.second->GetSizes().GetWindowSize().x + Padding * 4 ), Loc.y } );
        break;
      }

      case 1: // move right
      {
        const ImVec2 &Loc = GetLocation();
        SetLocation( ImVec2{ Loc.x + ( Iter.second->GetSizes().GetWindowSize().x + Padding * 4 ), Loc.y } );
        break;
      }
    }
  }

  return 0;
}

void ConversationTreeWindow_NPC::RunWindow() noexcept
{
  static ConversationTreeWindow_NPCPtr ExpandedRunner = nullptr;

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

  if( !Trunk )
  {
    RunShowLines();


    ImGui::Begin( GetTempName(), nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove ) );
  }
  else
  {
    ImGui::Begin( "Trunk NPC Node", nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove ) );
  }
  
  RunFocused();

  NPCText.RunInput( BigText ? ImVec2{ ( X - 237.0f ) / ( 7.1f * 20 ) , ( Y - 65.0f ) / 20 } : GetInputScale() );

  BigText = ImGui::IsItemActive();

  if( Trunk )
  {
    TreeName.RunInput();
  }

  AddResponse.RunButton( this );
  RemoveResponse.RunButton( this );

  ImGui::End();
}

void ConversationTreeWindow_NPC::AddResponseWindow( void *This ) noexcept
{
  auto CastedData = static_cast< ConversationTreeWindow_NPCPtr >( This );

  if( !CastedData->ChildWindows.empty() )
  {
    for( auto Iter : CastedData->ChildWindows )
    {
      auto Window = Iter.second;

      auto Loc = Window->GetLocation();

      Window->SetLocation( { Loc.x - Window->GetSizes().WindowSize_.x / 2 - Padding, Loc.y } );
    }

    auto End = CastedData->ChildWindows.end();

    --End;

    CastedData->ChildWindows.emplace(
      std::make_pair( CastedData->ChildWindows.size(),
                      new ConversationTreeWindow_Player( ImVec2{ End->second->GetLocation().x + End->second->GetSizes().WindowSize_.x +
                                                                 Padding * 4, CastedData->GetLocation().y },
                                                                 CastedData->Trunk ? CastedData->TrunkSize.WindowSize_ : 
                                                                                     CastedData->Sizes.WindowSize_, CastedData->GetID() ) ) );
  }
  else
  {
    CastedData->ChildWindows.emplace(
      std::make_pair( CastedData->ChildWindows.size(),
                      new ConversationTreeWindow_Player( CastedData->GetLocation(), CastedData->Trunk ? CastedData->TrunkSize.WindowSize_ :
                                                                                                        CastedData->Sizes.WindowSize_, 
                                                         CastedData->GetID() ) ) );
  }
}

void ConversationTreeWindow_NPC::RemoveResponseWindow( void *This ) noexcept
{
  auto CastedData = static_cast< ConversationTreeWindow_NPCPtr >( This );

  if( CastedData->ChildWindows.size() > 1 )
  {
    auto Index = CastedData->ChildWindows.size() - 1;

    CastedData->ChildWindows.find( Index )->second->SetGarbage();

    CastedData->ChildWindows.erase( Index );

    for( auto Iter : CastedData->ChildWindows )
    {
      auto Window = Iter.second;

      auto Loc = Window->GetLocation();

      Window->SetLocation( { Loc.x + Window->GetSizes().WindowSize_.x / 2 + Padding, Loc.y } );
    }
  }
}
