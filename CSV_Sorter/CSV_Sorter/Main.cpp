#include "Sorter.h"

#include <random>
#include <unordered_map>
#include <string>
#include <iostream>

template< size_t ParameterCount >
void GenerateCSV( const char *const FileName, const ParameterArray< ParameterCount > &Parameters, size_t RowCount ) noexcept
{
  std::ofstream File{ FileName };

  {
    CSV::Row Temp;

    Temp.Elements.reserve( ParameterCount );

    for( size_t i = 0; i < ParameterCount; ++i )
    {
      Temp.Elements.emplace_back( "Header: '" + std::to_string( i + 1 ) + "'" );
    }

    File << Temp;
  }

  std::mt19937 Generator{ std::random_device{}() };

  using RandomType = std::uniform_int_distribution< size_t >;

  std::pmr::unordered_map< size_t, RandomType > Ranges{};

  Ranges.reserve( ParameterCount );

  for( size_t i = 0; i < ParameterCount; ++i )
  {
    const size_t Size = Parameters[ i ].Priorities.size();

    if( Ranges.find( Size ) == Ranges.cend() )
    {
      Ranges.emplace( Size, RandomType{ 0, Size - 1 } );
    }
  }

  // Greater since RowCount starts at 1
  for( ; RowCount > 0; --RowCount )
  {
    CSV::Row Temp;

    Temp.Elements.resize( ParameterCount );

    for( size_t i = 0; i < ParameterCount; ++i )
    {
      const SortingParameter &Parameter = Parameters[ i ];

      Temp.Elements[ Parameter.Index ] = Parameter.Priorities[ Ranges[ Parameter.Priorities.size() ]( Generator ) ];
    }

    File << Temp;
  }
}

int main()
{
#if 0
  constexpr size_t DataSize = 5000;

  const auto Array{ MakeParameterArray( SortingParameter{ 2, { "T1", "T2", "T3", "T4" } },
                                        SortingParameter{ 1, { "V0", "V1", "V2", "V3", "V4", "V5" } },
                                        SortingParameter{ 0, { "High", "Medium", "Low", "Backlog" } } ) };

  GenerateCSV( "TestData.csv", Array, DataSize );

  CSV_Sort( "TestData.csv", "OutData.csv", Array );
#else
  constexpr size_t DataSize = 50000;

  const auto Array{ MakeParameterArray( SortingParameter{ 5, { "T1", "T2", "T3", "T4" } },
                                        SortingParameter{ 4, { "V0", "V1" } },
                                        SortingParameter{ 3, { "High", "Medium", "Low", "Backlog" } },
                                        SortingParameter{ 2, { "FALSE", "TRUE" } },
                                        SortingParameter{ 1, { "Minutes", "Hours", "Days", "Unknown" } },
                                        SortingParameter{ 0, { "Trivial", "Medium", "Hard", "Unknown" } } ) };

  GenerateCSV( "TestData.csv", Array, DataSize );

  CSV_Sort( "TestData.csv", "OutData.csv", Array );
#endif

  return 0;
}
