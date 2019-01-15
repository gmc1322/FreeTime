/*!------------------------------------------------------------------------------
\file   BehaviorTree_CheckBranch.h

\author Garrett Conti

\par    Project: BehaviorTreeDLL
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BehaviorTree_Stack.h"

namespace BehaviorTreeSpace
{
  typedef bool( *CheckFunctionPtr )( void *Data );

  class CheckBranch
  {
    public:
      /**
       * \brief The constructor for the Check Branch
       *
       * \param FunctionPtr    - The function for the Branch to run
       * \param DataStorageKey - The Key to the Data Storage for the Branch to use, default = 0
       */
      BehaviorTreeDLL_API CheckBranch( CheckFunctionPtr FunctionPtr, size_t DataStorageKey = 0 ) noexcept;

      BehaviorTreeDLL_API ~CheckBranch( ) noexcept;

    public:
      /**
       * \brief Runs the Branch's execution flow and its children seperatly
       *
       * \param DataStoragePtr - The Data Storage to access
       * \param ParentStack    - The stack for the parents of this branch
       *
       * \exception BadDataThrow    - Your Data Storage is bad!
       * \exception BadDataKeyThrow - The Data Storage Key provided was bad!
       *
       * \return If we are done running or not
       */
      BehaviorTreeDLL_API bool RunCheck( TreeDataStoragePtr DataStoragePtr, 
                                         StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept );
    public:
      UnionClassPtr Child = nullptr; // The Check Node's only child

    private:
      bool ShouldRunParent = false; // If false: run child. If true: run parent

      CheckFunctionPtr FunctionPtr_; // The Check Function to run

      const size_t DataStorageKey_; // Key to know which Data Storage to use

    private:
      StackClass NewParentStack; // The Check Node's Parent Stack
  };
}
