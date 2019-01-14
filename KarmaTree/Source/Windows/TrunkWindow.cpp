/*!----------------------------------------------------------------------------
\file   TrunkWindow.cpp

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "TrunkWindow.h"
#include "PlayerWindow.h"
#include "ImVec.h"

Static_NoInstance_Variables KarmaTreeWindow_Trunk::Sizes{ ImVec2{ 230, 172 } };
Static_NoInstance_Variables KarmaTreeWindow_Trunk::ExpandedSizes{ ImVec2{ 0,0 } };

KarmaTreeWindow_Trunk::KarmaTreeWindow_Trunk() noexcept :
  KarmaTreeWindow_Base( NPC, ImVec2( X / 2 - 450.0f / 4 - Sizes.WindowSize_.x / 2, 20.0f ), TrunkID ), TreeName( "Tree Name" ),
  NPCText( "Trunk Text" ), BadKarmaThreshold( "Bad Karma Threshold", 10, true ), GoodKarmaThreshold( "Good Karma Threshold", 10, true ),
  AddResponse( "Add Response", AddResponseWindow ), RemoveResponse( "Remove Response", RemoveResponseWindow )
{
  ChildWindows.emplace( 0, new KarmaTreeWindow_Player{ GetLocation(), Sizes.WindowSize_, GetID() } );

  BadKarmaThreshold.SetInput( "-50" );
  GoodKarmaThreshold.SetInput( "50" );
}

KarmaTreeWindow_Trunk::~KarmaTreeWindow_Trunk()
{
  for( auto Iter : ChildWindows )
  {
    Iter.second->SetGarbage();
  }
}

Static_NoInstance_Variables & KarmaTreeWindow_Trunk::GetSizes() const noexcept
{
  return ( BigText ? ExpandedSizes : Sizes );
}

const char * KarmaTreeWindow_Trunk::GetNPCText() const noexcept
{
  return NPCText.GetInput();
}

const char * KarmaTreeWindow_Trunk::GetTreeName() const noexcept
{
  return TreeName.GetInput();
}

const std::map< size_t, KarmaTreeWindow_PlayerPtr > & KarmaTreeWindow_Trunk::GetChildren() const noexcept
{
  return ChildWindows;
}

const char * KarmaTreeWindow_Trunk::GetBadThreshold() const noexcept
{
  return BadKarmaThreshold.GetInput();
}

const char * KarmaTreeWindow_Trunk::GetGoodThreshold() const noexcept
{
  return GoodKarmaThreshold.GetInput();
}

void KarmaTreeWindow_Trunk::RunWindow() noexcept
{
  static KarmaTreeWindow_TrunkPtr ExpandedRunner = nullptr;

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

  ImGui::Begin( "Trunk Node", nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar |
                                         ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove ) );
  
  RunFocused();

  NPCText.RunInput( BigText ? ImVec2{ ( X - 237.0f ) / ( 7.1f * 20 ) , ( Y - 65.0f ) / 20 } : GetInputScale() );

  BigText = ImGui::IsItemActive();

  TreeName.RunInput();

  BadKarmaThreshold.RunInput();
  GoodKarmaThreshold.RunInput();

  AddResponse.RunButton( this );
  RemoveResponse.RunButton( this );

  ImGui::End();
}

void KarmaTreeWindow_Trunk::AddResponseWindow( void *This ) noexcept
{
  auto CastedData = static_cast< KarmaTreeWindow_TrunkPtr >( This );

  if( !CastedData->ChildWindows.empty() )
  {
    for( auto Iter : CastedData->ChildWindows )
    {
      const ImVec2 & Loc = Iter.second->GetLocation();

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

void KarmaTreeWindow_Trunk::RemoveResponseWindow( void *This ) noexcept
{
  auto CastedData = static_cast< KarmaTreeWindow_TrunkPtr >( This );

  if( CastedData->ChildWindows.size() > 1 )
  {
    auto Index = CastedData->ChildWindows.size() - 1;

    CastedData->ChildWindows.find( Index )->second->SetGarbage();

    CastedData->ChildWindows.erase( Index );

    for( auto Iter : CastedData->ChildWindows )
    {
      const ImVec2 & Loc = Iter.second->GetLocation();

      Iter.second->SetLocation( { Loc.x + Iter.second->GetSizes().WindowSize_.x / 2 + Padding, Loc.y } );
    }
  }
}
