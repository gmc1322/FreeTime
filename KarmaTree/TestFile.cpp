#include <KarmaTree.h>
KarmaTree KarmaTree() noexcept
{
  auto Tree = KarmaTreeCreator{ "Hello!\nHow are you?", -50, 50 };
  auto ID0 = Tree.AddText( "Great actually! How are you!", "Quite well, thank you!", KarmaTreeCreator::DefaultID, 50, {  } );
  Tree.AddText( "End Conversation.", DefaultResponseOptions::EndConversation, ID0, 0, {  } );
  auto ID1 = Tree.AddText( "Good.", "Im glad your having a good day!", KarmaTreeCreator::DefaultID, 25, { 1 } );
  Tree.AddText( "End Conversation.", DefaultResponseOptions::EndConversation, ID1, 0, {  } );
  auto ID2 = Tree.AddText( "OK.", "Well, I hope your day gets better!", KarmaTreeCreator::DefaultID, 0, {  } );
  auto ID3 = Tree.AddText( "Why thank you for your consideration!", "Any time!", , ID2, 0, { 1, 2 } );
  Tree.AddText( "End Conversation.", DefaultResponseOptions::EndConversation, ID3, 0, {  } );
  Tree.AddText( "End Conversation.", DefaultResponseOptions::EndConversation, ID2, 0, {  } );
  Tree.AddText( "Bad.", DefaultResponseOptions::EndConversation, KarmaTreeCreator::DefaultID, -25, { 1 } );
  Tree.AddText( "Get outa my sight!", static_cast< DefaultResponseOptions >( 1 ), KarmaTreeCreator::DefaultID, -50, {  } );
  return KarmaTree{ std::move( Tree ) };
}
