#include <BehaviorTree.h>
int ExecuteDown( void *Data );
void * GetLeft();
BehaviorTreeSpace::BehaviorTreePtr CheckAllTree();
int Move( void *Data );
int Return( void *Data );
void * GetUp();
void * GetRight();
void * GetDown();
void * GetReturn0();
void * GetReturn1();
void * GetReturn2();
void * GetReturn3();
BehaviorTreeSpace::BehaviorTreePtr CheckMove() noexcept
{
  auto Tree = new BehaviorTreeSpace::BehaviorTree( ExecuteDown, GetLeft(), 0, nullptr );
  Tree->AddChild( CheckAllTree) );
  Tree->AddBranch( Move, 0, { { 0 } } );
  Tree->AddBranch( Move, 1, { { 0 } } );
  Tree->AddBranch( Move, 2, { { 0 } } );
  Tree->AddBranch( Move, 3, { { 0 } } );
  Tree->AddBranch( Return, 4, { { 0, 0 } } );
  Tree->AddBranch( Return, 5, { { 0, 1 } } );
  Tree->AddBranch( Return, 6, { { 0, 2 } } );
  Tree->AddBranch( Return, 7, { { 0, 3 } } );
  Tree->AddData( GetUp(), 1 );
  Tree->AddData( GetRight(), 2 );
  Tree->AddData( GetDown(), 3 );
  Tree->AddData( GetReturn0(), 4 );
  Tree->AddData( GetReturn1(), 5 );
  Tree->AddData( GetReturn2(), 6 );
  Tree->AddData( GetReturn3(), 7 );
  return Tree;
}
