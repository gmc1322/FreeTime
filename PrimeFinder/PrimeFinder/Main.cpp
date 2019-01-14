
#include "PrimeFinder.h"

#include <future>

static constexpr size_t MaxPrime = static_cast< size_t >( -1 );
static constexpr size_t MinPrime = 4'294'967'295; // sqrt( Max ) )
//static constexpr size_t Increment = ( MaxPrime / 2 ) / 16;

//static const Size_T ThreadCount = _Thrd_hardware_concurrency() - 1;
//static const size_t ThreadCheckCount = MinPrime / ThreadCount;

//static const Size_T ThreadCount = _Thrd_hardware_concurrency();
//static const Size_T ThreadIncrement = ThreadCount * 2;

bool PrimeRangeCheck( const size_t Number, size_t Start, const size_t End ) NoExcept
{
  bool IsPrime = true;

  for( ; Start < End; Start += 2 )
  {
    if( Number % Start == 0 )
    {
      IsPrime = false;

      break;
    }
  }

  return IsPrime;
};

// Only odds
//bool IsPrime_Threaded( const size_t Number ) NoExcept
//{
//  bool IsPrime = true;
//
//  std::vector< std::future< bool > > PrimeFinders;
//
//  Size_T CurrentThreadCount = 0;
//
//  constexpr unsigned WaitTime = 1; // Tested number
//
//  for( size_t i = 3, End = Number / 2; i < End; ++CurrentThreadCount, i += ThreadCheckCount )
//  {
//    if( CurrentThreadCount < ThreadCount )
//    {
//      PrimeFinders.emplace_back( std::async( std::launch::async, PrimeRangeCheck, i, i + ThreadCheckCount ) );
//    }
//    else do
//    {
//      for( auto Iter = PrimeFinders.begin(), EndIter = PrimeFinders.end(); Iter != EndIter; ++Iter )
//      {
//        if( Iter->wait_for( std::chrono::nanoseconds{ WaitTime } ) == std::future_status::ready )
//        {
//          if( Iter->get() )
//          {
//            PrimeFinders.erase( Iter );
//          }
//          else { IsPrime = false; goto EndLoop; }
//            
//          break;
//        }
//      }
//    }
//    while( PrimeFinders.size() );
//  }
//
//  do
//  {
//    for( auto Iter = PrimeFinders.begin(), End = PrimeFinders.end(); Iter != End; ++Iter )
//    {
//      if( Iter->wait_for( std::chrono::nanoseconds{ WaitTime } ) == std::future_status::ready )
//      {
//        if( Iter->get() )
//        {
//          PrimeFinders.erase( Iter );
//        }
//        else { IsPrime = false; goto EndLoop; }
//
//        break;
//      }
//    }
//  }
//  while( PrimeFinders.size() );
//  
//EndLoop:
//  return IsPrime;
//}

bool IsPrime_Fast( const size_t PrimeNumber ) NoExcept
{
  bool IsPrime = true;

  for( size_t i = 3, End = static_cast< size_t >( std::ceil( std::sqrt( PrimeNumber ) ) ); i < End; i += 2 )
  {
    if( PrimeNumber % i == 0 )
    {
      IsPrime = false;

      break;
    }
  }

  return IsPrime;
}

#include <chrono>
#include <random>
#include <array>
#include <fstream>
#include <string>

#include <windows.h>
#include <processthreadsapi.h>

int main( /*int, char *argv[]*/ )
{
#if 1
  {
    //for( Size_T Step = 100, i = 0; i < 18; ++i, Step *= 2 )
    {
      std::vector< size_t > Values;

      double Ave = 0;
      double Count = 0;

      {
        size_t Time = 0;

        {
          std::ifstream InputFile{ "OutData.txt" };

          size_t EndRandVal;
          size_t StartRandVal;

          size_t PrevTime = 0;

          //const auto Step = std::stoul( argv[ 1 ] );
          const Size_T Step = 6553600;

          do
          {
            InputFile >> EndRandVal;
            InputFile >> StartRandVal;

            for( size_t CurrentVal = StartRandVal, Increment = ( EndRandVal - StartRandVal ) / Step; CurrentVal < EndRandVal; CurrentVal += Increment )
            {
              const auto StartTime = std::chrono::system_clock::now();

              IsPrime_Fast( CurrentVal );

              const auto EndTime = std::chrono::system_clock::now();

              Time += ( EndTime - StartTime ).count();
            }

            Values.emplace_back( Time - PrevTime );

            PrevTime = Time;

            ++Count;
          }
          while( !( InputFile.eof() ) );
        }

        {
          std::ofstream Outputfile{ "OutFast.txt", std::ios_base::app };

          Ave = Time / Count;

          Outputfile << Ave << " ";
        }
      }

      {
        double StdDev = 0;

        for( size_t Iter : Values )
        {
          const double Temp = Iter - Ave;

          StdDev += Temp * Temp;
        }

        StdDev = std::sqrt( StdDev / ( Count - 1 ) );

        std::ofstream Outputfile{ "OutFastStdDev.txt", std::ios_base::app };

        Outputfile << StdDev << " ";
      }
    }

    return 0;
  }
#elif 1
  std::ifstream InputFile{ "OutData.txt" };
  std::ifstream InputFastFile{ "OutFast.txt" };

  do
  {

  }
  while(  )

#elif 0
  {
    const size_t RandVal  = std::stoull( argv[ 1 ] );
    const size_t StartVal = std::stoull( argv[ 2 ] );
    const size_t EndVal   = std::stoull( argv[ 3 ] );

    const bool IsPrime = PrimeRangeCheck( RandVal, StartVal, EndVal );

    return IsPrime;
  }
#elif 1
  {
    std::ofstream Outputfile{ "OutData.txt" };

    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng( rd() ); // seed the generator
    //std::uniform_int_distribution< size_t > distr( MinPrime - Increment / 2, MinPrime + Increment / 2 ); // define the range
    std::uniform_int_distribution< size_t > distrLarge( MaxPrime / 2, MaxPrime ); // define the range
    std::uniform_int_distribution< size_t > distrSmall( MinPrime / 2, MinPrime ); // define the range

    for( size_t i = 0; i < 512; ++i )
    {
      for( ; ; )
      {
        const size_t Val = distrLarge( eng );

        if( Val % 2 != 0 )
        {
          Outputfile << Val << " ";

          break;
        }
      }

      for( ; ; )
      {
        const size_t Val = distrSmall( eng );

        if( Val % 2 != 0 )
        {
          Outputfile << Val << " ";

          break;
        }
      }
    }

    return 0;
  }
#elif 0
  {
    if( argv ) {}

    size_t Time = 0;

    float Count = 0;
    //float Count = 512;

    std::ifstream InputFile{ "OutData.txt" };

    size_t RandVal;

    do
    {
      STARTUPINFO si;
      ZeroMemory( &si, sizeof( si ) );
      si.cb = sizeof( si );

      PROCESS_INFORMATION pi{};

      std::vector< HANDLE > ProccessArray;

      ProccessArray.reserve( ThreadCount );

      InputFile >> RandVal;

      for( size_t StartVal = 3, EndVal = StartVal + Increment, EndRandVal = RandVal / 2; 
           /*i < ThreadCount && */StartVal < EndRandVal;
           StartVal += Increment, EndVal += Increment )
      {
        ZeroMemory( &pi, sizeof( pi ) );

        const std::string &Temp = std::to_string( RandVal ) + " " + std::to_string( StartVal ) + " " + std::to_string( EndVal );

        char *const Temp2 = new char[ Temp.size() ];

        std::strcpy( Temp2, Temp.c_str() );

        CreateProcess( "PrimeFinder_Proc.exe", Temp2, NULL, NULL, FALSE, NULL, NULL, NULL, &si, &pi );

        ProccessArray.emplace_back( pi.hProcess );

        delete[] Temp2;
      }

      const auto StartTime = std::chrono::system_clock::now();

      do
      {
        DWORD FinishedProccess = WaitForMultipleObjects( static_cast< DWORD >( ProccessArray.size() ), ProccessArray.data(), FALSE, INFINITE );

        DWORD Result;

        HANDLE const Proc = ProccessArray[ FinishedProccess ];

        GetExitCodeProcess( Proc, &Result );

        if( Result == 1 )
        {
          CloseHandle( Proc );

          ProccessArray.erase( ProccessArray.begin() + FinishedProccess );
        }
        else break;
      }
      while( ProccessArray.size() );

      const auto EndTime = std::chrono::system_clock::now();

      for( HANDLE Iter : ProccessArray )
      {
        TerminateProcess( Iter, (UINT)-1 );

        CloseHandle( Iter );
      }

      Time += ( EndTime - StartTime ).count();

      ++Count;
    }
    while( !( InputFile.eof() ) );

    std::ofstream Outputfile{ "OutProc.txt" };

    Outputfile << Time / Count;

    return 0;
  }
#elif 1
  if( argv ) {}

  size_t Time = 0;

  float Count = 0;
  //float Count = 512;

  std::ifstream InputFile{ "OutData.txt" };

  size_t RandVal;

  std::vector< std::future< bool > > PrimeFinders;

  PrimeFinders.reserve( ThreadCount );

  do
  { 
    InputFile >> RandVal;

    {
      for( size_t CurrentPrime = MaxPrime - Increment * ThreadCount;
           CurrentPrime < RandVal;
           CurrentPrime += Increment ) // Increment by 2, we only want odds
      {
        PrimeFinders.emplace_back( std::async( std::launch::async, IsPrime_Fast, CurrentPrime ) );
      }

      for( auto &PrimeIter : PrimeFinders ) // Can't be const, .get() moves value
      {
        const auto StartTime = std::chrono::system_clock::now();

        PrimeIter.wait();

        const auto EndTime = std::chrono::system_clock::now();

        Time += ( EndTime - StartTime ).count();
      }

      PrimeFinders.clear();

    }

    ++Count;
  }
  while( !( InputFile.eof() ) );

  std::ofstream Outputfile{ "OutThread.txt" };

  Outputfile << Time / Count;

  return 0;
#endif

//#if 0
//  for( unsigned i = 1; i <= 1'000; ++i )
//  {
//    std::cout << PrimeFinderClass::GetPrimeAtIndex( i ) << std::endl;
//  }
//#elif 1
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 2 ) << std::endl;
//
//  for( unsigned i = 3; i <= 3 * ( 2 * 98 ); i += 2 )
//  {
//    std::cout << PrimeFinderClass::GetNearestPrimeIndex( i ) << std::endl;
//  }
//#else
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 2 ) << std::endl;
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 4 ) << std::endl;
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 6 ) << std::endl;
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 8 ) << std::endl;
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 12 ) << std::endl;
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 14 ) << std::endl;
//  std::cout << PrimeFinderClass::GetNearestPrimeIndex( 18 ) << std::endl;
//#endif
}
