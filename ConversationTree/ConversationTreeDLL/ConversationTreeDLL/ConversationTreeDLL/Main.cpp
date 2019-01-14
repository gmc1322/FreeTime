#include <iostream>

#include "ConversationTree.h"

ConversationTree MakeTree()
{
  ConversationTreeCreator This{ "Hello!\nHow are you?" };

  auto ID1 = This.AddText( "Good", "Im glad your having a good day!" );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID1 );

  auto ID2 = This.AddText( "Ok", "Well I hope your day gets better!" );
  auto ID2_1 = This.AddText( "Why thank you for your consideration!", "Any time!", ID2 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2_1 );

  This.AddText( "Bad" );
  This.AddText( "Get outa my sight!", DefaultResponseOptions::CustomOption );

  return ConversationTree{ std::move( This ) };
}

ConversationTree Test_Tree() noexcept
{
  auto Tree = ConversationTreeCreator( "Test Text." );
  Tree.AddText( "End With Event.", static_cast< DefaultResponseOptions >( 2 ) );
  Tree.AddText( "End.", DefaultResponseOptions::EndConversation );
  auto ID0 = Tree.AddText( "Continue.", "The Pain." );
  Tree.AddText( "End.", DefaultResponseOptions::EndConversation, ID0 );
  auto ID1 = Tree.AddText( "Continue.", "The Pain Some More.", ID0 );
  Tree.AddText( "End?", DefaultResponseOptions::EndConversation, ID1 );
  return ConversationTree{ std::move( Tree ) };
}

using namespace ConversationTreeSpace;

int main()
{
  const ConversationTree Tree = MakeTree();

  for( ; ; )
  {
    DefaultResponseOptions Result;
    do
    {
      std::cout << Tree.GetNPCText() << std::endl << std::endl;

      size_t Count = 0;
      for( auto Iter : Tree.GetPlayerOptions() )
      {
        std::cout << "Option " << ++Count << ": " << Iter << std::endl;
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
      catch( ConversationTree::InvalidOptionNumber &Error )
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
      std::cout << "Egad! He killed you!" << std::endl;
    }

    char End;

    std::cout << "Type 'e' at any time to quit the program." << std::endl << std::endl;

    scanf_s( "%c", &End, 2 );

    if( End == 'e' )
    {
      return 0;
    }
  }
}
