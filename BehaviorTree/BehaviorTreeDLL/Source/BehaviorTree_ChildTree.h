/*!------------------------------------------------------------------------------
\file   BehaviorTree_ChildTree.h

\author Garrett Conti

\par    Project: BehaviorTreeDLL
\par    Course:  S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BehaviorTree.h"

namespace BehaviorTreeSpace
{
  class ChildTree
  {
    public:
      /**
       * \brief The constructor for the Child Tree
       *
       * \param Tree - A pointer to the Tree to run
       */
      BehaviorTreeDLL_API ChildTree( BehaviorTreePtr Tree ) noexcept;

      BehaviorTreeDLL_API ChildTree( const BehaviorTree &Copy ) noexcept = delete;

    public:
      BehaviorTreeDLL_API void operator=( const ChildTree &Copy ) noexcept = delete;

    public:
      /**
       * \brief Runs the Child Tree
       *
       * \exception BadDataThrow    - Your Data Storage is bad!
       * \exception BadDataKeyThrow - The Data Storage Key provided to a Branch was bad!
       * \exception BadBranchThrow  - The Branch execution flow value was bad!
       * 
       * \param DataStorage - The data to use for the children of this branch
       * \param ParentStack - The stack for the parents of this branch
       * 
       * \return void
       */
      BehaviorTreeDLL_API bool RunChild( TreeDataStoragePtr DataStorage, StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept );

    public:
      Branches ChildBranches; // Map containing all of the Nodes connected one deep to this Branch

    private:
      BehaviorTreePtr Tree;
  };
}
