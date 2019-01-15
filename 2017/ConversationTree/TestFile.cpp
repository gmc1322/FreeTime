#include <ConversationTree.h>
ConversationTree Test_Tree() noexcept
{
  auto Tree = ConversationTreeCreator( "Test Text." );
  Tree.AddText( "End With Event.", static_cast< DefaultResponseOptions >( 2 ) );
  auto ID0 = Tree.AddText( "End.", "TEst2" );
  Tree.AddText( "end.", DefaultResponseOptions::EndConversation, ID0 );
  auto ID1 = Tree.AddText( "Continue.", "The Pain.", ID0 );
  Tree.AddText( "End.", DefaultResponseOptions::EndConversation, ID1 );
  auto ID2 = Tree.AddText( "Continue.", "The Pain Some More.", ID1 );
  Tree.AddText( "End?", DefaultResponseOptions::EndConversation, ID2 );
  return ConversationTree{ std::move( Tree ) };
}
