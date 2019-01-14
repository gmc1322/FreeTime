
#include <iostream>
#include <ctime>
#include <algorithm>
#include <execution>

#include "SortSuite.h"

struct Test
{
  Test() noexcept = default;

  explicit Test( const int i ) noexcept : i( i ) {}

  Test( const Test &Copy ) noexcept = default;


  bool operator<( const Test &Right ) const noexcept
  {
    return i < Right.i;
  }

  static void swap( Test &Left, Test &Right ) noexcept
  {
    const Test Temp{ Left };

    Left = Right;

    Right = Temp;
  }

  int i = 0;

  void *ptr = nullptr;
};

int main()
{
  constexpr size_t Size = 165'859'953;

  auto const Array1 = new Test[ static_cast< size_t >( Size ) ];
  auto const Array2 = new Test[ static_cast< size_t >( Size ) ];

  for( size_t j = 0; j < Size; ++j )
  {
    Array2[ j ] = Array1[ j ] = static_cast< Test >( rand() );
  }

  {
    const time_t StartTime = time( nullptr );

    std::sort( Array1, Array1 + Size );

    const time_t EndTime = time( nullptr );

    std::cout << "std::sort Time Took = " << EndTime - StartTime << ", Item Count = " << Size << std::endl;
  }

  {
    const time_t StartTime = time( nullptr );

    std::sort( std::execution::parallel_unsequenced_policy{}, Array2, Array2 + Size );

    const time_t EndTime = time( nullptr );

    std::cout << "std::sort Threaded Time Took = " << EndTime - StartTime << ", Item Count = " << Size << std::endl;
  }

  return 0;
}
