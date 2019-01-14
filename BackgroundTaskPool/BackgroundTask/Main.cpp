
#include "SocketAddress.h"

#include <memoryapi.h>

#include <fstream>

#pragma comment( lib, "Ws2_32.lib" )

enum
{
  Port = 1,
};

unsigned short atous( const char *const String ) noexcept
{
  unsigned short Result = 0;

  for( unsigned char i = 0; String[ i ] != '\0'; ++i )
  {
    Result = Result * 10 + String[ i ] - '0';
  }

  return Result;
}

unsigned long long atoull( const char *const String ) noexcept
{
  unsigned long long Result = 0;

  for( unsigned char i = 0; String[ i ] != '\0'; ++i )
  {
    Result = Result * 10 + String[ i ] - '0';
  }

  return Result;
}

int main( const int, char *const Argv[] )
{
  // Initialize WSA
  {
    WSADATA Garbage;

    WSAStartup( MAKEWORD( 2, 2 ), &Garbage );
  }

  const SOCKET NetworkSocket = socket( AF_INET, SOCK_DGRAM, 0 );

  // Bind to recive data
  {
    const SocketAddress ReceiveAddress{ atous( Argv[ Port ] ) };

    bind( NetworkSocket, ReceiveAddress.GetSockAddr(), static_cast< int >( ReceiveAddress.Size() ) );
  }

  constexpr unsigned short MaxEthernetSize = 1500;

  auto *const Buffer = new char[ MaxEthernetSize + 1 ]; // + 1 for null

  for( int End; ; )
  {       
          // Get DLL name
    End = recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr );

    if( End )
    {
      Buffer[ End ] = '\0';

      const HMODULE DLL = LoadLibrary( Buffer );

      for( ; ; )
      {
        // Get Function type
        End = recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr );

        if( End )
        {
          if( Buffer[ 0 ] == '0' )
          {
            // Get Function name
            Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

            const auto DLLFunction = reinterpret_cast< void( *)( const void *, void * ) >( GetProcAddress( DLL, Buffer ) );


            Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

            HANDLE const SharedMemory = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, Buffer );

            auto *MemoryPtr = reinterpret_cast< char* >( MapViewOfFile( SharedMemory, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) );

            for( ; ; )
            {
              // Get offset to Arg memory
              End = recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr );

              if( End )
              {
                Buffer[ End ] = '\0';

                const void *const ArgMemoryPtr = MemoryPtr + atoull( Buffer );

                Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

                DLLFunction( ArgMemoryPtr, MemoryPtr + atoull( Buffer ) );
              }
              else // Quit condition
              {
                break;
              }
            }

            UnmapViewOfFile( MemoryPtr );

            CloseHandle( SharedMemory );
          }
          else
          {
            // Get Function name
            Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

            const auto DLLFunction = reinterpret_cast< void( *)( std::ifstream &ReadFile, std::ofstream &WriteFile
                                                                 ) >( GetProcAddress( DLL, Buffer ) );
            // Get Read File name
            Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

            std::ifstream ReadFile{ Buffer };

            // Get Write File name
            Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

            std::ofstream WriteFile{ Buffer };

            for( ; ; )
            {
              // Get offset to Args
              End = recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr );

              if( End )
              {
                Buffer[ End ] = '\0';

                ReadFile.seekg( atoull( Buffer ) );

                Buffer[ recvfrom( NetworkSocket, Buffer, MaxEthernetSize, 0, nullptr, nullptr ) ] = '\0';

                WriteFile.seekp( atoull( Buffer ) );

                DLLFunction( ReadFile, WriteFile );
              }
              else // Quit condition
              {
                break;
              }
            }
          }
        }
        else // Quit condition
        {
          break;
        }
      }

      FreeLibrary( DLL );
    }
    else // Quit condition
    {
      break;
    }
  }

  delete[] Buffer;

  // Close Socket and Clean Up
  closesocket( NetworkSocket );

  WSACleanup();

  return 0;
}