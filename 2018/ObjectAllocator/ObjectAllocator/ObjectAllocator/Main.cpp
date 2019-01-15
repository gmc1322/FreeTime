#include <iostream>
#include <random>
#include <time.h>

#include "ObjectAllocator.h"

struct TestStruct
{
  int var1;
  int var2;

  int get() const { return var1;  }
};

int main()
{
  ObjectAllocator< TestStruct > TestAllocator{ 4 };

  auto NewObject1 = TestAllocator.New();

  NewObject1->var1 = 5;
  NewObject1->var2 = NewObject1->get();

  std::cout << NewObject1->var2 << std::endl;

  auto NewObject2 = TestAllocator.New();

  TestAllocator.Delete( NewObject1 );

  auto NewArray = TestAllocator.New( 2 );

  NewArray[ 0 ].var1 = 7;

  ( NewArray + 1 )->var2 = NewArray->get();

  try
  {
    if( ( ( NewArray + 1 ) + 1 )->get() )
    { 
    }
  }
  catch (...)
  {
  }

  std::cout << ( NewArray + 1 ) - ( NewArray + 0 ) << std::endl;
  std::cout <<   NewArray       - ( NewArray + 1 ) << std::endl;
  std::cout << ( NewArray + 1 ) -   NewArray       << std::endl;
  std::cout <<   NewArray       -   NewArray       << std::endl;

  TestAllocator.Delete( NewArray );

  std::cout << std::endl;

  time_t StartTime;
  time_t EndTime;

  constexpr size_t ObjectCount = 10'000'000;

  std::cout << std::endl;

  TestStruct* *Array = new TestStruct*[ ObjectCount ];

  time( &StartTime );

  for( size_t j = 0; j < ObjectCount; ++j )
  {
    Array[ j ] = new TestStruct;
  }

  for( size_t j = 0; j < ObjectCount; ++j )
  {
    Array[ j ]->var1 = std::rand();
    Array[ j ]->var2 = Array[ j ]->get();
  }

  for( size_t j = 0; j < ObjectCount; ++j )
  {
    delete Array[ j ];
  }

  time( &EndTime );

  std::cout << "New/Delete time: " << EndTime - StartTime << std::endl << std::endl;

  delete[] Array;

  ObjectAllocator< TestStruct >::ObjectPtr *Array1 = new ObjectAllocator< TestStruct >::ObjectPtr[ ObjectCount ];

  for( size_t i = 1; ObjectCount / i > 100; i += 10 )
  {
    time( &StartTime );

    ObjectAllocator< TestStruct > TimedAllocator{ ObjectCount / i };

    for( size_t j = 0; j < ObjectCount; ++j )
    {
      Array1[ j ] = TimedAllocator.New();
    }
    
    for( size_t j = 0; j < ObjectCount; ++j )
    {
      Array1[ j ]->var1 = std::rand();
      Array1[ j ]->var2 = Array1[ j ]->get();
    }

    /*for( size_t j = 0; j < ObjectCount; ++j )
    {
      TimedAllocator.Delete( Array1[ j ] );
    }*/

    time( &EndTime );

    std::cout << ObjectCount / i << " Objects Per Page time: " << EndTime - StartTime << std::endl;
  }

  delete[] Array1;

  return 0;
}
