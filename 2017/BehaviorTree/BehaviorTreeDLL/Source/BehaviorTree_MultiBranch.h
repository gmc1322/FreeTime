#pragma once

#include "BehaviorTree_Stack.h"

namespace BehaviorTreeSpace
{
  class MultiBranch
  {
    public:
      /**
       * \brief The constructor for the Multi Branch
       *
       * \param FunctionPtr         - The function for the Branch to run
       * \param DataStorageKey      - The Key to the Data Storage for the Branch to use, default = 0
       */
      BehaviorTreeDLL_API MultiBranch( BranchFunctionPtr FunctionPtr, size_t DataStorageKey = 0 ) noexcept;

    public:
      /**
       * \brief Runs the Branch's execution flow
       *
       * \param DataStoragePtr - The Data Storage to access
       * \param ParentStack    - The stack for the parents of this branch
       *
       * \exception BadDataThrow    - Your Data Storage is bad!
       * \exception BadDataKeyThrow - The Data Storage Key provided to a Branch was bad!
       * \exception BadBranchThrow  - The Branch execution flow value was bad!
       *
       * \return If we are done running or not
       */
      BehaviorTreeDLL_API bool RunBranch( TreeDataStoragePtr DataStoragePtr, 
                                          StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept );

    public:
      Branches MultiBranches; // Map containing all of the Nodes connected one deep to this Branch

    protected:
      int BranchReturn_ = Current; // The return value to know which Branch to run. If -1 run this Branch, if -2 run prev

    private:
      BranchFunctionPtr FunctionPtr_; // What to run when this Branch is selected

      const size_t DataStorageKey_; // Key to know which Data Storage to use
  };
}
