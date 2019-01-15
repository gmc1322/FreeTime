#include "BehaviorTree_ChildTree.h"

int ResetRunTree( void *Data ) noexcept
{
  auto Tree = static_cast< BehaviorTreeSpace::BehaviorTree * >( Data );

  Tree->ResetBehaviorTree();

  Tree->RunBehaviorTree();

  return BehaviorTreeSpace::SpecialBranchReturns::Current;
}

int Move( void *Data ) noexcept
{
  if( Data )
  {  
  }

  return BehaviorTreeSpace::SpecialBranchReturns::Prev;
}

int Mine( void *Data ) noexcept
{
  if( Data )
  {
  }

  return BehaviorTreeSpace::Current;
}

int GivePower( void *Data ) noexcept
{
  if( Data )
  {
  }

  return BehaviorTreeSpace::Current;
}

int Return( void *Data )
{
  if( Data )
  {
  }

  return BehaviorTreeSpace::Current;
}

int End( void *Data )
{
  static_cast< BehaviorTreeSpace::BehaviorTreePtr >( Data )->EndBehaviorTree();

  return BehaviorTreeSpace::Current;
}

int CheckAll( void *Data ) noexcept
{
  if( Data )
  {
  }

  return BehaviorTreeSpace::Current;
}

int main()
{
  auto Tree = new BehaviorTreeSpace::BehaviorTree( new BehaviorTreeSpace::MultiBranch{ CheckAll, 5 } );
  Tree->AddBranch( Move, 0 );
  Tree->AddBranch( Move, 1 );
  Tree->AddBranch( Move, 2 );
  Tree->AddBranch( Move, 3 );
  Tree->AddBranch( Return, 4 );

  return 0;
}
 