#include "ConversationDisplay.h"

#include "imgui.h"

ConversationDisplay::NPCDataStruct::NPCDataStruct( const std::string &NPCString ) noexcept : NPCText( new char[ NPCString.size() ] )
{
  strcpy_s( const_cast< char* >( NPCText ), static_cast< size_t >( NPCString.size() + 1 ), NPCString.c_str() );
}

ConversationDisplay::NPCDataStruct::~NPCDataStruct() noexcept
{
  delete[] NPCText;
}

void ConversationDisplay::NPCDataStruct::operator=( NPCDataStruct &&Move ) const noexcept
{
  std::swap( NPCText, Move.NPCText );
}

///////////////////////////////////////////////////////////////////////////////

ConversationDisplay::PlayerDataStruct::ConversationFunctor::ConversationFunctor( const ConversationTree &Tree ) noexcept : Tree( Tree )
{
}

int ConversationDisplay::PlayerDataStruct::ConversationFunctor::operator()( ArgType Index ) const noexcept
{
  return Tree.ChooseOption_NoThrow( Index );
}

///////////////////////////////////////////////////////////////////////////////

ConversationDisplay::PlayerDataStruct::PlayerDataStruct( const ConversationTree &Tree ) noexcept
{
  size_t Count = 1;
  for( auto Iter : Tree.GetPlayerOptions() )
  {
    PlayerOptions.push_back( MakeButton( ( "Option " + std::to_string( Count ) + ": " + Iter ).c_str(), ConversationFunctor{ Tree } ) );

    ++Count;
  }
}

void ConversationDisplay::PlayerDataStruct::operator=( PlayerDataStruct &&Move ) const noexcept
{
  PlayerOptions = std::move( Move.PlayerOptions );
}

///////////////////////////////////////////////////////////////////////////////

ConversationDisplay::ConversationDisplay( ConversationTree &&Tree_, float NPCWindowSizeY, float PlayerWindowSizeY ) noexcept :
Tree{ std::move( Tree_ ) }, NPCWindowSizeY( NPCWindowSizeY ), PlayerWindowSizeY( PlayerWindowSizeY ), NPCData{ Tree.GetNPCText() },
PlayerData{ Tree }
{
}

int ConversationDisplay::Draw( int Width, int Hight ) const noexcept
{
  const float &WidthF = static_cast< float >( Width ), &HightF = static_cast< float >( Hight );

  DrawNPC( WidthF );

  return DrawPlayer( WidthF, HightF );
}

void ConversationDisplay::DrawNPC( float Width ) const noexcept
{
  ImGui::SetNextWindowPos( ImVec2{ 0, 0 } );

  ImGui::SetNextWindowSize( ImVec2{ Width, NPCWindowSizeY } );

  ImGui::Begin( "NPC Text", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize );

  ImGui::TextWrapped( NPCData.NPCText );

  ImGui::End();
}

int ConversationDisplay::DrawPlayer( float Width, float Hight ) const noexcept
{
  ImGui::SetNextWindowPos( ImVec2{ 0, Hight - NPCWindowSizeY } );

  ImGui::SetNextWindowSize( ImVec2{ Width, PlayerWindowSizeY } );

  ImGui::Begin( "Player ConversationReturnOptions", nullptr, ( ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                                                               ImGuiWindowFlags_NoResize ) );

  int Return = ConversationTreeCreator::ContinueConversation;

  size_t Count = 0;
  for( auto Iter = PlayerData.PlayerOptions.begin(), End = PlayerData.PlayerOptions.end(); Iter != End; ++Iter )
  {   
    const auto Pair = Iter->RunButton( &Count );
    if( Pair.first )
    {
      NPCData = NPCDataStruct{ Tree.GetNPCText() };
      PlayerData = PlayerDataStruct{ Tree };

      Return = Pair.second.Value;

      break;
    }

    ImGui::Separator();

    ++Count;
  }

  ImGui::End();

  return Return; // NamedRVO
}
