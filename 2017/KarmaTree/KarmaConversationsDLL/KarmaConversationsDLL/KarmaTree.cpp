#include "KarmaTree.h"

using namespace KarmaTreeSpace;

const KarmaTreeCreator::ID KarmaTreeCreator::DefaultID;

///////////////////////////////////////////////////////////////////////////////

KarmaTreeCreator::NPC_PLAYER_Pairs::NPC_PLAYER_Pairs( const std::string &NPCText ) noexcept : NPC( NPCText )
{
}

KarmaTreeCreator::NPC_PLAYER_Pairs::NPC_PLAYER_Pairs( const NPC_PLAYER_Pairs &Copy ) noexcept : NPC( Copy.NPC ), 
                                                                                                PlayerResponses( Copy.PlayerResponses )
{
}

KarmaTreeCreator::NPC_PLAYER_Pairs::NPC_PLAYER_Pairs( NPC_PLAYER_Pairs &&Move ) noexcept: NPC( std::move( Move.NPC ) ),
                                                                                          PlayerResponses( std::move( Move.PlayerResponses ) )
{
}

///////////////////////////////////////////////////////////////////////////////

KarmaTreeCreator::ID::ID( const ID &Copy ) noexcept : IDs( Copy.IDs )
{
}

KarmaTreeCreator::ID::ID( ID &&Move ) noexcept : IDs( std::move( Move.IDs ) )
{
}

std::vector< size_t >::const_iterator KarmaTreeCreator::ID::begin() const noexcept
{
  return IDs.cbegin();
}

std::vector< size_t >::const_iterator KarmaTreeCreator::ID::end() const noexcept
{
  return IDs.cend();
}

void KarmaTreeCreator::ID::push_back( size_t NewID ) noexcept
{
  IDs.push_back( NewID );
}

///////////////////////////////////////////////////////////////////////////////

KarmaTreeCreator::KarmaTreeCreator( const std::string &NPCStartText, int BadThreshold, int GoodThreshold ) noexcept :
  Head( NPCStartText ), BadThreshold( BadThreshold ), GoodThreshold( GoodThreshold )
{
}

KarmaTreeCreator::ID KarmaTreeCreator::AddText( const std::string &PlayerText, const std::string &NPCText, const ID &NodeIndex, 
                                                int ResultingKarma, int NeededKarma, TempKeyHolder NeededKeys ) noexcept
{
  NPC_PLAYER_Pairs *HeadIter = &Head;

  for( auto Iter : NodeIndex )
  {
    HeadIter = &( std::get< NPC_PLAYER_Pairs >( HeadIter->PlayerResponses[ Iter ] ) );
  }

  ID Return{ NodeIndex };

  Return.push_back( HeadIter->PlayerResponses.size() );

  HeadIter->PlayerResponses.push_back( std::make_tuple( ContinueConversation, std::string{ PlayerText },
                                                        KarmaContainer{ NeededKarma, KeyType{ NeededKeys }, ResultingKarma },
                                                        NPC_PLAYER_Pairs{ NPCText } ) );

  return Return; // Named RVO
}

void KarmaTreeCreator::AddText( const std::string &PlayerText, int OptionType, const ID &NodeIndex, int ResultingKarma, 
                                int NeededKarma, TempKeyHolder NeededKeys ) noexcept
{
  NPC_PLAYER_Pairs *HeadIter = &Head;

  for( auto Iter : NodeIndex )
  {
    HeadIter = &( std::get< NPC_PLAYER_Pairs >( HeadIter->PlayerResponses[ Iter ] ) );
  }

  HeadIter->PlayerResponses.push_back( std::make_tuple( OptionType, std::string{ PlayerText },
                                                        KarmaContainer{ NeededKarma, KeyType{ NeededKeys }, ResultingKarma },
                                                        NPC_PLAYER_Pairs{ "" } ) );
}

///////////////////////////////////////////////////////////////////////////////

constexpr KarmaTree::Responses::ConstIter::ConstIter( TreeText::ResponseContainer::const_iterator &&ConstIter ) noexcept :
  IterIndex( std::move( ConstIter ) )
{
}

bool operator!=( const TreeText::ResponseContainer::const_iterator &Lhs, const KarmaTree::Responses::ConstIter &Rhs ) noexcept
{
  return Lhs != Rhs.IterIndex;
}

///////////////////////////////////////////////////////////////////////////////

KarmaTree::Responses::Iter::Iter( TreeText::ResponseContainer::const_iterator &&Iter ) noexcept : IterIndex( std::move( Iter ) )
{
}

const KarmaTree::Responses::Iter::ReturnType KarmaTree::Responses::Iter::operator*() const noexcept
{
  return std::make_pair( std::get< std::string >( *IterIndex ), std::get< KarmaContainer >( *IterIndex ).Third );
}


bool KarmaTree::Responses::Iter::operator!=( const ConstIter& Rhs ) const noexcept
{
  return IterIndex != Rhs;
}

void KarmaTree::Responses::Iter::operator++() noexcept
{
  ++IterIndex;
}

const TreeText::ResponseType KarmaTree::Responses::Iter::GetCurrentResponse() const noexcept
{
  return *IterIndex;
}

///////////////////////////////////////////////////////////////////////////////

constexpr KarmaTree::Responses::Responses( const TreeText::ResponseContainer &Response ) noexcept : PlayerResponses( Response )
{
}

KarmaTree::Responses::Iter KarmaTree::Responses::begin() const noexcept
{
  return Iter{ PlayerResponses.cbegin() };
}

const KarmaTree::Responses::ConstIter KarmaTree::Responses::end() const noexcept
{
  return ConstIter{ PlayerResponses.cend() };
}

///////////////////////////////////////////////////////////////////////////////

KarmaTree::InvalidOptionNumber::InvalidOptionNumber( size_t Number, size_t Amount ) noexcept : 
What( "You tried to select option: " + std::to_string( Number ) +
      ( Number == 0 ? ", but the options start at 1!" : ( std::string{ ", but there" } +( Amount == 1 ? " is " : " are " ) + "only: " +
                                                          std::to_string( Amount ) + " option" + ( Amount == 1 ? "!" : "s!" ) ) ) )
{
}

///////////////////////////////////////////////////////////////////////////////

KarmaTree::KarmaTree( KarmaTreeCreator &&Move ) noexcept : Head( std::move( Move.Head ) ), BadThreshold( Move.BadThreshold ),
                                                           GoodThreshold( Move.GoodThreshold )
{
}

KarmaTree::KarmaTree( KarmaTree &&Move ) noexcept : Head( std::move( Move.Head ) ), BadThreshold( Move.BadThreshold ),
                                                    GoodThreshold( Move.GoodThreshold ), BadKeys( std::move( Move.BadKeys ) ),
                                                    NeutralKeys( std::move( Move.NeutralKeys ) ), GoodKeys( std::move( Move.GoodKeys ) )
{
}

const std::string & KarmaTree::GetNPCText() const noexcept
{
  return CurrentText->NPC;
}

const KarmaTree::Responses KarmaTree::GetPlayerOptions( int Karma ) const noexcept
{
  CurrentOptions.clear();

  CurrentOptions.reserve( CurrentText->PlayerResponses.size() );

  for( auto Iter : CurrentText->PlayerResponses )
  {
    const KarmaContainer &OptionKarma = std::get< KarmaContainer >( Iter );

    if( OptionKarma.First == 0 )
    {
      if( !( OptionKarma.Second.empty() ) )
      {
        bool Unlocked = true;

        for( auto KeyIter : OptionKarma.Second )
        {
          if( NeutralKeys.find( KeyIter ) == NeutralKeys.cend() )
          {
            Unlocked = false;

            break;
          }
        }

        if( Unlocked )
        {
          CurrentOptions.push_back( Iter );
        }
      }
      else
      {
        CurrentOptions.push_back( Iter );
      }
    }
    else if( OptionKarma.First < 0 )
    {
      if( Karma <= OptionKarma.First )
      {
        if( !( OptionKarma.Second.empty() ) )
        {
          if( Karma <= BadThreshold )
          {
            bool Unlocked = true;
            
            for( auto KeyIter : OptionKarma.Second )
            {
              if( BadKeys.find( KeyIter ) == BadKeys.cend() )
              {
                Unlocked = false;

                break;
              }
            }

            if( Unlocked )
            {
              CurrentOptions.push_back( Iter );
            }
          }
          else
          {
            bool Unlocked = true;

            for( auto KeyIter : OptionKarma.Second )
            {
              if( NeutralKeys.find( KeyIter ) == NeutralKeys.cend() )
              {
                Unlocked = false;

                break;
              }
            }

            if( Unlocked )
            {
              CurrentOptions.push_back( Iter );
            }
          }
        }
        else
        {
          CurrentOptions.push_back( Iter );
        }
      }
    }
    else
    {
      if( Karma >= OptionKarma.First )
      {
        if( !( OptionKarma.Second.empty() ) )
        {
          if( Karma >= GoodThreshold )
          {
            bool Unlocked = true;

            for( auto KeyIter : OptionKarma.Second )
            {
              if( GoodKeys.find( KeyIter ) == GoodKeys.cend() )
              {
                Unlocked = false;

                break;
              }
            }

            if( Unlocked )
            {
              CurrentOptions.push_back( Iter );
            }
          }
          else
          {
            bool Unlocked = true;

            for( auto KeyIter : OptionKarma.Second )
            {
              if( NeutralKeys.find( KeyIter ) == NeutralKeys.cend() )
              {
                Unlocked = false;

                break;
              }
            }

            if( Unlocked )
            {
              CurrentOptions.push_back( Iter );
            }
          }
        }
        else
        {
          CurrentOptions.push_back( Iter );
        }
      }
    }
  }

  return Responses{ CurrentOptions };
}

int KarmaTree::GetGoodKarmaThreshold() const noexcept
{
  return GoodThreshold;
}

int KarmaTree::GetBadKarmaThreshold() const noexcept
{
  return BadThreshold;
}

void KarmaTree::SetKarmaKey( int Karma, size_t Key ) noexcept
{
  if( Karma <= BadThreshold )
  {
    BadKeys.emplace( Key );
  }
  else if( Karma >= GoodThreshold )
  {
    GoodKeys.emplace( Key );
  }
  else
  {
    NeutralKeys.emplace( Key );
  }
}

void KarmaTree::RemoveKarmaKey( int Karma, size_t Key ) noexcept
{
  if( Karma <= BadThreshold )
  {
    BadKeys.erase( Key );
  }
  else if( Karma >= GoodThreshold )
  {
    GoodKeys.erase( Key );
  }
  else
  {
    NeutralKeys.erase( Key );
  }
}

void KarmaTree::ClearKarmaKeys( int Karma ) noexcept
{
  if( Karma <= BadThreshold )
  {
    BadKeys.clear();
  }
  else if( Karma >= GoodThreshold )
  {
    GoodKeys.clear();
  }
  else
  {
    NeutralKeys.clear();
  }
}

int KarmaTree::ChooseOption( size_t Number ) const noexcept( false )
{
  try
  {
    CurrentOptionStack.push( CurrentOptions );

    const TreeText::ResponseType *Tuple = &( CurrentOptionStack.top().at( Number ) );

    const DefaultResponseOptions &Return = std::get< DefaultResponseOptions >( *Tuple );

    if( Return != DefaultResponseOptions::ContinueConversation )
    {
      CurrentText = &Head;

      for( size_t i = 0, End = CurrentOptionStack.size(); i != End; ++i )
      {
        CurrentOptionStack.pop();
      }

      return Return;
    }

    CurrentText = &( std::get< TreeText >( *Tuple ) );

    return DefaultResponseOptions::ContinueConversation;
  }
  catch( std::out_of_range )
  {
    throw( InvalidOptionNumber( Number + 1, CurrentOptions.size() ) );
  }
}

int KarmaTree::ChooseOption_NoThrow( size_t Number ) const noexcept
{
  CurrentOptionStack.push( CurrentOptions );

  const TreeText::ResponseType *Tuple = &( CurrentOptionStack.top()[ Number ] );

  const DefaultResponseOptions &Return = std::get< DefaultResponseOptions >( *Tuple );

  if( Return != DefaultResponseOptions::ContinueConversation )
  {
    CurrentText = &Head;

    for( size_t i = 0, End = CurrentOptionStack.size(); i != End; ++i )
    {
      CurrentOptionStack.pop();
    }

    return Return;
  }

  CurrentText = &( std::get< TreeText >( *Tuple ) );

  return DefaultResponseOptions::ContinueConversation;
}

///////////////////////////////////////////////////////////////////////////////

#include <iostream>

namespace KarmaTreeSpace
{
  std::ostream & operator<<( std::ostream &Lhs, const std::string &Rhs ) noexcept
  {
    return Lhs << Rhs.c_str();
  }
}
