/*!----------------------------------------------------------------------------
\file   BehaviorTree_ChildTree.h

\author Garrett Conti

\par    Project: BehaviorTreeDLL
\par    Course:  S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "BehaviorTree_ChildTree.h"

using namespace BehaviorTreeSpace;

ChildTree::ChildTree( BehaviorTreePtr Tree ) noexcept : Tree( Tree )
{
}

bool ChildTree::RunChild( TreeDataStoragePtr DataStorage, StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept )
{
  int Return;

  if( !Tree->RunBehaviorTree() )
  {
    Return = Prev;
  }
  else
  {
    Return = Tree->GetReturnValue();
  }

  Throwing(
    if( Return < Prev )
    {
      throw( BadBranchThrow( Return ) );
    }
  )

  if( Return == Prev )
  {
    if( ParentStack.Empty() )
    {
      return true;
    }

    ParentStack.Pop();
  }
  else if( Return > Current )
  {
    Tree->ResetBehaviorTree();

    ParentStack.Push( UnionClass::GetUnion( &ChildBranches, static_cast< size_t >( Return ) ) );

    auto Parent = ParentStack.Top();

    switch( Parent->GetUnionType() )
    {
      case UnionClass::BehaviorTreeType:
      {
        auto ChildTree = Parent->GetTree();

        if( !( ChildTree->RunBehaviorTree() ) )
        {
          ChildTree->ResetBehaviorTree();

          ParentStack.Pop();
        }

        break;
      }

      case UnionClass::MultiBranchType:
        Parent->GetBranch()->RunBranch( DataStorage, ParentStack ); // This should never return false
        break;

      case UnionClass::ThreadedType:
        Parent->GetThreaded()->RunThreaded( DataStorage, ParentStack ); // This should never return false
        break;

      case UnionClass::CheckBranchType:
        Parent->GetCheck()->RunCheck( DataStorage, ParentStack ); // This should never return false
        break;

      case UnionClass::ChildTreeType:
        Parent->GetChild()->RunChild( DataStorage, ParentStack ); 
        break;
    }
  }

  return false;
}
