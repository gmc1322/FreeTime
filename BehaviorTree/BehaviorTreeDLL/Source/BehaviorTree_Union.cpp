
#include "BehaviorTree_ChildTree.h"

using namespace BehaviorTreeSpace;

UnionClass::UnionClass( MultiBranchPtr Branch ) noexcept : Ptr( Branch ), Type( MultiBranchType )
{
}

UnionClass::UnionClass( BehaviorTreePtr Tree ) noexcept : Ptr( Tree ), Type( BehaviorTreeType )
{
}

UnionClass::UnionClass( ThreadedBranchPtr Thread ) noexcept : Ptr( Thread ), Type( ThreadedType )
{
}

UnionClass::UnionClass( CheckBranchPtr Check ) noexcept : Ptr( Check ), Type( CheckBranchType )
{
}

UnionClass::UnionClass( ChildTreePtr Child ) noexcept : Ptr( Child ), Type( ChildTreeType )
{
}

UnionClass::~UnionClass()
{
  switch( Type )
  {
    case BehaviorTreeType:
      delete static_cast< BehaviorTreePtr >( Ptr );
      return;

    case MultiBranchType:
      delete static_cast< MultiBranchPtr >( Ptr );
      return;

    case ThreadedType:
      delete static_cast< ThreadedBranchPtr >( Ptr );
      return;

    case CheckBranchType:
      delete static_cast< CheckBranchPtr >( Ptr );
      return;
    
    case ChildTreeType:
      delete static_cast< ChildTreePtr >( Ptr );
      return;
  }
}

UnionClass::UnionTypes UnionClass::GetUnionType() const noexcept
{
  return Type;
}

MultiBranchPtr UnionClass::GetBranch() const ThrowingElse( noexcept( false ), noexcept )
{
  Throwing( 
    if( Type != MultiBranchType )
    {
      throw( WrongGetTypeThrow( Type, MultiBranchType ) );
    }
  )

  return static_cast< MultiBranchPtr >( Ptr );
}

BehaviorTreePtr UnionClass::GetTree() const ThrowingElse( noexcept( false ), noexcept )
{
  Throwing( 
    if( Type != BehaviorTreeType )
    {
      throw( WrongGetTypeThrow( Type, BehaviorTreeType ) );
    } 
  )

  return static_cast< BehaviorTreePtr >( Ptr );
}

ThreadedBranchPtr UnionClass::GetThreaded() const ThrowingElse( noexcept( false ), noexcept )
{
  Throwing(
    if( Type != ThreadedType )
    {
      throw( WrongGetTypeThrow( Type, ThreadedType ) );
    } 
  )

  return static_cast< ThreadedBranchPtr >( Ptr );
}

CheckBranchPtr UnionClass::GetCheck() const ThrowingElse( noexcept( false ), noexcept )
{
  Throwing( 
    if( Type != CheckBranchType )
    {
      throw( WrongGetTypeThrow( Type, CheckBranchType ) );
    }
  )

  return static_cast< CheckBranchPtr >( Ptr );
}

ChildTreePtr UnionClass::GetChild() const ThrowingElse( noexcept( false ), noexcept )
{
  Throwing(
    if( Type != ChildTreeType )
    {
      throw( WrongGetTypeThrow( Type, ChildTreeType ) );
    }
  )

  return static_cast< ChildTreePtr >( Ptr );
}


UnionClassPtr UnionClass::GetUnion( BranchesPtr Branches, size_t ID ) ThrowingElse( noexcept( false ), noexcept )
{
  ThrowingElse( 
    auto Union = Branches->find( ID );

    if( Union == Branches->end() )
    {
      throw( BadBranchThrow( ID ) );
    }

    return &Union->second;
    ,

    return &Branches->find( ID )->second;
  )
}

#ifndef NoThrow
UnionClass::WrongGetTypeThrow::WrongGetTypeThrow( UnionTypes RealType, UnionTypes Type ) noexcept :
  TypeText( "Wrong get function used!\n" ), RealType_( RealType )
{
  TypeText.append( "Real type: " );

  switch( RealType )
  {
    case MultiBranchType:
      TypeText.append( "Branch.\n" );
      break;

    case BehaviorTreeType:
      TypeText.append( "Tree.\n" );
      break;

    case ThreadedType:
      TypeText.append( "Threaded.\n" );
      break;

    case CheckBranchType:
      TypeText.append( "Check.\n" );
      break;
  }

  TypeText.append( "Type used: " );

  switch( Type )
  {
    case MultiBranchType:
      TypeText.append( "Branch!" );
      break;

    case BehaviorTreeType:
      TypeText.append( "Tree!" );
      break;

    case ThreadedType:
      TypeText.append( "Threaded!" );
      break;

    case CheckBranchType:
      TypeText.append( "Check!\n" );
      break;
  }

  std::cerr << TypeText << std::endl;
}
#endif
