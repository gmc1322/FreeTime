#include "BehaviorTree.h"

using namespace BehaviorTreeSpace;

MultiBranch::MultiBranch( BranchFunctionPtr FunctionPtr, size_t DataStorageKey ) noexcept :
  FunctionPtr_( FunctionPtr ), DataStorageKey_( DataStorageKey )
{
}

bool MultiBranch::RunBranch( TreeDataStoragePtr DataStoragePtr, StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept )
{
  if( FunctionPtr_ )
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

      BranchReturn_ = FunctionPtr_( StoragePtr->second );
      ,

      BranchReturn_ = FunctionPtr_( DataStoragePtr->find( DataStorageKey_ )->second );
    )

    Throwing( 
      if( BranchReturn_ < Prev )
      {
        throw( BadBranchThrow( BranchReturn_ ) );
      } 
    )

    if( BranchReturn_ == Prev )
    {
      if( ParentStack.Empty() )
      {
        return true;
      }

      ParentStack.Pop();
    }
    else if( BranchReturn_ > Current )
    {
      ParentStack.Push( UnionClass::GetUnion( &MultiBranches, static_cast< size_t >( BranchReturn_ ) ) );
    }

    BranchReturn_ = Current;
  }
  else
  {
    ParentStack.Pop();
  }

  return false;
}
