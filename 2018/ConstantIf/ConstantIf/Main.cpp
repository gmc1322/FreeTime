#include <iostream>

#include "ConstantIf.h"

constexpr size_t Num = 0;

// Can be used with a named object, IE. variable
constexpr size_t function1()
{
  ConstIf( Num == 0 )
  {
    return 0;
  }
  ConstElseIf( Num == 1 )
  {
    return 1;
  }
  ConstElseIf( Num == 2 )
  {
    return 0;
  }
  ConstElseIf( Num == 3 )
  {
    return 3;
  }
  else
  {
    return 4;
  }
}

// Can't use this with any named object
constexpr size_t function2()
{
  ConstIf( Num == 0 )
  {
    return function1();
  }
  ConstElseIf( Num == 1 )
  {
    return function1();
  }
  else
  {
    ConstIf( Num == 2 )
    {
      return 2;
    }
    ConstElseIf( Num == 3 )
    {
      return 3;
    }
    else
    {
      return 4;
    }
  }
}

// Can call no-name return function in another no-name return function
constexpr size_t function3()
{
  ConstIf( Num == 0 )
  {
    return function2();
  }
  ConstElseIf( Num == 1 )
  {
    return function2();
  }
  else
  {
    ConstIf( Num == 2 )
    {
      return function2();
    }
    ConstElseIf( Num == 3 )
    {
      return function2();
    }
    else
    {
      return function2();
    }
  }
}

// Can't use for named return if it contains no-name returns
constexpr size_t function4()
{
  ConstIf( Num == 0 )
  {
    return function2();
  }
  ConstElseIf( Num == 1 )
  {
    return function2();
  }
  else
  {
    return function2();
  }
}

// Can double up named returns w/ other named returns
constexpr size_t function5()
{
  ConstIf( Num == 0 )
  {
    return !function1();
  }
  ConstElseIf( Num == 1 )
  {
    return !function1();
  }
  ConstElseIf( Num == 2 )
  {
    return function1();
  }
  ConstElseIf( Num == 3 )
  {
    return function1();
  }
  else
  {
    return function1();
  }
}

int main()
{
  constexpr bool Condition = function1();
  //constexpr bool Condition1 = function3();
  //constexpr bool Condition1 = function4();
  constexpr bool Condition1 = function5();

  ConstIf( Condition )
  {
    std::cout << "If Condition == True" << std::endl;
  }

  ConstIf( Condition )
  {
    std::cout << "If Condition == True" << std::endl;
  }
  else
  {
    std::cout << "Else Condition == False" << std::endl;
  }

  ConstIf( Condition )
  {
    std::cout << "If Condition == True" << std::endl;
  }
  ConstElseIf( Condition1 )
  {
    std::cout << "ElseIf Condition1 == True" << std::endl;
  }

  ConstIf( Condition )
  {
    ConstIf( Condition1 )
    {
      ConstIf( Num == 3 )
      {
        std::cout << "If Condition == True, If Condition1 == True, If Num == 3" << std::endl;
      }
      else
      {
        std::cout << "If Condition == True, If Condition1 == True, If Num != 3" << std::endl;
      }
    }
  }

  return 0;
}