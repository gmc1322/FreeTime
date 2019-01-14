#pragma once

#include "BehaviorTree_Union.h"

namespace BehaviorTreeSpace
{
  class StackClass
  {
    Throwing(
    public:
      struct EmptyStackThrow
      {
        BehaviorTreeDLL_API EmptyStackThrow( const char * ErrorText ) noexcept : ErrorText_( ErrorText )
        {
          std::cerr << ErrorText << std::endl;
        }

        std::string ErrorText_;
      };
    )

    public:
      BehaviorTreeDLL_API StackClass() noexcept;

      BehaviorTreeDLL_API ~StackClass();

    public:
      BehaviorTreeDLL_API void operator=( const StackClass &Copy ) noexcept = delete;

    public:
      BehaviorTreeDLL_API void Push( UnionClassPtr Ptr ) noexcept;

      BehaviorTreeDLL_API void Pop() ThrowingElse( noexcept( false ), noexcept );;

      BehaviorTreeDLL_API UnionClassPtr Top() const ThrowingElse( noexcept( false ), noexcept );;

      BehaviorTreeDLL_API bool Empty() const noexcept;

      BehaviorTreeDLL_API void Clear() noexcept;


    private:
      BehaviorTreeDLL_API void Grow() noexcept;

    private:
      UnionClassPtr *Stack = new UnionClassPtr[ 1 ];

      size_t Size = 0;

      size_t Capacity = 1;
  };

  /**
  * \brief Special return values to control Branch execution flow
  */
  enum SpecialBranchReturns
  {
    End = -3,     // Return this to end the Tree (or call the End funtion to reset it at the same time)
    Prev = -2,    // Return this to go back a Branch
    Current = -1, // Return this to run the current Branch
  };

  typedef int( *BranchFunctionPtr )( void *Data );

  typedef std::map< size_t, void* > TreeDataStorage, *TreeDataStoragePtr;
}
