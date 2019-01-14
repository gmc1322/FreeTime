/*!----------------------------------------------------------------------------
\file   BehaviorTree_ThreadedBranch.cpp

\author Garrett Conti

\par    Project: BehaviorTreeDLL
\par    Course:  GAMS17

\copyright 
   ---------------------------------------------------------------------------- */

#include <Windows.h>

#include "BehaviorTree.h"

using namespace BehaviorTreeSpace;

ThreadedBranch::ThreadedBranch() noexcept : GenFunction_( nullptr ), GenCount_( 0 ), GenType_( Constant ), GenData( nullptr )
{
}

ThreadedBranch::ThreadedBranch( GeneratorFunctionPtr GenFunction, size_t GenCount, GeneratedFunctionPtr RunFunction,
                                GeneratorTypes GenType ) noexcept :
  GenFunction_( GenFunction ), GenCount_( GenCount ), GenType_( GenType ), GenData( new GeneratedThreadData[ GenCount ] )
{
  if( GenType != Regenerating )
  {
    for( size_t i = 0; i < GenCount; ++i )
    {
      GenData[ i ].RunFunction = RunFunction;

      GenData[ i ].Data = GenFunction();
    }
  }
  else
  {
    for( size_t i = 0; i < GenCount; ++i )
    {
      GenData[ i ].RunFunction = RunFunction;
    }
  }
}

ThreadedBranch::~ThreadedBranch()
{
  delete [] Threads;

  delete [] ChildData;

  delete [] GenData;
}

bool ThreadedBranch::RunThreaded( TreeDataStoragePtr DataStoragePtr,
                                  StackClass &ParentStack ) ThrowingElse( noexcept( false ), noexcept )
{
  const auto Count = ThreadBranches.size();

  if( Stacks.empty() )
  {
    Stacks.resize( Count );
  }

  if( !Threads )
  {
    Threads = new HANDLE[ Count + GenCount_ ];

    ChildData = new ChildThreadData[ Count - 1 ];
  }

  MainThreadData MainData;

  if( Stacks[ 0 ].Empty() )
  {
    MainData = MainThreadData{ UnionClass::GetUnion( &ThreadBranches, 0 ), DataStoragePtr, &Stacks[ 0 ] };
  }
  else
  {
    MainData = MainThreadData{ Stacks[ 0 ].Top(), DataStoragePtr, &Stacks[ 0 ] };
  }

  Threads[ 0 ] = CreateThread( nullptr, 0, &MainThreadFunction, &MainData, 0, nullptr );

  Throwing(
    if( !Threads[ 0 ] )
    {
      throw( BadThreadThrow() );
    }
  )

  for( size_t i = 0; i < Count - 1; ++i )
  {
    if( Stacks[ i + 1 ].Empty() )
    {
      ChildData[ i ].Union = UnionClass::GetUnion( &ThreadBranches, i + 1  );
    }
    else
    {
      ChildData[ i ].Union = Stacks[ i + 1 ].Top();
    }

    ChildData[ i ].Storage = DataStoragePtr;
    ChildData[ i ].Stack = &Stacks[ i + 1 ];

    Threads[ i + 1 ] = CreateThread( nullptr, 0, &ChildThreadFunction, &ChildData[ i ], 0, nullptr );

    Throwing(
      if( !Threads[ i + 1 ] )
      {
        for( size_t j = 0; j < i + 1; ++j )
        {
          CloseHandle( Threads[ j ] );
        }

        throw( BadThreadThrow() );
      }
    )
  }

  if( GenFunction_ && GenType_ == Regenerating )
  {
    for( size_t i = 0; i < GenCount_; ++i )
    {
      GenData[ i ].Data = GenFunction_();
    }
  }

  for( size_t i = 0; i < GenCount_; ++i )
  {
    Threads[ i + Count ] = CreateThread( nullptr, 0, &GeneratedThreadFunction, &GenData[ i ], 0, nullptr );

    Throwing(
      if( !Threads[ i + Count ] )
      {
        for( size_t j = 0; j < i + Count; ++j )
        {
          CloseHandle( Threads[ j ] );
        }

        throw( BadThreadThrow() );
      }
    )
  }

  WaitForMultipleObjects( Count, Threads, true, INFINITE );

  for( size_t i = 0; i < Count + GenCount_; ++i )
  {
    CloseHandle( Threads[ i ] );
  }

  Throwing( 
    if( MainData.Return < Prev )
    {
      throw( BadBranchThrow( MainData.Return ) );
    } 
  )

  if( !MainData.Return )
  {
    ParentStack.Pop();

    Stacks.clear();

    if( GenFunction_ && GenType_ == Dynamic )
    {
      for( size_t i = 0; i < GenCount_; ++i )
      {
        GenData[ i ].Data = GenFunction_();
      }
    }
  }
  else if( MainData.Return == Prev )
  {
    return true;
  }

  return false;
} 

DWORD WINAPI ThreadedBranch::MainThreadFunction( LPVOID Data ) ThrowingElse( noexcept( false ), noexcept )
{
  auto CastedData = static_cast< MainThreadData * >( Data );

  switch( CastedData->Data.Union->GetUnionType() )
  {
    case UnionClass::BehaviorTreeType:
    {
      auto Tree = CastedData->Data.Union->GetTree();

      if( !( Tree->RunBehaviorTree() ) )
      {
        Tree->ResetBehaviorTree();

        CastedData->Return = false;
      }
      else
      {
        CastedData->Return = true;
      }

      break;
    }

    case UnionClass::MultiBranchType:
      CastedData->Return = CastedData->Data.Union->GetBranch()->RunBranch( CastedData->Data.Storage, *CastedData->Data.Stack );
      break;
  }
  
  return 0;
}

DWORD WINAPI ThreadedBranch::ChildThreadFunction( LPVOID Data ) ThrowingElse( noexcept( false ), noexcept )
{
  auto CastedData = static_cast< ChildThreadData * >( Data );

  switch( CastedData->Union->GetUnionType() )
  {
    case UnionClass::BehaviorTreeType:
    {
      auto Tree = CastedData->Union->GetTree();

      if( !( Tree->RunBehaviorTree() ) )
      {
        Tree->ResetBehaviorTree();
      }

      break;
    }

    case UnionClass::MultiBranchType:
      CastedData->Union->GetBranch()->RunBranch( CastedData->Storage, *CastedData->Stack ); // This should never return false
      break;
  }

  return 0;
}

DWORD WINAPI ThreadedBranch::GeneratedThreadFunction( LPVOID Data ) ThrowingElse( noexcept( false ), noexcept )
{
  auto CastedData = static_cast< GeneratedThreadData * >( Data );

  CastedData->RunFunction( CastedData->Data );

  return 0;
}

