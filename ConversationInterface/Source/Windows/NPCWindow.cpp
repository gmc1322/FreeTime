/*!----------------------------------------------------------------------------
\file   NPCWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "NPCWindow.h"
#include "PlayerWindow.h"
#include "ImVec.h"

Static_NoInstance_Variables KarmaTreeWindow_NPC::Sizes{ ImVec2{ 223, 100 } };
Static_NoInstance_Variables KarmaTreeWindow_NPC::ExpandedSizes{ ImVec2{ 0,0 } };

KarmaTreeWindow_NPC::KarmaTreeWindow_NPC( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, size_t ParentID ) noexcept :
  KarmaTreeWindow_Base( NPC, ParentLocation, ParentSize, Sizes.WindowSize_.x, ParentID ), NPCText( "NPC Text" ),
  AddResponse( "Add Response", AddResponseWindow ), RemoveResponse( "Remove Response", RemoveResponseWindow )
{
  ChildWindows.emplace( 0, new KarmaTreeWindow_Player{ GetLocation(), Sizes.WindowSize_, GetID() } );
}

KarmaTreeWindow_NPC::~KarmaTreeWindow_NPC()
{
  for( auto Iter : ChildWindows )
  {
    Iter.second->SetGarbage();
  }
}

Static_NoInstance_Variables & KarmaTreeWindow_NPC::GetSizes() const noexcept
{
  return ( BigText ? ExpandedSizes : Sizes );
}

const char * KarmaTreeWindow_NPC::GetNPCText() const noexcept
{
  return NPCText.GetInput();
}

const std::map< size_t, KarmaTreeWindow_PlayerPtr > & KarmaTreeWindow_NPC::GetChildren() const noexcept
{
  return ChildWindows;
}

void KarmaTreeWindow_NPC::RunWindow() noexcept
{
  static KarmaTreeWindow_NPCPtr ExpandedRunner = nullptr;

  Sizes.CalculateSizes( GetMappedScale() );

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

  ImGui::Begin( GetTempName(), nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                          ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove ) );
  
  RunFocused();

  NPCText.RunInput( BigText ? ImVec2{ ( X - 237.0f ) / ( 7.1f * 20 ) , ( Y - 65.0f ) / 20 } : GetInputScale() );

  BigText = ImGui::IsItemActive();

  AddResponse.RunButton( this );
  RemoveResponse.RunButton( this );

  ImGui::End();
}

void KarmaTreeWindow_NPC::AddResponseWindow( void *This ) noexcept
{
  auto CastedData = static_cast< KarmaTreeWindow_NPCPtr >( This );

  if( !CastedData->ChildWindows.empty() )
  {
    for( auto Iter : CastedData->ChildWindows )
    {
      const ImVec2 &Loc = Iter.second->GetLocation();

      Iter.second->SetLocation( { Loc.x - Iter.second->GetSizes().WindowSize_.x / 2 - Padding, Loc.y } );
    }

    auto End = CastedData->ChildWindows.end();

    --End;

    CastedData->ChildWindows.emplace(
      std::make_pair( CastedData->ChildWindows.size(),
                      new KarmaTreeWindow_Player( ImVec2{ End->second->GetLocation().x + End->second->GetSizes().WindowSize_.x +
                                                                 Padding * 4, CastedData->GetLocation().y },
                                                  CastedData->Sizes.WindowSize_, CastedData->GetID() ) ) );
  }
  else
  {
    CastedData->ChildWindows.emplace(
      std::make_pair( CastedData->ChildWindows.size(),
                      new KarmaTreeWindow_Player( CastedData->GetLocation(), CastedData->Sizes.WindowSize_, CastedData->GetID() ) ) );
  }
}

void KarmaTreeWindow_NPC::RemoveResponseWindow( void *This ) noexcept
{
  auto CastedData = static_cast< KarmaTreeWindow_NPCPtr >( This );

  if( CastedData->ChildWindows.size() > 1 )
  {
    auto Index = CastedData->ChildWindows.size() - 1;

    CastedData->ChildWindows.find( Index )->second->SetGarbage();

    CastedData->ChildWindows.erase( Index );

    for( auto Iter : CastedData->ChildWindows )
    {
      const ImVec2 &Loc = Iter.second->GetLocation();

      Iter.second->SetLocation( { Loc.x + Iter.second->GetSizes().WindowSize_.x / 2 + Padding, Loc.y } );
    }
  }
}
