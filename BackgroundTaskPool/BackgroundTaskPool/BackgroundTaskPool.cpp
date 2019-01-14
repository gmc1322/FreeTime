/*!----------------------------------------------------------------------------
\file   BackgroundTaskPool.h

\author Garrett Conti

\brief  Project: BackgroundTaskPool
        Course:  FUN
---------------------------------------------------------------------------- */

#include "BackgroundTaskPool.h"

#include "SocketAddress.h"

#include <string>
#include <execution>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< size_t PoolSize, bool ThreadedCreation >
BackgroundTaskPool< PoolSize, ThreadedCreation >::BackgroundTaskPool() noexcept
{
  if constexpr( ThreadedCreation )
  {
    // Increment and return prev value
    if( !( BackgroundTask::TaskCriticalSection::Initialized.fetch_add( 1, std::memory_order_relaxed ) ) )
    {
      InitializeCriticalSection( &( BackgroundTask::TaskCriticalSection::CritSec ) );
    }
  }

  static const auto CreateProcessFunction = []( BackgroundTask::TaskIDType &TaskPort )noexcept->void
  {
    {
      STARTUPINFO Garbo1; // We don't care about the process once its started
      PROCESS_INFORMATION Garbo2;

      ZeroMemory( &Garbo1, sizeof( Garbo1 ) );
      Garbo1.cb = sizeof( Garbo1 );

      ZeroMemory( &Garbo2, sizeof( Garbo2 ) );

      std::string ConstArgs = "BackgroundTask.exe ";

      if constexpr( ThreadedCreation ) // Make sure we don't access port while it is changing
      {
        EnterCriticalSection( &( BackgroundTask::TaskCriticalSection::CritSec ) );
        ConstArgs.append( std::to_string( BackgroundTask::TaskPortNumber::Port ) );
        TaskPort = BackgroundTask::TaskPortNumber::Port;
        ++BackgroundTask::TaskPortNumber::Port;
        LeaveCriticalSection( &( BackgroundTask::TaskCriticalSection::CritSec ) );
      }
      else
      {
        ConstArgs.append( std::to_string( BackgroundTask::TaskPortNumber::Port ) );
        TaskPort = BackgroundTask::TaskPortNumber::Port;
        ++BackgroundTask::TaskPortNumber::Port;
      }

      auto *const CommandLineArgs = new char[ ConstArgs.size() + 1 ];

      strcpy_s( CommandLineArgs, ConstArgs.size() + 1, ConstArgs.data() );

      CreateProcess( nullptr, CommandLineArgs, nullptr, nullptr, FALSE, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &Garbo1,
                     &Garbo2 );

      delete[] CommandLineArgs;
    }
  };

  if constexpr( ThreadedCreation )
  {
    std::for_each( std::execution::par, AvailableTasks.begin(), AvailableTasks.end(), CreateProcessFunction );
  }
  else
  {
    std::for_each( std::execution::seq, AvailableTasks.begin(), AvailableTasks.end(), CreateProcessFunction );
  }
}

template< size_t PoolSize, bool ThreadedCreation >
BackgroundTaskPool< PoolSize, ThreadedCreation >::BackgroundTaskPool( BackgroundTaskPool &&Move ) noexcept :
AvailableIndex( Move.AvailableIndex ), Socket{ std::move( Move.Socket ) }
{
  memcpy( AvailableTasks, Move.AvailableTasks, PoolSize * decltype( AvailableTasks )::value_type );

  Move.AvailableIndex = static_cast< size_t >( -1 );
}

template< size_t PoolSize, bool ThreadedCreation >
template< bool DiffrentThreadedCreation >
BackgroundTaskPool<PoolSize, ThreadedCreation>::BackgroundTaskPool( 
  BackgroundTaskPool< PoolSize, DiffrentThreadedCreation > &&Move ) noexcept :
AvailableIndex( Move.AvailableIndex ), Socket{ std::move( Move.Socket ) }
{
  memcpy( AvailableTasks, Move.AvailableTasks, PoolSize * decltype( AvailableTasks )::value_type );

  Move.AvailableIndex = static_cast< size_t >( -1 );
}

template< size_t PoolSize, bool ThreadedCreation >
BackgroundTaskPool< PoolSize, ThreadedCreation >::~BackgroundTaskPool() noexcept
{
  static const auto KillTasks = [ this ]( BackgroundTask::TaskIDType TaskPort )noexcept->void
  {
    const SocketAddress SendAddress{ TaskPort };

    sendto( Socket.NetworkSocket, nullptr, 0, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
    sendto( Socket.NetworkSocket, nullptr, 0, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
    sendto( Socket.NetworkSocket, nullptr, 0, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
  };

  if( AvailableIndex != static_cast< size_t >( -1 ) )
  {
    std::for_each( std::execution::par_unseq, AvailableTasks.begin(), AvailableTasks.end(), KillTasks );
  }

  if constexpr( ThreadedCreation )
  {
    // Decrement and return Prev value
    // if Prev was 1, we just decremented to 0; so there are no more running pools, delete CritSec
    if( BackgroundTask::TaskCriticalSection::Initialized.fetch_sub( 1, std::memory_order_relaxed ) == 1 )
    {
      DeleteCriticalSection( &( BackgroundTask::TaskCriticalSection::CritSec ) );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< size_t PoolSize, bool ThreadedCreation >
BackgroundTaskPool< PoolSize, ThreadedCreation > & 
BackgroundTaskPool< PoolSize, ThreadedCreation >::operator=( BackgroundTaskPool &&Move ) noexcept
{
  memcpy( AvailableTasks, Move.AvailableTasks, PoolSize * decltype( AvailableTasks )::value_type );

  AvailableIndex = Move.AvailableIndex;
  Socket = std::move( Move.Socket );

  Move.AvailableIndex = static_cast< size_t >( -1 );

  return *this;
}

template< size_t PoolSize, bool ThreadedCreation >
template< bool DiffrentThreadedCreation >
BackgroundTaskPool< PoolSize, ThreadedCreation > &
BackgroundTaskPool< PoolSize, ThreadedCreation >::operator=( BackgroundTaskPool< PoolSize, DiffrentThreadedCreation > &&Move
                                                             ) noexcept
{
  memcpy( AvailableTasks, Move.AvailableTasks, PoolSize * decltype( AvailableTasks )::value_type );

  AvailableIndex = Move.AvailableIndex;
  Socket = std::move( Move.Socket );

  Move.AvailableIndex = static_cast< size_t >( -1 );

  return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< size_t PoolSize, bool ThreadedCreation >
unsigned short BackgroundTaskPool< PoolSize, ThreadedCreation >::CreateTask() noexcept
{
  return AvailableIndex == 0 ? 0 : AvailableTasks[ --AvailableIndex ];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_DLL( BackgroundTask::TaskIDType TaskID,
                                                                     const char *const DLLName, const int Size ) const noexcept
{
  const SocketAddress SendAddress{ TaskID };

  sendto( Socket.NetworkSocket, DLLName, Size, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
}

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_DLL( BackgroundTask::TaskIDType TaskID,
                                                                     const std::string &DLLName ) const noexcept
{
  SendTask_DLL( TaskID, DLLName.c_str(), static_cast< int >( DLLName.length() ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_Function( BackgroundTask::TaskIDType TaskID,
                                                                          const char *const FunctionName,
                                                                          const int FunctionNameSize,
                                                                          const char *const SharedMemoryName,
                                                                          const int MemoryNameSize ) const noexcept
{
  const SocketAddress SendAddress{ TaskID };

  {
    const auto *const Temp = new char( '0' );

    // Tell the Task we are using shared mem
    sendto( Socket.NetworkSocket, Temp, 1, 0, SendAddress.GetSockAddr(), SendAddress.Size() );

    delete Temp;
  }

  sendto( Socket.NetworkSocket, FunctionName,     FunctionNameSize, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
  sendto( Socket.NetworkSocket, SharedMemoryName, MemoryNameSize,   0, SendAddress.GetSockAddr(), SendAddress.Size() );
}

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_Function( BackgroundTask::TaskIDType TaskID,
                                                                          const std::string &FunctionName,
                                                                          const std::string &SharedMemoryName ) const noexcept
{
  SendTask_Function( TaskID, FunctionName.c_str(), static_cast< int >( FunctionName.length() ),
                     SharedMemoryName.c_str(),     static_cast< int >( SharedMemoryName.length() ) );
}

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_Function( BackgroundTask::TaskIDType TaskID,
                                                                          const char *const FunctionName,
                                                                          const int FunctionNameSize,
                                                                          const char *const ReadFileName,
                                                                          const int ReadFileNameSize,
                                                                          const char *const WriteFileName,
                                                                          const int WriteFileNameSize ) const noexcept
{
  const SocketAddress SendAddress{ TaskID };

  {
    const auto *const Temp = new char( '1' );

    // Tell the Task we are using shared mem
    sendto( Socket.NetworkSocket, Temp, 1, 0, SendAddress.GetSockAddr(), SendAddress.Size() );

    delete Temp;
  }

  sendto( Socket.NetworkSocket, FunctionName, FunctionNameSize, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
  sendto( Socket.NetworkSocket, ReadFileName, ReadFileNameSize, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
  sendto( Socket.NetworkSocket, WriteFileName, WriteFileNameSize, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
}

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_Function( BackgroundTask::TaskIDType TaskID,
                                                                          const std::string &FunctionName,
                                                                          const std::string &ReadFileName,
                                                                          const std::string &WriteFileName ) const noexcept
{
  SendTask_Function( TaskID, FunctionName.c_str(),  static_cast< int >( FunctionName.length()  ), 
                             ReadFileName.c_str(),  static_cast< int >( ReadFileName.length()  ),
                             WriteFileName.c_str(), static_cast< int >( WriteFileName.length() ) );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_Args( BackgroundTask::TaskIDType TaskID,
                                                                      const size_t DataOffset,
                                                                      const size_t ResultOffset ) const noexcept
{
  const SocketAddress SendAddress{ TaskID };

  {
    const auto Temp = std::to_string( DataOffset );

    sendto( Socket.NetworkSocket, Temp.c_str(), static_cast< int >( Temp.length() ), 0,
            SendAddress.GetSockAddr(), SendAddress.Size() );
  }

  {
    const auto Temp = std::to_string( ResultOffset );

    sendto( Socket.NetworkSocket, Temp.c_str(), static_cast< int >( Temp.length() ), 0,
            SendAddress.GetSockAddr(), SendAddress.Size() );
  }
}

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_ArgsDone( BackgroundTask::TaskIDType TaskID ) const noexcept
{
  const SocketAddress SendAddress{ TaskID };

  sendto( Socket.NetworkSocket, nullptr, 0, 0, SendAddress.GetSockAddr(), SendAddress.Size() );
}

template< size_t PoolSize, bool ThreadedCreation >
void BackgroundTaskPool< PoolSize, ThreadedCreation >::SendTask_FunctionDone( BackgroundTask::TaskIDType TaskID ) noexcept
{
  SendTask_ArgsDone( TaskID );

  AvailableTasks[ AvailableIndex ] = TaskID;

  ++AvailableIndex;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
