#include "ConversationTree.h"

using namespace ConversationTreeSpace;

ConversationTreeCreator::ID::ID( const ID &Copy ) noexcept : IDs( Copy.IDs )
{
}

ConversationTreeCreator::ID::ID( ID &&Move ) noexcept : IDs( std::move( Move.IDs ) )
{
}


std::vector<size_t>::const_iterator ConversationTreeCreator::ID::begin() const noexcept
{
  return IDs.cbegin();
}

std::vector<size_t>::const_iterator ConversationTreeCreator::ID::end() const noexcept
{
  return IDs.cend();
}

void ConversationTreeCreator::ID::push_back( size_t NewID ) noexcept
{
  IDs.push_back( NewID );
}

ConversationTreeCreator::ConversationTreeCreator( const std::string &NPCStartText ) noexcept : Head( NPCStartText )
{
}

ConversationTreeCreator::ID ConversationTreeCreator::AddText( const std::string &PlayerText, const std::string &NPCText, 
                                                              const ID &NodeIndex ) noexcept
{
  NPC_PLAYER_Pairs *HeadIter = &Head;

  for( auto Iter : NodeIndex )
  {
    HeadIter = &( HeadIter->PlayerResponses[ Iter ].Third );
  }

  ID Return{ NodeIndex };

  Return.push_back( HeadIter->PlayerResponses.size() );

  HeadIter->PlayerResponses.push_back( MakeTriple( static_cast< int >( ContinueConversation ), std::string{ PlayerText },
                                                   NPC_PLAYER_Pairs{ NPCText } ) );

  return Return; // Named RVO
}

void ConversationTreeCreator::AddText( const std::string &PlayerText, int OptionType, const ID &NodeIndex ) noexcept
{
  NPC_PLAYER_Pairs *HeadIter = &Head;

  for( auto Iter : NodeIndex )
  {
    HeadIter = &( HeadIter->PlayerResponses[ Iter ].Third );
  }

  HeadIter->PlayerResponses.push_back( MakeTriple( OptionType, std::string{ PlayerText }, NPC_PLAYER_Pairs{ "" } ) );
}


ConversationTreeCreator::NPC_PLAYER_Pairs::NPC_PLAYER_Pairs( const std::string &NPCText ) noexcept : NPC( NPCText )
{
}

ConversationTreeCreator::NPC_PLAYER_Pairs::NPC_PLAYER_Pairs( const NPC_PLAYER_Pairs &Copy ) noexcept : NPC( Copy.NPC ), 
                                                                                                       PlayerResponses( Copy.PlayerResponses )
{
}

ConversationTreeCreator::NPC_PLAYER_Pairs::NPC_PLAYER_Pairs( NPC_PLAYER_Pairs &&Move ) noexcept :
  NPC( std::move( Move.NPC ) ), PlayerResponses( std::move( Move.PlayerResponses ) )
{
}

///////////////////////////////////////////////////////////////////////////////

constexpr ConversationTree::Responses::ConstIter::ConstIter( ResponseType::const_iterator &&ConstIter ) noexcept :
  IterIndex( std::move( ConstIter ) )
{
}

bool operator!=( const ConversationTree::Responses::ResponseType::const_iterator &Lhs,
                 const ConversationTree::Responses::ConstIter &Rhs ) noexcept
{
  return Lhs != Rhs.IterIndex;
}

ConversationTree::Responses::Iter::Iter( ResponseType::const_iterator &&Iter ) noexcept : IterIndex( std::move( Iter ) )
{
}

const std::string & ConversationTree::Responses::Iter::operator*() const noexcept
{
  return IterIndex->Second;
}

bool ConversationTree::Responses::Iter::operator!=( const ConstIter &Rhs ) const noexcept
{
  return IterIndex != Rhs;
}

void ConversationTree::Responses::Iter::operator++() noexcept
{
  ++IterIndex;
}

///////////////////////////////////////////////////////////////////////////////

constexpr ConversationTree::Responses::Responses( const ResponseType &Response ) noexcept :
PlayerResponses( Response )
{
}

ConversationTree::Responses::Iter ConversationTree::Responses::begin() const noexcept
{
  return Iter{ PlayerResponses.cbegin() };
}

const ConversationTree::Responses::ConstIter ConversationTree::Responses::end() const noexcept
{
  return ConstIter{ PlayerResponses.cend() };
}

size_t ConversationTree::Responses::size() const noexcept
{
  return PlayerResponses.size();
}

///////////////////////////////////////////////////////////////////////////////

ConversationTree::ConversationTree( ConversationTreeCreator &&Move ) noexcept : Head( std::move( Move.Head ) )
{
}

ConversationTree::ConversationTree( ConversationTree &&Move ) noexcept : Head( std::move( Move.Head ) ), CurrentText( &Head )
{
}

const std::string & ConversationTree::GetNPCText() const noexcept
{
  return CurrentText->NPC;
}

ConversationTree::Responses ConversationTree::GetPlayerOptions() const noexcept
{
  return Responses{ CurrentText->PlayerResponses };
}

int ConversationTree::ChooseOption( size_t Number ) const noexcept( false )
{
  try
  {
    const auto &Pair = CurrentText->PlayerResponses.at( Number );

    if( Pair.First != DefaultResponseOptions::ContinueConversation )
    {
      CurrentText = &Head;

      return Pair.First;
    }

    CurrentText = &( Pair.Third );

    return DefaultResponseOptions::ContinueConversation;
  }
  catch( std::out_of_range )
  {
    throw( InvalidOptionNumber( Number + 1, CurrentText->PlayerResponses.size() ) );
  }
}

int ConversationTree::ChooseOption_NoThrow( size_t Number ) const noexcept
{
  const auto &Pair = CurrentText->PlayerResponses[ Number ];

  if( Pair.First != DefaultResponseOptions::ContinueConversation )
  {
    CurrentText = &Head;

    return Pair.First;
  }

  CurrentText = &( Pair.Third );

  return DefaultResponseOptions::ContinueConversation;
}

ConversationTree::InvalidOptionNumber::InvalidOptionNumber( size_t Number, size_t Amount ) noexcept :
  What( "You tried to select option: " + std::to_string( Number ) +
        ( Number == 0 ? ", but the options start at 1!" : ( std::string{ ", but there" } +( Amount == 1 ? " is " : " are " ) + "only: " +
                                                            std::to_string( Amount ) + " option" + ( Amount == 1 ? "!" : "s!" ) ) ) )
{
}

#include <iostream>

namespace ConversationTreeSpace
{
  std::ostream & operator<<( std::ostream &Lhs, const std::string &Rhs ) noexcept
  {
    return Lhs << Rhs.c_str();
  }
}
