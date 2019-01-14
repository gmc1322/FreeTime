/*!------------------------------------------------------------------------------
\file   BehaviorTree_ThreadedBranch.h

\author Garrett Conti

\par    Project: BehaviorTreeDLL
\par    Course:  GAMS17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include <vector>

#include "BehaviorTree_Stack.h"

namespace BehaviorTreeSpace
{
  typedef void *( *GeneratorFunctionPtr )( );
  typedef void( *GeneratedFunctionPtr )( void * );

  class ThreadedBranch
  {
    public:
      Throwing( 
        struct BadThreadThrow
        {
          BehaviorTreeDLL_API BadThreadThrow() noexcept : ThreadText( "Failed to create thread!" )
          {
            std::cerr << ThreadText << std::endl;
          }

          std::string ThreadText;
        };
      )

    public:
      enum GeneratorTypes
      {
        Constant,    // One time generation
        Dynamic,     // Re-generates every time the Node is reset
        Regenerating // Re-generates every loop
      };

    public:
      /**
      * \brief Constructs a Threaded Branch, with no generator
       */
      BehaviorTreeDLL_API ThreadedBranch() noexcept;

      /**
       * \brief Constructs a Threaded Branch, with a generator
       *
       * \param GenFunction - The function to generate data
       * \param GenCount    - How many instances of the Run Function to use
       * \param RunFunction - The function getting the generated data
       * \param GenType     - The type of generation to use
       */
      BehaviorTreeDLL_API ThreadedBranch( GeneratorFunctionPtr GenFunction, size_t GenCount, GeneratedFunctionPtr RunFunction,
                                          GeneratorTypes GenType ) noexcept;

      BehaviorTreeDLL_API ~ThreadedBranch();

    public:
      /**
       * \brief Runs the Branch's execution flow
       *
       * \param DataStoragePtr - The Data Storage to access
       * \param ParentStack    - The stack for the parents of this branch
       *
       * \exception BadBranchThrow  - The Branch execution flow value was bad!
       * \exception BadThreadThrow  - Fail to create a thread!
       */
      BehaviorTreeDLL_API bool RunThreaded( TreeDataStoragePtr DataStoragePtr,
                                            StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept );

    public:
      Branches ThreadBranches; // Map containg all of the Nodes connected one deep to this Branch

    private:
      static BehaviorTreeDLL_API unsigned long __stdcall MainThreadFunction( void *Data ) ThrowingElse( noexcept( false ), noexcept );
      static BehaviorTreeDLL_API unsigned long __stdcall ChildThreadFunction( void *Data ) ThrowingElse( noexcept( false ), noexcept );
      static BehaviorTreeDLL_API unsigned long __stdcall GeneratedThreadFunction( void *Data) ThrowingElse( noexcept( false ), noexcept );

    private:
      struct ChildThreadData
      {
        UnionClassPtr Union;

        TreeDataStoragePtr Storage;

        StackClass *Stack;
      };

      struct MainThreadData
      {
        ChildThreadData Data;

        int Return = 0;
      };

      struct GeneratedThreadData
      {
        GeneratedFunctionPtr RunFunction;

        void *Data;
      };

    private:
      std::vector< StackClass > Stacks;

      void* *Threads = nullptr;

      ChildThreadData *ChildData = nullptr;

    private:
      GeneratorFunctionPtr GenFunction_;
      size_t GenCount_;
      GeneratorTypes GenType_;
      GeneratedThreadData *GenData;
  };
}
