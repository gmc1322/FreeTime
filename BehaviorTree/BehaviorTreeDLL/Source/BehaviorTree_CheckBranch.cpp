/*!----------------------------------------------------------------------------
\file   BehaviorTree_CheckBranch.cpp

\author Garrett Conti

\par    Project: BehaviorTreeDLL
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */


#include "BehaviorTree_CheckBranch.h"

#include "BehaviorTree.h"

BehaviorTreeSpace::CheckBranch::CheckBranch( CheckFunctionPtr FunctionPtr, size_t DataStorageKey ) noexcept :
  FunctionPtr_( FunctionPtr ), DataStorageKey_( DataStorageKey )
{
}

BehaviorTreeSpace::CheckBranch::~CheckBranch() noexcept
{
  delete Child;
}

bool BehaviorTreeSpace::CheckBranch::RunCheck( TreeDataStoragePtr DataStoragePtr,
                                               StackClass& ParentStack ) ThrowingElse( noexcept( false ), noexcept )
{ 
  Throwing(
    if( DataStoragePtr->size() == 0 )
    {
      throw( BadDataThrow( DataStoragePtr ) );
    }
  )

  ThrowingElse(
    auto StoragePtr = DataStoragePtr->find( DataStorageKey_ );

    if( StoragePtr == DataStoragePtr->end() )
    {
      throw( BadDataKeyThrow( DataStorageKey_ ) );
    }

    ShouldRunParent = FunctionPtr_( StoragePtr->second );
    ,

    ShouldRunParent = FunctionPtr_( DataStoragePtr->find( DataStorageKey_ )->second );
  )

  if( ShouldRunParent )
  {
    if( ParentStack.Empty() )
    {
      return true;
    }

    ParentStack.Pop();

    NewParentStack.Clear();

    return false;
  }
  
  UnionClassPtr Node;

  if( NewParentStack.Empty() )
  {
    Node = Child;
  }
  else
  {
    Node = NewParentStack.Top();
  }

  switch( Node->GetUnionType() )
  {
    case UnionClass::BehaviorTreeType:
    {
      auto Tree = Node->GetTree();

      if( !( Tree->RunBehaviorTree() ) )
      {
        Tree->ResetBehaviorTree();

        NewParentStack.Pop();
      }

      break;
    }

    case UnionClass::MultiBranchType:
      Node->GetBranch()->RunBranch( DataStoragePtr, NewParentStack );
      break;

    case UnionClass::ThreadedType:
      Node->GetThreaded()->RunThreaded( DataStoragePtr, NewParentStack );
      break;
  }

  return false;
}
