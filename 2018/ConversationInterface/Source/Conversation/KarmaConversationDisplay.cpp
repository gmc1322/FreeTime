#include "KarmaConversationDisplay.h"

#include <algorithm>

KarmaConversationDisplay::NPCDataStruct::NPCDataStruct( const std::string &NPCString ) noexcept : NPCText( new char[ NPCString.size() ] )
{
  strcpy_s( const_cast< char* >( NPCText ), static_cast< size_t >( NPCString.size() + 1 ), NPCString.c_str() );
}

KarmaConversationDisplay::NPCDataStruct::~NPCDataStruct() noexcept
{
  delete[] NPCText;
}

void KarmaConversationDisplay::NPCDataStruct::operator=( NPCDataStruct &&Move ) const noexcept
{
  std::swap( NPCText, Move.NPCText );
}

///////////////////////////////////////////////////////////////////////////////

KarmaConversationDisplay::PlayerDataStruct::KarmaConversationFunctor::KarmaConversationFunctor( const KarmaTree &Tree ) noexcept : Tree( Tree )
{
}

int KarmaConversationDisplay::PlayerDataStruct::KarmaConversationFunctor::operator()( ArgType Index ) const noexcept
{
  return Tree.ChooseOption_NoThrow( Index );
}

///////////////////////////////////////////////////////////////////////////////

KarmaConversationDisplay::PlayerDataStruct::PlayerDataStruct( const KarmaTree &Tree, int InitialKarma, float MaxBad,
                                                              float MaxGood ) noexcept
{
  size_t Count = 1;
  for( auto Iter : Tree.GetPlayerOptions( InitialKarma ) )
  {
    PlayerOptions.push_back(
      std::make_pair(
        Button< KarmaConversationFunctor >{
      ( Iter.second ? ( Iter.second < 0 ? ImColor{ std::min( Iter.second / MaxBad, 1.f ) + .25f, 0.f, 0.f } :
                                          ImColor{ 0.f, std::min( Iter.second / MaxGood, 1.f ) + .25f, 0.f } ) :
        ImColor{ 0.0f, 0.0f, 1.5f } ),
        ( "Option " + std::to_string( Count ) + ": " + Iter.first ).c_str(), KarmaConversationFunctor{ Tree } },
        Iter.second ) );

    ++Count;
  }
}

void KarmaConversationDisplay::PlayerDataStruct::operator=( PlayerDataStruct &&Move ) const noexcept
{
  PlayerOptions = std::move( Move.PlayerOptions );
}

///////////////////////////////////////////////////////////////////////////////

KarmaConversationDisplay::KarmaConversationDisplay( KarmaTree &&Tree_, int InitialKarma, int MaxBad, int MaxGood, float NPCWindowSizeY,
                                                    float PlayerWindowSizeY ) noexcept :
Tree{ std::move( Tree_ ) }, NPCWindowSizeY( NPCWindowSizeY ), PlayerWindowSizeY( PlayerWindowSizeY ), NPCData{ Tree.GetNPCText() },
PlayerData{ Tree, InitialKarma, static_cast< float >( MaxBad ), static_cast< float >( MaxGood ) }, InitialKarma( InitialKarma ),
MaxBad( static_cast< float >( MaxBad ) ), MaxGood( static_cast< float >( MaxGood ) ), KarmaChange( 0 )
{
}

std::pair< int, int > KarmaConversationDisplay::Draw( int Width, int Hight ) const noexcept
{
  const float &WidthF = static_cast< float >( Width ), &HightF = static_cast< float >( Hight );

  DrawNPC( WidthF );

  return DrawPlayer( WidthF, HightF );
}

void KarmaConversationDisplay::DrawNPC( float Width ) const noexcept
{
  ImGui::SetNextWindowPos( ImVec2{ 0, 0 } );

  ImGui::SetNextWindowSize( ImVec2{ Width, NPCWindowSizeY } );

  ImGui::Begin( "NPC Text", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize );

  ImGui::TextWrapped( NPCData.NPCText );

  ImGui::End();
}

std::pair< int, int > KarmaConversationDisplay::DrawPlayer( float Width, float Hight ) const noexcept
{
  ImGui::SetNextWindowPos( ImVec2{ 0, Hight - NPCWindowSizeY } );

  ImGui::SetNextWindowSize( ImVec2{ Width, PlayerWindowSizeY } );

  ImGui::Begin( "Player ConversationReturnOptions", nullptr, ( ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                                                               ImGuiWindowFlags_NoResize ) );

  int Return = KarmaTreeCreator::ContinueConversation;

  size_t Count = 0;
  for( auto Iter = PlayerData.PlayerOptions.begin(), End = PlayerData.PlayerOptions.end(); Iter != End; ++Iter )
  {
    const auto Pair = Iter->first.RunButton( &Count );
    if( Pair.first )
    {
      NPCData = NPCDataStruct{ Tree.GetNPCText() };
      PlayerData = PlayerDataStruct{ Tree, InitialKarma, MaxBad, MaxGood };

      Return = Pair.second.Value;
      KarmaChange += Iter->second;

      break;
    }

    ImGui::Separator();

    ++Count;
  }

  ImGui::End();

  return std::make_pair( Return, KarmaChange ); // NamedRVO
}
