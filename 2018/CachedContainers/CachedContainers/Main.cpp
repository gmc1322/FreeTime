
#include "CachedArray.h"
#include "PartialCacheArray.h"
#include "CachedList.h"

#include <chrono>
#include <iostream>

constexpr size_t Round( const double &Val ) noexcept
{
  return static_cast< size_t >( Val - static_cast< size_t >( Val ) >= .5 ? Val + 1 : Val );
}

struct CacheFunctor
{
  constexpr size_t operator()( const size_t Index ) const noexcept { return ( Index ? Round( std::log( Index ) * std::log( Index ) + 1 ) : 0 ); }
};

#define ContainerType PartialCacheArray< CacheFunctor >

int main( int, char*[] )
{
  constexpr std::chrono::steady_clock::time_point QuitTime{ std::chrono::seconds{ 10 } };

  std::srand( 20 );

  for( size_t i = 16; ; i *= 16 )
  {
    std::cout << std::endl << "i = " << i << std::endl;

    {
      std::cout << std::endl << "Test 1, Fill, Find all 10 times, Remove all." << std::endl;

      ContainerType CArray{ i };

      for( size_t j = 0; j < i; ++j )
      {
        CArray.Emplace( j );
      }

      const std::chrono::steady_clock::time_point TempTime = std::chrono::steady_clock::now();

      for( size_t j = 0; j < 10; ++j )
      {
        for( size_t k = 0; k < i; ++k )
        {
          CArray.Find( j );
        }
      }

      std::cout << "Find time: " << ( std::chrono::steady_clock::now() - TempTime ).count() / 1e+9 << "s" << std::endl;

      if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
      {
        goto End;
      }
    }

    {
      std::cout << std::endl << "Test 2, Fill, Find random i*10 times, Remove all." << std::endl;

      ContainerType CArray{ i };

      for( size_t j = 0; j < i; ++j )
      {
        CArray.Emplace( j );
      }

      const std::chrono::steady_clock::time_point TempTime = std::chrono::steady_clock::now();

      for( size_t j = 0, End = i * 10; j < End; ++j )
      {
        CArray.Find( std::rand() % ( i - 1 ) );

        if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
        {
          goto End;
        }
      }

      std::cout << "Find time: " << ( std::chrono::steady_clock::now() - TempTime ).count() / 1e+9 << "s" << std::endl;

      if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
      {
        goto End;
      }
    }

    {
      std::cout << std::endl << "Test 3, Fill 4th Find all 4th repeat 4 times, Remove all." << std::endl;

      ContainerType CArray{ i };

      const size_t Forth = i / 4;

      for( size_t j = 1; j <= 4; ++j )
      {
        for( size_t k = Forth * ( j - 1 ), End = Forth * j; k < End; ++k )
        {
          CArray.Emplace( k );
        }

        const std::chrono::steady_clock::time_point TempTime = std::chrono::steady_clock::now();

        for( size_t k = 0, End = Forth * j; k < End; ++k )
        {
          CArray.Find( k );
        }

        std::cout << "Find time " << j << ": " << ( std::chrono::steady_clock::now() - TempTime ).count() / 1e+9 << "s" << std::endl;

        if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
        {
          goto End;
        }
      }
    }

    {
      std::cout << std::endl << "Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all." << std::endl;

      ContainerType CArray{ i };

      const size_t Forth = i / 4;

      for( size_t j = 1; j <= 4; ++j )
      {
        for( size_t k = Forth * ( j - 1 ), End = Forth * j; k < End; ++k )
        {
          CArray.Emplace( k );
        }

        const std::chrono::steady_clock::time_point TempTime = std::chrono::steady_clock::now();

        for( size_t k = 0, End = Forth * j; k < End; ++k )
        {
          CArray.Find( rand() % ( End - 1 ) );
        }

        std::cout << "Find time " << j << ": " << ( std::chrono::steady_clock::now() - TempTime ).count() / 1e+9 << "s" << std::endl;

        if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
        {
          goto End;
        }
      }
    }

    {
      std::cout << std::endl << "Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times." << std::endl;

      ContainerType CArray{ i };

      const size_t Forth = i / 4;
      const size_t Aeith = i / 8;

      for( size_t j = 1; j <= 4; ++j )
      {
        for( size_t k = Forth * ( j - 1 ), End = Forth * j; k < End; ++k )
        {
          CArray.Emplace( k );
        }

        const std::chrono::steady_clock::time_point TempTime = std::chrono::steady_clock::now();

        for( size_t k = Aeith * ( j - 1 ), End = Forth * j; k < End; ++k )
        {
          CArray.Find( k );
        }

        std::cout << "Find time " << j << ": " << ( std::chrono::steady_clock::now() - TempTime ).count() / 1e+9 << "s" << std::endl;

        if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
        {
          goto End;
        }

        for( size_t k = Aeith * ( j - 1 ), End = Aeith * j; k < End; ++k )
        {
          CArray.Remove( k );
        }
      }
    }

    {
      std::cout << std::endl << "Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times." << std::endl;

      ContainerType CArray{ i };

      const size_t Forth = i / 4;
      const size_t Aeith = i / 8;

      for( size_t j = 1; j <= 4; ++j )
      {
        for( size_t k = Forth * ( j - 1 ), End = Forth * j; k < End; ++k )
        {
          CArray.Emplace( k );
        }

        const std::chrono::steady_clock::time_point TempTime = std::chrono::steady_clock::now();

        for( size_t k = Aeith * ( j - 1 ), End = Forth * j; k < End; ++k )
        {
          CArray.Find( k );
        }

        std::cout << "Find time " << j << ": " << ( std::chrono::steady_clock::now() - TempTime ).count() / 1e+9 << "s" << std::endl;

        if( std::chrono::steady_clock::now() - TempTime >= QuitTime.time_since_epoch() )
        {
          goto End;
        }

        for( size_t k = Aeith * ( j - 1 ), End = Aeith * j; k < End; ++k )
        {
          CArray.Remove( k );
        }
      }
    }
  }

End:
  std::cout << std::endl << "Ended" << std::endl;

  return 0;
}

/* CachedArray
i = 4096

Test 1, Fill, Find all 10 times, Remove all.
Find time: 5.3966e-05s

Test 2, Fill, Find random i*10 times, Remove all.
Find time: 0.161293s

Test 3, Fill 4th Find all 4th repeat 4 times, Remove all.
Find time 1: 0.00103228s
Find time 2: 0.00346768s
Find time 3: 0.0102957s
Find time 4: 0.0181778s

Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all.
Find time 1: 0.000918534s
Find time 2: 0.00350228s
Find time 3: 0.00875831s
Find time 4: 0.0135505s

Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times.
Find time 1: 0.000879512s
Find time 2: 0.00213928s
Find time 3: 0.00446343s
Find time 4: 0.00620225s

Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times.
Find time 1: 0.000862354s
Find time 2: 0.00287986s
Find time 3: 0.00403336s
Find time 4: 0.00759181s

i = 65536

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.000874254s

Test 2, Fill, Find random i*10 times, Remove all.

Ended
 */

/* Partial Cache Array, Cache Amount = 4
i = 4096

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.000541601s

Test 2, Fill, Find random i*10 times, Remove all.
Find time: 0.0793855s

Test 3, Fill 4th Find all 4th repeat 4 times, Remove all.
Find time 1: 0.000564571s
Find time 2: 0.0015891s
Find time 3: 0.00437044s
Find time 4: 0.00876081s

Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all.
Find time 1: 0.000633758s
Find time 2: 0.00191262s
Find time 3: 0.00445457s
Find time 4: 0.00694449s

Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times.
Find time 1: 0.000473796s
Find time 2: 0.00117536s
Find time 3: 0.00186862s
Find time 4: 0.00250847s

Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times.
Find time 1: 0.000614662s
Find time 2: 0.00208725s
Find time 3: 0.00360384s
Find time 4: 0.00395227s

i = 65536

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.0259888s

Test 2, Fill, Find random i*10 times, Remove all.
Find time: 8.28235s

Test 3, Fill 4th Find all 4th repeat 4 times, Remove all.
Find time 1: 0.102685s
Find time 2: 0.414142s
Find time 3: 0.939425s
Find time 4: 1.677s

Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all.
Find time 1: 0.0995426s
Find time 2: 0.417251s
Find time 3: 0.600362s
Find time 4: 0.819243s

Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times.
Find time 1: 0.0991375s
Find time 2: 0.229639s
Find time 3: 0.406747s
Find time 4: 0.643286s

Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times.
Find time 1: 0.101271s
Find time 2: 0.229471s
Find time 3: 0.404046s
Find time 4: 0.644134s

i = 1048576

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.100761s

Test 2, Fill, Find random i*10 times, Remove all.

Ended
 */

/* Partial Cache Array, Cache Amount = round( ln(index)^2 + 1 )
i = 4096

Test 1, Fill, Find all 10 times, Remove all.
Find time: 8.4962e-05s

Test 2, Fill, Find random i*10 times, Remove all.
Find time: 0.0628342s

Test 3, Fill 4th Find all 4th repeat 4 times, Remove all.
Find time 1: 0.000486527s
Find time 2: 0.00181493s
Find time 3: 0.00349093s
Find time 4: 0.00630825s

Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all.
Find time 1: 0.000518906s
Find time 2: 0.00155866s
Find time 3: 0.00393567s
Find time 4: 0.00623048s

Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times.
Find time 1: 0.00047518s
Find time 2: 0.00101678s
Find time 3: 0.00166216s
Find time 4: 0.00251677s

Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times.
Find time 1: 0.000495936s
Find time 2: 0.00099381s
Find time 3: 0.00197019s
Find time 4: 0.00252479s

i = 65536

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.00142914s

Test 2, Fill, Find random i*10 times, Remove all.
Find time: 6.35653s

Test 3, Fill 4th Find all 4th repeat 4 times, Remove all.
Find time 1: 0.0825264s
Find time 2: 0.300248s
Find time 3: 0.682222s
Find time 4: 1.19864s

Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all.
Find time 1: 0.0757344s
Find time 2: 0.308279s
Find time 3: 0.46044s
Find time 4: 0.604559s

Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times.
Find time 1: 0.0758705s
Find time 2: 0.172672s
Find time 3: 0.30814s
Find time 4: 0.471125s

Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times.
Find time 1: 0.0764318s
Find time 2: 0.169662s
Find time 3: 0.301403s
Find time 4: 0.466643s

i = 1048576

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.0151429s

Test 2, Fill, Find random i*10 times, Remove all.

Ended
*/

/* Cahced list
i = 4096

Test 1, Fill, Find all 10 times, Remove all.
Find time: 8.4685e-05s

Test 2, Fill, Find random i*10 times, Remove all.
Find time: 0.144503s

Test 3, Fill 4th Find all 4th repeat 4 times, Remove all.
Find time 1: 0.00114907s
Find time 2: 0.00458935s
Find time 3: 0.012283s
Find time 4: 0.0218948s

Test 4, Fill 4th Find rand 4th repeat 4 times, Remove all.
Find time 1: 0.000753037s
Find time 2: 0.00267452s
Find time 3: 0.00565318s
Find time 4: 0.0177231s

Test 5, Fill 4th Find all 4th Remove 8th repeat 4 times.
Find time 1: 0.00115073s
Find time 2: 0.00266262s
Find time 3: 0.00467044s
Find time 4: 0.00778803s

Test 6, Fill 4th, Find all rand 4th, Remove 8th, repeat 4 times.
Find time 1: 0.00116318s
Find time 2: 0.00413326s
Find time 3: 0.00476564s
Find time 4: 0.00865232s

i = 65536

Test 1, Fill, Find all 10 times, Remove all.
Find time: 0.00127084s

Test 2, Fill, Find random i*10 times, Remove all.

Ended
 */