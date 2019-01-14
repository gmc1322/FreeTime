#include <iostream>

#include <Windows.h>

int main( int argc, const char* argv[] )
{
  Sleep( 10 );

  std::cout << "Client command line count: " << argc << std::endl;

  if( argc < 3 )
  {
    return -1;
  }

  std::cout << "Client connecting to pipe: " << argv[ 0 ] << std::endl;

  const HANDLE Pipe = CreateFile( argv[ 0 ], GENERIC_READ, FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr );

  if( !( WaitNamedPipe( argv[ 0 ], 5000 ) ) )
  {
    std::cout << "Client could not connect!" << std::endl;

    return -2;
  }

  std::cout << "Client connected" << std::endl;

  const size_t MaxMsgSize = atoi( argv[ 1 ] );

  std::cout << "Client msg size: " << MaxMsgSize << std::endl;

  char *MsgHolder = new char [ MaxMsgSize + 1 ]();
  std::string PrevMsg{};

  const size_t RoutineCheckTime = atoi( argv[ 2 ] );

  std::cout << "Client check time: " << RoutineCheckTime << std::endl;

  for( ; ; )
  {
    std::cout << "Client reading" << std::endl;

    ReadFile( Pipe, MsgHolder, MaxMsgSize, nullptr, nullptr );

    std::cout << "Client read: " << MsgHolder << std::endl;

    if( MsgHolder != PrevMsg )
    {
      PrevMsg = MsgHolder;

      if( atoi( MsgHolder ) == -1 )
      {
        std::cout << "Client closing" << std::endl;

        break;
      }
      
      std::cout << "Client got msg: " << MsgHolder << std::endl;
    }
    else
    {
      std::cout << "No response from Main, Client closing" << std::endl;

      break;
    }

    Sleep( RoutineCheckTime );
  }

  delete[] MsgHolder;

  DisconnectNamedPipe( Pipe );
  CloseHandle( Pipe );

  getchar();
  getchar();

  return 0;
}
