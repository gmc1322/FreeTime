#include <BehaviorTree.h>
int 1( void *Data );
void * 5();
int 4( void *Data );
int 3( void *Data );
BehaviorTreeSpace::BehaviorTreePtr AutoGen() noexcept
{
  auto Tree = new BehaviorTreeSpace::BehaviorTree( 1, 5(), 0, nullptr );
  Tree->AddBranch( 4, 0, { 1, 4294967295 } );
  Tree->AddBranch( 3, 0 );
  return Tree;
}
