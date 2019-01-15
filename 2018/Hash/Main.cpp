
#include "Hash.h"

#include <string>
#include <random>
#include <chrono>
#include <cstdlib>
#include <vector>
#include <iostream>

#define Now std::chrono::system_clock::now()

int main()
{
  constexpr size_t Count = 1'000'000;
  constexpr Size_T DefaultSTLCapacity = 15;

  Size_T Capacity = DefaultSTLCapacity;

  std::vector< bool > Filled( DefaultSTLCapacity );

  decltype( Now - Now ) TimeStamp{};

  {
    Hash HashTable{ DefaultSTLCapacity };

    std::vector< std::string > Keys( DefaultSTLCapacity );

    for( size_t j = 0; j < Count; ++j )
    {
      std::string Num = std::to_string( std::rand() );
      const char *const StrNum = Num.c_str();

      Size_T Index;

      {
        const auto Stamp = Now;
        Index = HashTable.GetInitialIndex( StrNum );
        const auto EndStamp = Now;

        TimeStamp += EndStamp - Stamp;
      }

      if( Filled[ Index ] )
      {
        Size_T Probe;

        {
          const auto Stamp = Now;
          Probe = HashTable.GetInitialProbeIndex( StrNum );
          const auto EndStamp = Now;

          TimeStamp += EndStamp - Stamp;
        }

        const Size_T StartIndex = Index;

        do
        {
          Index = HashTable.ProbeArray( Index, Probe );

          if( Index == StartIndex )
          {
            {
              const Size_T NewCapacity = Capacity * 2;

              HashTable.ResetTableSize( NewCapacity );

              std::vector< bool > NewFilled( NewCapacity );
              std::vector< std::string > NewKeys( NewCapacity );

              for( size_t k = 0; k < Capacity; ++k )
              {
                if( Filled[ k ] )
                {
                  {
                    const char *const StrNumK = Keys[ k ].c_str();

                    const auto Stamp = Now;
                    Index = HashTable.GetInitialIndex( StrNumK );
                    const auto EndStamp = Now;

                    TimeStamp += EndStamp - Stamp;
                  }

                  if( NewFilled[ Index ] )
                  {
                    {
                      const auto Stamp = Now;
                      Probe = HashTable.GetInitialProbeIndex( StrNum );
                      const auto EndStamp = Now;

                      TimeStamp += EndStamp - Stamp;
                    }

                    do
                    {
                      Index = HashTable.ProbeArray( Index, Probe );
                    }
                    while( NewFilled[ Index ] );

                    NewFilled[ Index ] = true;
                    NewKeys[ Index ] = std::move( Keys[ k ] );
                  }
                  else
                  {
                    NewFilled[ Index ] = true;
                    NewKeys[ Index ] = std::move( Keys[ k ] );
                  }
                }
              }

              Filled = std::move( NewFilled );
              Keys = std::move( NewKeys );

              Capacity = NewCapacity;
            }

            {
              const auto Stamp = Now;
              Index = HashTable.GetInitialIndex( StrNum );
              const auto EndStamp = Now;

              TimeStamp += EndStamp - Stamp;
            }

            if( Filled[ Index ] )
            {
              {
                const auto Stamp = Now;
                Probe = HashTable.GetInitialProbeIndex( StrNum );
                const auto EndStamp = Now;

                TimeStamp += EndStamp - Stamp;
              }

              if( Filled[ Probe ] )
              {
                do
                {
                  Index = HashTable.ProbeArray( Index, Probe );
                }
                while( Filled[ Index ] );

                break;
              }
              else Index = Probe; break;
            }
            else break;
          }
        }
        while( Filled[ Index ] );

        Filled[ Index ] = true;
        Keys[ Index ] = std::move( Num );
      }
      else
      {
        Filled[ Index ] = true;
        Keys[ Index ] = std::move( Num );
      }
    }
  }

  std::cout << "String Hash Total Time: " << ( std::chrono::duration< double >{ TimeStamp }.count() ) << std::endl;
  std::cout << "String Hash Ave Time: "   << ( std::chrono::duration< double >{ TimeStamp }.count() ) / Count << std::endl;

  Capacity = DefaultSTLCapacity;

  Filled.clear();
  Filled.resize( DefaultSTLCapacity );

  std::vector< void* > Keys( DefaultSTLCapacity );

  PtrHash HashTable{ DefaultSTLCapacity };

  TimeStamp = decltype( Now - Now ){};

  for( size_t j = 0; j < Count; ++j )
  {
    void *const StrNum = new bool;

    Size_T Index;

    {
      const auto Stamp = Now;
      Index = HashTable.GetInitialIndex( StrNum );
      const auto EndStamp = Now;

      TimeStamp += EndStamp - Stamp;
    }

    if( Filled[ Index ] )
    {
      Size_T Probe;

      {
        const auto Stamp = Now;
        Probe = HashTable.GetInitialProbeIndex( StrNum );
        const auto EndStamp = Now;

        TimeStamp += EndStamp - Stamp;
      }

      const Size_T StartIndex = Index;

      do
      {
        Index = HashTable.ProbeArray( Index, Probe );

        if( Index == StartIndex )
        {
          {
            const Size_T NewCapacity = Capacity * 2;

            HashTable.ResetTableSize( NewCapacity );

            std::vector< bool > NewFilled( NewCapacity );
            std::vector< void* > NewKeys( NewCapacity );

            for( size_t k = 0; k < Capacity; ++k )
            {
              if( Filled[ k ] )
              {
                {
                  const void *const StrNumK = Keys[ k ];

                  const auto Stamp = Now;
                  Index = HashTable.GetInitialIndex( StrNumK );
                  const auto EndStamp = Now;

                  TimeStamp += EndStamp - Stamp;
                }

                if( NewFilled[ Index ] )
                {
                  {
                    const auto Stamp = Now;
                    Probe = HashTable.GetInitialProbeIndex( StrNum );
                    const auto EndStamp = Now;

                    TimeStamp += EndStamp - Stamp;
                  }

                  do
                  {
                    Index = HashTable.ProbeArray( Index, Probe );
                  }
                  while( NewFilled[ Index ] );

                  NewFilled[ Index ] = true;
                  NewKeys[ Index ] = Keys[ k ];
                }
                else
                {
                  NewFilled[ Index ] = true;
                  NewKeys[ Index ] = Keys[ k ];
                }
              }
            }

            Filled = std::move( NewFilled );
            Keys = std::move( NewKeys );

            Capacity = NewCapacity;
          }

          {
            const auto Stamp = Now;
            Index = HashTable.GetInitialIndex( StrNum );
            const auto EndStamp = Now;

            TimeStamp += EndStamp - Stamp;
          }

          if( Filled[ Index ] )
          {
            {
              const auto Stamp = Now;
              Probe = HashTable.GetInitialProbeIndex( StrNum );
              const auto EndStamp = Now;

              TimeStamp += EndStamp - Stamp;
            }

            if( Filled[ Probe ] )
            {
              do
              {
                Index = HashTable.ProbeArray( Index, Probe );
              }
              while( Filled[ Index ] );

              break;
            }
            else Index = Probe; break;
          }
          else break;
        }
      }
      while( Filled[ Index ] );

      Filled[ Index ] = true;
      Keys[ Index ] = StrNum;
    }
    else
    {
      Filled[ Index ] = true;
      Keys[ Index ] = StrNum;
    }
  }

  std::cout << "Ptr Hash Total Time: " << ( std::chrono::duration< double >{ TimeStamp }.count() ) << std::endl;
  std::cout << "Ptr Hash Ave Time: "   << ( std::chrono::duration< double >{ TimeStamp }.count() ) / Count << std::endl;

  return 0;
}