/*!----------------------------------------------------------------------------
\file   BackgroundTaskPool.h

\author Garrett Conti
             
\brief  Project: BackgroundTaskPool
        Course:  FUN
---------------------------------------------------------------------------- */

#pragma once

#include <array>
#include <atomic>

#ifndef WIN32_LEAN_AND_MEAN
#define LAM_DEF
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

#include <WinSock2.h>

#ifdef LAM_DEF
#undef WIN32_LEAN_AND_MEAN
#endif

template< size_t PoolSize, bool ThreadedCreation >
class BackgroundTaskPool;

namespace BackgroundTask
{
  class TaskPortNumber
  {
    static unsigned short Port;

    template< size_t PoolSize, bool ThreadedCreation >
    friend class BackgroundTaskPool;
  };

  unsigned short TaskPortNumber::Port = 1024;


  class TaskCriticalSection
  {
    static std::atomic_int Initialized;

    static CRITICAL_SECTION CritSec;

    template< size_t PoolSize, bool ThreadedCreation >
    friend class BackgroundTaskPool;
  };

  std::atomic_int TaskCriticalSection::Initialized{ 0 };

  CRITICAL_SECTION TaskCriticalSection::CritSec;

  class TaskSocket
  {
    public:
      TaskSocket( const TaskSocket &Copy ) noexcept = delete;
      TaskSocket & operator=( const TaskSocket &Copy ) noexcept = delete;

    private:
      TaskSocket() noexcept
      {
        // Increment and return Prev value
        if( NetworkNotInitialized.fetch_add( 1, std::memory_order_relaxed ) )
        {
          WSADATA Garbage;

          WSAStartup( MAKEWORD( 2, 2 ), &Garbage );
        }

        NetworkSocket = socket( AF_INET, SOCK_DGRAM, 0 );
      }

      TaskSocket( TaskSocket &&Move ) noexcept : NetworkSocket( Move.NetworkSocket )
      {
        Move.NetworkSocket = 0;
      }

      ~TaskSocket() noexcept
      {
        if( NetworkSocket )
        {
          closesocket( NetworkSocket );

          // Decrement and return Prev value
          // if Prev was 1, we just decremented to 0; so there are no more running pools, Cleanup Network
          if( NetworkNotInitialized.fetch_sub( 1, std::memory_order_relaxed ) == 1 )
          {
            WSACleanup();
          }
        }
      }

    private:
      TaskSocket & operator=( TaskSocket &&Move ) noexcept
      {
        NetworkSocket = Move.NetworkSocket;

        Move.NetworkSocket = 0;

        return *this;
      }

    private:
      static std::atomic_int NetworkNotInitialized;

      SOCKET NetworkSocket;

    private:
      template< size_t PoolSize, bool ThreadedCreation >
      friend class BackgroundTaskPool;
  };

  std::atomic_int TaskSocket::NetworkNotInitialized{ 0 };

  using TaskIDType = unsigned short;

  using SharedMemoryFunction  = void( *)( const void *, void * );
  using FileReadWriteFunction = void( *)( std::ifstream &ReadFile, std::ofstream &WriteFile );
}

/*!
 * \brief Creates a pool of Tasks to load DLLs and run fucntions from them without stalling the main Program.
 *
 * \tparam PoolSize         - The max number of Tasks this can have running at a time.
 * \tparam ThreadedCreation - If the pool is being created in a thread, 
 *                              or if you want it to create the Tasks using multiple threads.
 */
template< size_t PoolSize, bool ThreadedCreation >
class BackgroundTaskPool
{
  public:
    BackgroundTaskPool() noexcept;

    
    BackgroundTaskPool( const BackgroundTaskPool &Copy ) noexcept = delete;
    BackgroundTaskPool( BackgroundTaskPool &&Move ) noexcept;

    template< bool DiffrentThreadedCreation >
    explicit BackgroundTaskPool( const BackgroundTaskPool< PoolSize, DiffrentThreadedCreation > &Copy ) noexcept = delete;

    template< bool DiffrentThreadedCreation >
    explicit BackgroundTaskPool( BackgroundTaskPool< PoolSize, DiffrentThreadedCreation > &&Move ) noexcept;

    template< size_t DiffrentPoolSize >
    explicit BackgroundTaskPool( const BackgroundTaskPool< DiffrentPoolSize, ThreadedCreation > &Copy ) noexcept = delete;

    template< size_t DiffrentPoolSize >
    explicit BackgroundTaskPool( BackgroundTaskPool< DiffrentPoolSize, ThreadedCreation > &&Move ) noexcept = delete;

    ~BackgroundTaskPool() noexcept;

  public:
    BackgroundTaskPool & operator=( const BackgroundTaskPool &Copy ) noexcept = delete;
    BackgroundTaskPool & operator=( BackgroundTaskPool &&Move ) noexcept;

    template< bool DiffrentThreadedCreation >
    BackgroundTaskPool & operator=( const BackgroundTaskPool< PoolSize, DiffrentThreadedCreation > &Copy ) noexcept = delete;

    template< bool DiffrentThreadedCreation >
    BackgroundTaskPool & operator=( BackgroundTaskPool< PoolSize, DiffrentThreadedCreation > &&Move ) noexcept;

    template< size_t DiffrentPoolSize >
    BackgroundTaskPool & operator=( const BackgroundTaskPool< DiffrentPoolSize, ThreadedCreation > &Copy ) noexcept = delete;

    template< size_t DiffrentPoolSize >
    BackgroundTaskPool & operator=( BackgroundTaskPool< DiffrentPoolSize, ThreadedCreation > &&Move ) noexcept = delete;

  public:
    /*!
     * \brief Gets and ID from the Task pool.
     * 
     * \return unsigned short - The ID of the Task. If ID is 0, there are no more Tasks left!
     * 
     * \warning <b> This cannot be used in a thread! </b>
     */
    BackgroundTask::TaskIDType CreateTask() noexcept;

  public:
    void SendTask_DLL( BackgroundTask::TaskIDType TaskID, const char *DLLName, int Size ) const noexcept;
    void SendTask_DLL( BackgroundTask::TaskIDType TaskID, const std::string &DLLName    ) const noexcept;

  public:
    void SendTask_Function( BackgroundTask::TaskIDType TaskID, const char *FunctionName,     int FunctionNameSize,
                                                               const char *SharedMemoryName, int MemoryNameSize
                                                               ) const noexcept;
    void SendTask_Function( BackgroundTask::TaskIDType TaskID, const std::string &FunctionName,
                                                               const std::string &SharedMemoryName ) const noexcept;
  
    void SendTask_Function( BackgroundTask::TaskIDType TaskID, const char *FunctionName,  int FunctionNameSize,
                                                               const char *ReadFileName,  int ReadFileNameSize,
                                                               const char *WriteFileName, int WriteFileNameSize 
                                                               ) const noexcept;
    void SendTask_Function( BackgroundTask::TaskIDType TaskID, const std::string &FunctionName, 
                                                               const std::string &ReadFileName,
                                                               const std::string &WriteFileName ) const noexcept;
  public:
    void SendTask_Args( BackgroundTask::TaskIDType TaskID, size_t DataOffset, size_t ResultOffset ) const noexcept;

    void SendTask_ArgsDone( BackgroundTask::TaskIDType TaskID ) const noexcept;

    /*!
     * \brief Tell the Task that you are done sending it functions to run.
     *
     * \param TaskID - The ID of the Task.
     * 
     * This will free up the Task to be used with another DLL.
     * 
     * \warning <b> This cannot be used in a thread! </b>
     */
    void SendTask_FunctionDone( BackgroundTask::TaskIDType TaskID ) noexcept;

  private:
    std::array< BackgroundTask::TaskIDType, PoolSize > AvailableTasks;

    size_t AvailableIndex = PoolSize;

    const BackgroundTask::TaskSocket Socket{};
};

#include "BackgroundTaskPool.cpp"
