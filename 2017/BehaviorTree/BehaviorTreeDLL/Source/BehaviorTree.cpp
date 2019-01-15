
#include "BehaviorTree_ChildTree.h"

using namespace BehaviorTreeSpace;

bool BehaviorTree::Run( UnionClassPtr Child ) ThrowingElse( noexcept( false ), noexcept )
{
  switch( Child->GetUnionType() )
  {
    case UnionClass::MultiBranchType:
      return Child->GetBranch()->RunBranch( &DataStorage, BranchStack );

    case UnionClass::ThreadedType:
      return Child->GetThreaded()->RunThreaded( &DataStorage, BranchStack );

    case UnionClass::CheckBranchType:
      return Child->GetCheck()->RunCheck( &DataStorage, BranchStack );
    
    case UnionClass::ChildTreeType:
      return Child->GetChild()->RunChild( &DataStorage, BranchStack );

    case UnionClass::BehaviorTreeType:
    {
      auto Tree = Child->GetTree();

      if( !( Tree->RunBehaviorTree() ) )
      {
        Tree->ResetBehaviorTree();

        BranchStack.Pop();
      }

      break;
    }
  }

  return false;
}

void BehaviorTree::GetParentBranch( BranchesPtr *ParentBranch, size_t ID ) noexcept( false )
{
  try
  {
    *ParentBranch = GetParentBranch( UnionClass::GetUnion( *ParentBranch, ID ) );
  }
  catch( CheckChildThrow ChildThrow )
  {
    if( *ChildThrow.Child_ )
    {
      *ParentBranch = GetParentBranch( *ChildThrow.Child_ );
    }
    else
    {
      throw( ChildThrow );
    }
  }
}

BranchesPtr BehaviorTree::GetParentBranch( UnionClassPtr Union ) noexcept( false )
{
  switch( Union->GetUnionType() )
  {
    case UnionClass::MultiBranchType:
      return &( Union->GetBranch()->MultiBranches );

    Throwing(
    case UnionClass::BehaviorTreeType: 
      throw( BadTreeAccessThrow( "Can't add a child to a Tree Node!" ) );
    )

    case UnionClass::ThreadedType:
      return &( Union->GetThreaded()->ThreadBranches );
    
    case UnionClass::ChildTreeType:
      return &( Union->GetChild()->ChildBranches );

    case UnionClass::CheckBranchType:
      throw( CheckChildThrow( &( Union->GetCheck()->Child ) ) );
  }

  return nullptr;
}

BranchesPtr BehaviorTree::GetParentUnion( const std::initializer_list< size_t > &iterOuter ) noexcept( false )
{
  BranchesPtr ParentBranch = GetParentBranch( TopBranch );

  for( auto iterInner : iterOuter )
  {
    GetParentBranch( &ParentBranch, iterInner );
  }

  return ParentBranch;
}

#ifndef NoThrow
UnionClassPtr BehaviorTree::GetParentUnionDebug( const std::initializer_list< size_t > &iterOuter ) noexcept( false )
{
  if( empty( iterOuter ) )
  {
    return TopBranch;
  }

  BranchesPtr ParentBranch = GetParentBranch( TopBranch );

  auto iterBegin = iterOuter.begin(), iterEnd = iterOuter.end();

  --iterEnd;

  for( ; iterBegin != iterEnd; ++iterBegin )
  {
    GetParentBranch( &ParentBranch, *iterBegin );
  }

  return UnionClass::GetUnion( ParentBranch, *iterBegin );
}
#endif

BehaviorTree::BehaviorTree( UnionClassPtr Trunk, FunctionPtr OnFinish, FunctionPtr OnReset ) ThrowingElse( noexcept( false ), noexcept ) :
  FinishedFunction( OnFinish ), ResetFunction( OnReset ), TopBranch( Trunk )
{
  Throwing(
    if( Trunk->GetUnionType() == UnionClass::BehaviorTreeType )
    {
      throw( BadTopBranchThrow{} );
    }
  )
}

BehaviorTree::~BehaviorTree() noexcept
{
  delete TopBranch;
}

bool BehaviorTree::RunBehaviorTree() ThrowingElse( noexcept( false ), noexcept )
{
  if( !BranchStack.Empty() )
  {
    Run( BranchStack.Top() );
  }
  else if( Run( TopBranch ) || ReturnValue == End )
  {
    ReturnValue = Current;

    if( FinishedFunction )
    {
      FinishedFunction();
    }

    return false; // Finished
  }

  return true; // Not Finished
}

void BehaviorTree::ResetBehaviorTree() noexcept
{
  BranchStack.Clear();

  if( ResetFunction )
  {
    ResetFunction();
  }
}

void BehaviorTree::EndBehaviorTree() noexcept
{
  ResetBehaviorTree();

  if( FinishedFunction )
  {
    FinishedFunction();
  }

  ReturnValue = End;
}

void BehaviorTree::ReturnBehaviorTree( int ReturnValue_ ) noexcept
{
  EndBehaviorTree();

  ReturnValue = ReturnValue_;
}

void BehaviorTree::AddBranch( BranchFunctionPtr FunctionPtr, size_t DataStorageKey, 
                              std::initializer_list< std::initializer_list< size_t > > IDs ) ThrowingElse( noexcept( false ), noexcept )
{
  auto Branch = new MultiBranch( FunctionPtr, DataStorageKey );

  for( auto iterOuter : IDs )
  {
    try
    {
      AddChild( GetParentUnion( iterOuter ), Branch );
    }
    catch ( CheckChildThrow ChildThrow )
    {
      *ChildThrow.Child_ = new UnionClass( Branch );
    }
  }
}

void BehaviorTree::AddTree( TreeInitFunctionPtr FunctionPtr,
                            std::initializer_list< std::initializer_list< size_t > > IDs ) ThrowingElse( noexcept( false ), noexcept )
{
  auto Tree = FunctionPtr();

  for( auto iterOuter : IDs )
  {
    try
    {
      AddChild( GetParentUnion( iterOuter ), Tree );
    }
    catch( CheckChildThrow ChildThrow )
    {
      *ChildThrow.Child_ = new UnionClass( Tree );
    }
  }
}

void BehaviorTree::AddChildTree( TreeInitFunctionPtr FunctionPtr, 
                                 std::initializer_list< std::initializer_list< size_t > > IDs ) ThrowingElse( noexcept( false ), noexcept )
{
  AddChildTree( FunctionPtr(), IDs );
}

void BehaviorTree::AddChildTree( BehaviorTreePtr TreePtr, 
                                 std::initializer_list< std::initializer_list< size_t > > IDs ) ThrowingElse( noexcept( false ), noexcept )
{
  auto ChildTree = new ::ChildTree{ TreePtr };

  for( auto iterOuter : IDs )
  {
    try
    {
      AddChild( GetParentUnion( iterOuter ), ChildTree );
    }
    catch( CheckChildThrow ChildThrow )
    {
      *ChildThrow.Child_ = new UnionClass( ChildTree );
    }
  }
}

void BehaviorTree::AddThreaded( std::initializer_list< std::initializer_list< size_t > > IDs ) ThrowingElse( noexcept( false ), noexcept )
{
  auto ThreadBranch = new ThreadedBranch();

  for( auto iterOuter : IDs )
  {
    ThrowingElse(
      auto Parent = GetParentUnionDebug( iterOuter );

      if( Parent->GetUnionType() == UnionClass::ThreadedType )
      {
        throw( BadThreadAccessThrow() );
      }

      try
      {
        AddChild( GetParentBranch( Parent ), ThreadBranch );
      }
      catch( CheckChildThrow ChildThrow )
      {
        *ChildThrow.Child_ = new UnionClass( ThreadBranch );
      }

      ,

      try
      {
        AddChild( GetParentUnion( iterOuter ), ThreadBranch );
      }
      catch( CheckChildThrow ChildThrow )
      {
        *ChildThrow.Child_ = new UnionClass( ThreadBranch );
      }
    )
  }
}

void BehaviorTree::AddThreaded( GeneratorFunctionPtr GenFunction, size_t GenCount, GeneratedFunctionPtr RunFunction,
                                ThreadedBranch::GeneratorTypes GenType, std::initializer_list< std::initializer_list< size_t > > IDs )
                                ThrowingElse( noexcept( false ), noexcept )
{
  auto ThreadBranch = new ThreadedBranch( GenFunction, GenCount, RunFunction, GenType );

  for( auto iterOuter : IDs )
  {
    ThrowingElse(
      auto Parent = GetParentUnionDebug( iterOuter );

      if( Parent->GetUnionType() == UnionClass::ThreadedType )
      {
        throw( BadThreadAccessThrow() );
      }

      try
      {
        AddChild( GetParentBranch( Parent ), ThreadBranch );
      }
      catch( CheckChildThrow ChildThrow )
      {
        *ChildThrow.Child_ = new UnionClass( ThreadBranch );
      }

      ,

      try
      {
        AddChild( GetParentUnion( iterOuter ), ThreadBranch );
      }
      catch( CheckChildThrow ChildThrow )
      {
        *ChildThrow.Child_ = new UnionClass( ThreadBranch );
      }
    )
  }
}

void BehaviorTree::AddCheck( CheckFunctionPtr FunctionPtr, size_t DataStorageKey,
                             std::initializer_list< std::initializer_list< size_t > > IDs ) ThrowingElse( noexcept( false ), noexcept )
{
  auto CheckBranchHolder = new CheckBranch{ FunctionPtr, DataStorageKey };

  for( auto iterOuter : IDs )
  {
    ThrowingElse(
      auto Parent = GetParentUnionDebug( iterOuter );

      if( Parent->GetUnionType() == UnionClass::CheckBranchType )
      {
        throw( BadCheckBranchAccessThrow() );
      }

      AddChild( GetParentBranch( Parent ), CheckBranchHolder );

      ,

      AddChild( GetParentUnion( iterOuter ), CheckBranchHolder );
    )
  }
}

void BehaviorTree::AddData( void* DataPtr, size_t DataStorageKey ) ThrowingElse( noexcept( false ), noexcept )
{
  Throwing( 
    if( DataStorage.find( DataStorageKey ) != DataStorage.end() )
    {
      throw( BadDataKeyThrow( DataStorageKey, "Key alread exists: " ) );
    }
  )

  DataStorage.emplace( std::pair< size_t, void * >( DataStorageKey, DataPtr ) );
}

int BehaviorTree::GetReturnValue() noexcept
{
  int Return = ReturnValue;

  ReturnValue = Current;

  return Return;
}
