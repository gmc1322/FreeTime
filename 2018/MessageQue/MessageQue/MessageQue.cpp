#include "MessageQue.h"
#include <mutex>

static void AppendError( std::string &What )
{
  LPVOID ErrorMsg = nullptr;

  FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, GetLastError(),
                 MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), LPTSTR( &ErrorMsg ), 0, nullptr );

  

  What.append( LPTSTR( ErrorMsg ) );

  LocalFree( ErrorMsg ); // Free mem allocated from FormatMessage
}

///////////////////////////////////////////////////////////////////////////////

MessageQue::BadProcessCreation::BadProcessCreation( const std::string &ExecProgram ) noexcept :
What( "Could not create process: " + ExecProgram + ", becouse: " )
{
  AppendError( What );
}

MessageQue::BadQueCreation::BadQueCreation( const std::string &PipeName ) noexcept :
What( "Could not create pipe: " + PipeName + ", becouse: " )
{
  AppendError( What );
}

MessageQue::QueNotConnected::QueNotConnected() noexcept : What( "Could not connect to the pipe becouse: " )
{
  AppendError( What );
}

///////////////////////////////////////////////////////////////////////////////

const std::string MessageQue::CloseProgram{ "-1" };

MessageQue::MessageQue( const std::string &ExecProgram, const std::string &PipeName, const size_t MessageSize_, float RoutineCheckTime ) :
MessageSize( MessageSize_ + 1 )
{
  const std::string FullPipeName = "\\\\.\\pipe\\" + PipeName;
  Pipe = CreateNamedPipe( FullPipeName.c_str(),     // Pipe name
                          PIPE_ACCESS_OUTBOUND,                       // This pipe outputs
                          PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | // This will be a message pipe
                          PIPE_WAIT |                                 // Wait untill all data is writen
                          PIPE_REJECT_REMOTE_CLIENTS,                 // No remote access
                          2,                                          // Only one sender and reciver for the pipe
                          MessageSize,                            // Size of the messages being sent (max)
                          0,                                          // Input size (0, no input)
                          5000,                                       // Wait 5s when connecting to pipe
                          nullptr );                                  // Security attributes

  AssertAlways( Pipe == INVALID_HANDLE_VALUE, BadQueCreation{ PipeName } );

  // Allocate memory and set it to 0
  ZeroMemory( &StartInfo, sizeof( STARTUPINFO ) );
  ZeroMemory( &ProcInfo, sizeof( PROCESS_INFORMATION ) );

  AssertCleanupAlways(
    !( CreateProcess( ExecProgram.c_str(), // Program to run
                      const_cast< char * >( ( FullPipeName + " " + // Send pipe name, msg size, and check time on command line
                                              std::to_string( MessageSize ) + " " +
                                              std::to_string( RoutineCheckTime *
                                                              1000 ) ).c_str() ),
                      nullptr,    // Security attributes
                      nullptr,    // Thread attributes
                      FALSE,      // Don't inherit handles
                      0,          // Creation flags (none)
                      nullptr,    // Use parent's environment
                      nullptr,    // Use parent's directory
                      &StartInfo, // Start up info
                      &ProcInfo ) // Process info
       ), CloseHandle( Pipe ), BadProcessCreation{ ExecProgram } );

  AssertCleanupAlways( !( ConnectNamedPipe( Pipe, nullptr ) ),
                       CloseHandle( ProcInfo.hProcess ); CloseHandle( ProcInfo.hThread ); CloseHandle( Pipe ), 
                       ( QueNotConnected{} ) );

  // Create second pipe for client
  CreateNamedPipe( FullPipeName.c_str(),                       // Pipe name
                   PIPE_ACCESS_OUTBOUND,                       // This pipe outputs
                   PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | // This will be a message pipe
                   PIPE_WAIT |                                 // Wait untill all data is writen
                   PIPE_REJECT_REMOTE_CLIENTS,                 // No remote access
                   2,                                          // Only one sender and reciver for the pipe
                   MessageSize,                                // Size of the messages being sent (max)
                   0,                                          // Input size (0, no input)
                   5000,                                       // Wait 5s when connecting to pipe
                   nullptr );                                  // Security attributes
}

MessageQue::~MessageQue() noexcept
{
  SendMsg( CloseProgram );

  FlushFileBuffers( Pipe ); // Make sure to send all data, this will block sadly

  DisconnectNamedPipe( Pipe );

  CloseHandle( ProcInfo.hProcess );
  CloseHandle( ProcInfo.hThread );

  CloseHandle( Pipe );
}

void MessageQue::SendMsg( const std::string &Message ) const ExceptDebug
{
  AssertDebug( Message.size() > MessageSize, ( MsgTooLarge{ Message, static_cast< int >( Message.size() ) -
                                               static_cast< int >( MessageSize ) } ) );

  const size_t Size = MessageSize - Message.size();

  if( Size )
  {
    std::string Temp{ Message };

    for( size_t i = 0; i < Size; ++i )
    {
      Temp.append( " " );
    }

    WriteFile( Pipe, Temp.c_str(), MessageSize, nullptr, nullptr );
  }
  else
  {
    WriteFile( Pipe, Message.c_str(), MessageSize, nullptr, nullptr );
  }
}

///////////////////////////////////////////////////////////////////////////////

#include <iostream>

MessageQue::MsgTooLarge::MsgTooLarge( const std::string &Message, int Size ) noexcept
{
  std::cout << "The message: " << Message << ", is to large to send! It is over by: " << std::to_string( Size * -1 ) << " characters!" << std::endl;
}
