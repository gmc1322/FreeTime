#include <iostream>

#include "ObjectAllocator.h"

struct TestStruct
{
  int var1;
  int var2;

  int get() const { return var1;  }
};

int main()
{
  ObjectAllocator< TestStruct, 1 > TestAllocator{};

  auto NewObject1 = TestAllocator.New();

  NewObject1->var1 = 5;
  NewObject1->var2 = NewObject1->get();

  std::cout << NewObject1->var2 << std::endl;

  TestAllocator.Delete( NewObject1 );
}
