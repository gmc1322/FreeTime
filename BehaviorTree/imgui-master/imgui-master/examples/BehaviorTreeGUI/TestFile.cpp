#include <BehaviorTree.h>
int test1( void *Data );
void * test3();
BehaviorTreeSpace::BehaviorTreePtr test2() noexcept
{
  auto Tree = new BehaviorTreeSpace::BehaviorTree( test1, test3(), 0, nullptr );
  return Tree;
}
