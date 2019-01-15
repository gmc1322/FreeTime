#pragma once

#include "BehaviorTree_MultiBranch.h"
#include "BehaviorTree_ThreadedBranch.h"
#include "BehaviorTree_CheckBranch.h"

namespace BehaviorTreeSpace
{
  typedef BehaviorTreePtr( *TreeInitFunctionPtr )( );

  typedef void( *FunctionPtr )( );

  Throwing( 
    struct BadThreadAccessThrow
    {
      BehaviorTreeDLL_API BadThreadAccessThrow() noexcept : AccessText( "You tried to add a ThreadedBranch to a ThreadedBranch!" )
      {
        std::cerr << AccessText << std::endl;
      }

      std::string AccessText;
    };

    struct BadCheckBranchAccessThrow
    {
      BehaviorTreeDLL_API BadCheckBranchAccessThrow() noexcept : AccessText( "You tried to add a CheckBranch to a CheckBranch!" )
      {
        std::cerr << AccessText << std::endl;
      }

      std::string AccessText;
    };
    
    struct BadTopBranchThrow
    {
      BehaviorTreeDLL_API BadTopBranchThrow() noexcept : AccessText( "You tried to add a Tree to the TopBranch!" )
      {
        std::cerr << AccessText << std::endl;
      }

      std::string AccessText;
    };
  )

  class BehaviorTree
  {
    public:
      /**
       * \brief The constructor for the Behavior Tree
       *
       * \param Trunk    - The Trunk's branch, this cannot be another tree!
       * \param OnFinish - The function to run when the Tree has finished
       * \param OnReset  - The function to run when the Tree restarts
       */
      BehaviorTreeDLL_API BehaviorTree( UnionClassPtr Trunk, FunctionPtr OnFinish = nullptr,
                                        FunctionPtr OnReset = nullptr ) ThrowingElse( noexcept( false ), noexcept );

      BehaviorTreeDLL_API BehaviorTree( const BehaviorTree &Copy ) noexcept = delete;

      BehaviorTreeDLL_API ~BehaviorTree() noexcept;

    public:
      BehaviorTreeDLL_API void operator=( const BehaviorTree &Copy ) noexcept = delete;

    public:
      /**
       * \brief Runs the Behavior Tree
       *
       * \exception BadDataThrow    - Your Data Storage is bad!
       * \exception BadDataKeyThrow - The Data Storage Key provided to a Branch was bad!
       * \exception BadBranchThrow  - The Branch execution flow value was bad!
       *
       * \return If the Tree is done or not, also calls OnFinsih if done and function is valid
       */
      BehaviorTreeDLL_API bool RunBehaviorTree() ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Resets the Behavior Tree after it has finished so it can run again, also calls OnReset if valid
       *
       * \return void
       */
      BehaviorTreeDLL_API void ResetBehaviorTree() noexcept;

      /**
       * \brief Stops the Behavior Tree and resets it, also calls OnFinish and OnReset if valid
       *
       * \return void
       */
      BehaviorTreeDLL_API void EndBehaviorTree() noexcept;
    
      /**
       * \brief Calls EndBehaviorTree, but also sets a return value to be used by the Child Tree
       *
       * \param ReturnValue - Used like a normal return value of a Multi Branch
       *
       * \return void
       */
      BehaviorTreeDLL_API void ReturnBehaviorTree( int ReturnValue ) noexcept;

    public:
      /**
       * \brief Adds a Branch to the specified parent Branch
       *
       * \param FunctionPtr     - The function for the new Branch to run
       * \param DataStorageKey  - The Key to access the Data Storage, defualt = 0
       * \param IDs             - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddBranch( BranchFunctionPtr FunctionPtr, size_t DataStorageKey,
                                          std::initializer_list< std::initializer_list< size_t > > IDs = { { } } ) 
                                          ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Adds a Tree to the specified parent Branch
       *
       * \param FunctionPtr - The function to initialize the new Tree
       * \param IDs         - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddTree( TreeInitFunctionPtr FunctionPtr,
                                        std::initializer_list< std::initializer_list< size_t > > IDs = { { } } )
                                        ThrowingElse( noexcept( false ), noexcept );
    
      /**
       * \brief Adds a ChildTree to the specified parent Branch
       *
       * \param FunctionPtr - The function to initialize the new ChildTree
       * \param IDs         - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddChildTree( TreeInitFunctionPtr FunctionPtr,
                                             std::initializer_list< std::initializer_list< size_t > > IDs = { { } } )
                                             ThrowingElse( noexcept( false ), noexcept );
      /**
       * \brief Adds a ChildTree to the specified parent Branch
       *
       * \param TreePtr - The Tree to initialize the new ChildTree
       * \param IDs     - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddChildTree( BehaviorTreePtr TreePtr,
                                             std::initializer_list< std::initializer_list< size_t > > IDs = { {} } )
                                             ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Adds a Thread to the specified parent Branch
       *
       * \param IDs - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow   - You tried to add to a child to a Tree Node!
       * \exception BadThreadAccessThrow - You tried to add a ThreadedBranch to a ThreadedBranch!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddThreaded( std::initializer_list< std::initializer_list< size_t > > IDs = { {} } )
                                            ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Adds a Thread to the specified parent Branch
       *
       * \param IDs         - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow   - You tried to add to a child to a Tree Node!
       * \exception BadThreadAccessThrow - You tried to add a ThreadedBranch to a ThreadedBranch!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddThreaded( GeneratorFunctionPtr GenFunction, size_t GenCount, GeneratedFunctionPtr RunFunction,
                                            ThreadedBranch::GeneratorTypes GenType = ThreadedBranch::Constant,
                                            std::initializer_list< std::initializer_list< size_t > > IDs = { {} } )
                                            ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Adds a Check to the specified parent Branch
       *
       * \param FunctionPtr     - The function for the new Check to run
       * \param DataStorageKey  - The Key to access the Data Storage, defualt = 0
       * \param IDs             - The IDs of the parent Branches, default = Trunk
       *
       * \exception BadTreeAccessThrow        - You tried to add to a child to a Tree Node!
       * \exception BadCheckBranchAccessThrow - You tried to add a CheckBranch to a CheckBranch!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddCheck( CheckFunctionPtr FunctionPtr, size_t DataStorageKey = 0,
                                         std::initializer_list< std::initializer_list< size_t > > IDs = { {} } )
                                         ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Adds new Data to the Storage
       *
       * \param DataPtr        - A pointer to the new Data to add
       * \param DataStorageKey - The Key the Data will be accessed by
       * 
       * \exception BadDataKeyThrow - The Key you gave was already in use!
       *
       * \return void
       */
      BehaviorTreeDLL_API void AddData( void *DataPtr, size_t DataStorageKey ) ThrowingElse( noexcept( false ), noexcept );

    public:
      BehaviorTreeDLL_API int GetReturnValue() noexcept;

    private:
      template< typename ChildType > // ChildType must be a Ptr!
      static void AddChild( BranchesPtr ParentBranch, ChildType Child ) noexcept
      {
        ParentBranch->emplace( std::make_pair( ParentBranch->size(), Child ) );
      }

    protected:
      BehaviorTreeDLL_API bool Run( UnionClassPtr Child ) ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Gets the final parent branch
       *
       * \param[in,out] ParentBranch
       * \param ID
       *
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return void
       */
      static BehaviorTreeDLL_API void GetParentBranch( BranchesPtr *ParentBranch, size_t ID ) noexcept( false );
    
      /**
       * \brief 
       *
       * \param Union
       * 
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return BranchesPtr
       */
      static BehaviorTreeDLL_API BranchesPtr GetParentBranch( UnionClassPtr Union ) noexcept( false );

      /**
       * \brief 
       *
       * \param iterOuter - 
       * 
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return BranchesPtr
       */
      BehaviorTreeDLL_API BranchesPtr GetParentUnion( const std::initializer_list< size_t > &iterOuter ) noexcept( false );

      Throwing(
      /**
       * \brief
       *
       * \param iterOuter -
       *
       * \exception BadTreeAccessThrow - You tried to add to a child to a Tree Node!
       *
       * \return BranchesPtr
       */
      BehaviorTreeDLL_API UnionClassPtr GetParentUnionDebug( const std::initializer_list< size_t > &iterOuter ) noexcept( false );
      )

    private:
      struct CheckChildThrow
      {
        BehaviorTreeDLL_API CheckChildThrow( UnionClassPtr *Child ) noexcept : Child_( Child ) {}

        UnionClassPtr *Child_;
      };

    private:
      TreeDataStorage DataStorage;

      FunctionPtr FinishedFunction;

      FunctionPtr ResetFunction;

      StackClass BranchStack;

      UnionClassPtr TopBranch;

      int ReturnValue = Current;
  };
}
