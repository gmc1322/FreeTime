
#include "KarmaTree.h"

KarmaTree Maker() noexcept
{
  KarmaTreeCreator This{ "Hello!\nHow are you?" };

  auto ID0 = This.AddText( "Great actually! How are you!", "Quite well thank you!", KarmaTreeCreator::DefaultID, 50, 50 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID0 );

  auto ID1 = This.AddText( "Good", "Im glad your having a good day!", KarmaTreeCreator::DefaultID, 0, 25, { 1 } );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID1 );

  auto ID2 = This.AddText( "Ok", "Well I hope your day gets better!" );
  auto ID2_1 = This.AddText( "Why thank you for your consideration!", "Any time!", ID2, 15, 0, { 1, 2 } );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2_1 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2 );

  This.AddText( "Bad", KarmaTreeCreator::EndConversation, KarmaTreeCreator::DefaultID, 0, -25, { 1 } );
  This.AddText( "Get outa my sight!", DefaultResponseOptions::CustomOption, KarmaTreeCreator::DefaultID, -50, -50 );

  return KarmaTree{ std::move( This ) };
}

#include <iostream>

using namespace KarmaTreeSpace;

int main()
{
  KarmaTree Tree = Maker();

  for( ; ; )
  {
    int Karma;

    std::cout << "Whats your karma: ";

    scanf_s( "%i", &Karma );

    std::cout << std::endl;

    std::cout << "What Keys do you have: (0 = end)" << std::endl;

    size_t Key;
    do
    {
      scanf_s( "%ui", &Key );

      Tree.SetKarmaKey( Karma, Key );
    }
    while( Key );

    std::cout << std::endl;

    DefaultResponseOptions Result;
    do
    {
      std::cout << Tree.GetNPCText() << std::endl << std::endl;

      size_t Count = 0;
      for( auto Iter : Tree.GetPlayerOptions( Karma ) )
      {
        std::cout << "Option " << ++Count << ": " << Iter.first << " (" << Iter.second << " Karma)" << std::endl;
      }

      std::cout << std::endl;

      size_t Option;
      scanf_s( "%ui", &Option );

      std::cout << std::endl;

#ifdef _DEBUG
      try
      {
        Result = Tree.ChooseOption( Option - 1 );
      }
      catch( KarmaTree::InvalidOptionNumber &Error )
      {
        Result = DefaultResponseOptions::ContinueConversation;

        std::cout << Error.What << std::endl;

        std::cout << std::endl;
      }
#else
      Result = Tree.ChooseOption_NoThrow( Option - 1 );
#endif
    }
    while( Result == DefaultResponseOptions::ContinueConversation );

    if( Result >= DefaultResponseOptions::CustomOption )
    {
      std::cout << "Egad! He killed you!" << std::endl << std::endl;
    }

    char End;

    std::cout << "Type 'e' at any time to quit the program." << std::endl << std::endl;

    scanf_s( "%c", &End, 2 );

    if( End == 'e' )
    {
      return 0;
    }

    Tree.ClearKarmaKeys( 50 );
    Tree.ClearKarmaKeys( 0 );
    Tree.ClearKarmaKeys( -50 );
  }
}
