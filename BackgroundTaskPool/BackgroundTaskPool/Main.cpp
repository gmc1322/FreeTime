
#include "BackgroundTaskPool.h"

#include <iostream>

int main()
{
  constexpr unsigned char TaskPoolSize = 10;

  BackgroundTaskPool< TaskPoolSize, true > TaskPool{};
  //BackgroundTaskPool< TaskPoolSize, false > TaskPool{}; // compare speed

  std::vector< BackgroundTask::TaskIDType > Tasks;

  for( ; ; )
  {
    const BackgroundTask::TaskIDType ID = TaskPool.CreateTask();

    if( ID )
    {
      Tasks.push_back( ID );
    }
    else
    {
      break;
    }
  }

  {
    const auto SetDLLFunc = [ &TaskPool ]( const BackgroundTask::TaskIDType &ID )noexcept->void
    {
      TaskPool.SendTask_DLL( ID, std::string{ "MyDLL.dll" } );
    };

    std::for_each( std::execution::par_unseq, Tasks.begin(), Tasks.end(), SetDLLFunc );
  }
  
  {
    HANDLE const SharedMemory = CreateFileMapping( INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0,
                                                TaskPoolSize * sizeof( int ) * 2, "MySharedMem" );

    auto *const MemoryPtr = reinterpret_cast< int * >( MapViewOfFile( SharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) );

    for( unsigned char i = 0; i < TaskPoolSize; i += 2 )
    {
      *( MemoryPtr + i ) = i;
    }

    {
      const auto SetFunctionFunc = [ &TaskPool ]( const BackgroundTask::TaskIDType &ID )noexcept->void
      {
        TaskPool.SendTask_Function( ID, std::string{ "SharedMemFunc" }, std::string{ "MySharedMem" } );
      };

      std::for_each( std::execution::par_unseq, Tasks.begin(), Tasks.end(), SetFunctionFunc );
    }

    {
      size_t ArgOffset    = 0;
      size_t ResultOffset = sizeof( int );

      const auto SendArgsFunc = [ &TaskPool, &ArgOffset, &ResultOffset ]( const BackgroundTask::TaskIDType &ID )noexcept->void
      {
        TaskPool.SendTask_Args( ID, ArgOffset, ResultOffset );

        ArgOffset = ResultOffset + sizeof( int );

        ResultOffset = ArgOffset + sizeof( int );
      };

      std::for_each( std::execution::seq, Tasks.begin(), Tasks.end(), SendArgsFunc );
    }

    {
      const auto SendArgsDoneFunc = [ &TaskPool ]( const BackgroundTask::TaskIDType &ID )noexcept->void
      {
        TaskPool.SendTask_ArgsDone( ID );
      };

      std::for_each( std::execution::par_unseq, Tasks.begin(), Tasks.end(), SendArgsDoneFunc );
    }

    for( unsigned char i = 0; i < TaskPoolSize; ++i )
    {
      std::cout << *( MemoryPtr + i ) << " ";
    }

    UnmapViewOfFile( MemoryPtr );

    CloseHandle( SharedMemory );
  }

  {
    const auto SendFunctionDoneFunc = [ &TaskPool ]( const BackgroundTask::TaskIDType &ID )noexcept->void
    {
      TaskPool.SendTask_FunctionDone( ID );
    };

    std::for_each( std::execution::par_unseq, Tasks.begin(), Tasks.end(), SendFunctionDoneFunc );
  }

  return 0;
}
