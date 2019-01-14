#pragma once

#include <iostream>
#include <string>

#include <map>

#include "SpecialDefines.h"

namespace BehaviorTreeSpace
{
  typedef class BehaviorTree   *BehaviorTreePtr;
  typedef const BehaviorTree   * const DoubleConstBehaviorTreePtr;
  typedef class ChildTree      *ChildTreePtr;
  typedef const ChildTree      * const DoubleConstChildTreePtr;

  typedef class MultiBranch    *MultiBranchPtr;
  typedef class ThreadedBranch *ThreadedBranchPtr;
  typedef class CheckBranch    *CheckBranchPtr;

  typedef class UnionClass     *UnionClassPtr;

  typedef std::map< size_t, UnionClass > Branches, *BranchesPtr;

  class UnionClass
  {
    public:
      enum UnionTypes
      {
        MultiBranchType,
        BehaviorTreeType,
        ThreadedType,
        CheckBranchType,
        ChildTreeType
      };

    Throwing(
    public:
      struct WrongGetTypeThrow
      {
        BehaviorTreeDLL_API WrongGetTypeThrow( UnionTypes RealType, UnionTypes Type ) noexcept;

        std::string TypeText;

        UnionTypes RealType_;
      };
    )

    public:
      /**
       * \brief Constructs a Union with the MultiBranch variable type
       *
       * \param Branch - A pointer to the MultiBranch to add to the Union
       */
      BehaviorTreeDLL_API UnionClass( MultiBranchPtr Branch ) noexcept;

      /**
       * \brief Constructs a Union with the BehaviorTree variable type
       *
       * \param Tree - A pointer to the BehaviorTree to add to the Union
       */
      BehaviorTreeDLL_API UnionClass( BehaviorTreePtr Tree ) noexcept;

      /**
       * \brief Constructs a Union with the ThreadedBranch variable type
       *
       * \param Thread - A pointer to the ThreadedBranch to add to the Union
       */
      BehaviorTreeDLL_API UnionClass( ThreadedBranchPtr Thread ) noexcept;
    
      /**
       * \brief Constructs a Union with the CheckBranch variable type
       *
       * \param Check - A pointer to the CheckBranch to add to the Union
       */
      BehaviorTreeDLL_API UnionClass( CheckBranchPtr Check ) noexcept;
    
      /**
       * \brief Constructs a Union with the ChildTree variable type
       *
       * \param Child - A pointer to the ChildTree to add to the Union
       */
      BehaviorTreeDLL_API UnionClass( ChildTreePtr Child ) noexcept;

      BehaviorTreeDLL_API UnionClass( const UnionClass &Copy ) noexcept = delete;

      BehaviorTreeDLL_API ~UnionClass();

    public:
      BehaviorTreeDLL_API void operator=( const UnionClass &Copy ) noexcept = delete;

    public:
      /**
       * \brief Returns the type the Union is using
       *
       * \return The type of the Union
       */
      BehaviorTreeDLL_API UnionTypes GetUnionType() const noexcept;

     /**
       * \brief Returns a pointer to the MultiBranch contained in the Union
       *
       * \exception WrongGetTypeThrow - You tried to get a MultiBranch but the Union contains a diffrent Object!
       *
       * \return A pointer to the MultiBranch
       */
      BehaviorTreeDLL_API MultiBranchPtr GetBranch() const ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Returns a pointer to the BehaviorTree contained in the Union
       *
       * \exception WrongGetTypeThrow - You tried to get a BehaviorTree but the Union contains a diffrent Object!
       *
       * \return A pointer to the BehaviorTree
       */
      BehaviorTreeDLL_API BehaviorTreePtr GetTree() const ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Returns a pointer to the ThreadedBranch contained in the Union
       *
       * \exception WrongGetTypeThrow - You tried to get a ThreadedBranch but the Union contains a diffrent Object!
       *
       * \return A pointer to the ThreadedBranch
       */
      BehaviorTreeDLL_API ThreadedBranchPtr GetThreaded() const ThrowingElse( noexcept( false ), noexcept );
    
      /**
       * \brief Returns a pointer to the CheckBranch contained in the Union
       *
       * \exception WrongGetTypeThrow - You tried to get a CheckBranch but the Union contains a diffrent Object!
       *
       * \return A pointer to the CheckBranch
       */
      BehaviorTreeDLL_API CheckBranchPtr GetCheck() const ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Returns a pointer to the CheckBranch contained in the Union
       *
       * \exception WrongGetTypeThrow - You tried to get a ChildTree but the Union contains a diffrent Object!
       *
       * \return A pointer to the CheckBranch
       */
      BehaviorTreeDLL_API ChildTreePtr GetChild() const ThrowingElse( noexcept( false ), noexcept );

      /**
       * \brief Returns the Union at the specified ID, uses the provided Branch Container to find the Union
       *
       * \param ID - The ID for the Union
       *
       * \exception BadBranchThrow - The ID was bad!
       *
       * \return A pointer to the Union
       */
      static BehaviorTreeDLL_API UnionClassPtr GetUnion( BranchesPtr Branches,
                                                         size_t ID ) ThrowingElse( noexcept( false ), noexcept );

    private:
      void *Ptr;

      UnionTypes Type;
  };

  Throwing( 
    struct BadBranchThrow
    {
      BehaviorTreeDLL_API BadBranchThrow( size_t BranchReturn ) noexcept :
      BranchText( "Bad Branch: " + std::to_string( BranchReturn ) )
      {
        std::cerr << BranchText << std::endl;
      }

      std::string BranchText;
    };

    struct BadDataKeyThrow
    {
      BehaviorTreeDLL_API BadDataKeyThrow( size_t Key ) noexcept : DataKeyText( "Bad Data Key: " + std::to_string( Key ) )
      {
        std::cerr << DataKeyText << std::endl;
      }

      BehaviorTreeDLL_API BadDataKeyThrow( size_t Key, const char *Error ) noexcept : DataKeyText( Error + std::to_string( Key ) )
      {
        std::cerr << DataKeyText << std::endl;
      }

      std::string DataKeyText;
    };

    struct BadDataThrow
    {
      BehaviorTreeDLL_API BadDataThrow( void *Data ) noexcept :
      DataText( "Bad Data at address: " + std::to_string( ( unsigned long long )Data ) )
      {
        std::cerr << DataText << std::endl;
      }

      std::string DataText;
    };

    struct BadTreeAccessThrow
    {
      BehaviorTreeDLL_API BadTreeAccessThrow( const char *String ) noexcept : TreeText( "Bad Tree Access: " )
      {
        TreeText.append( String );

        std::cerr << TreeText << std::endl;
      }

      std::string TreeText;
    };
  )
}
