
#include "Union.h"

#include <iostream>

template< typename Type >
using IsTypeDefaultSmall = std::bool_constant< std::is_arithmetic< Type >::value && sizeof( Type ) <= sizeof( void* ) >;

template< typename Type >
using FuncArgType = typename std::conditional< IsTypeDefaultSmall< Type >::value, const Type,
                                               typename std::conditional< std::is_pointer< Type >::value, const Type *const,
                                                                          typename std::conditional< std::is_move_constructible< Type >::value, Type, const Type & >::type >::type >;

int main()
{
  struct Test1
  {
    Test1( Test1&& ) = delete;

    long long i;
    long int *j;
  };
  
  struct Test2
  {
    long long i;
    long int *j;
  };

  Union< unsigned char > Union2{};

  Union2.SetType< bool >( true    );
  Union2.SetType< bool >( true, 2 );
  Union2.SetType< bool >( true, 4 );
  Union2.SetType< bool >( true, 6 );

  std::cout << static_cast< unsigned >( Union2.GetType< unsigned char >() );

  return 0;
}
