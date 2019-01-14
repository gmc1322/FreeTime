#include "BehaviorTree.h"
#include "ETF2.h"
#include "ETF1.h"

int SmallMultiTestFunction( void *Data )
{
  std::cout << "SmallMultiTreeTop" << std::endl;

  auto TestData = ( ETF2Num* )Data;

  if( *TestData >= One )
  {
    *TestData = static_cast< ETF2Num >( *TestData - 1 );

    return *TestData;
  }

  return BehaviorTreeSpace::Prev;
}

int SmallTestFunction1( void *Data )
{
  std::cout << "SmallTopDynamic" << std::endl;

  auto TestData = ( ETF2Num* )Data;

  if( *TestData >= One )
  {
    return *TestData - 1;
  }

  return BehaviorTreeSpace::Prev;
}

int TestSmallOne1( void *Data )
{
  std::cout << "RightDynamic" << std::endl;

  auto TestData = ( ETF2Num* )Data;

  *TestData = One;

  return BehaviorTreeSpace::Prev;
}

int TestSmallZero1( void *Data )
{
  std::cout << "LeftDynamic" << std::endl;

  auto TestData = ( ETF2Num* )Data;

  *TestData = Zero;

  return BehaviorTreeSpace::Prev;
}

int MediumMultiParentFunction( void *Data )
{
  std::cout << "MediumTopDynamic" << std::endl;

  auto TestData = ( ETF2Num* )Data;

  switch( *TestData )
  {
    case Zero:
      ResetData12();
      *TestData = One;
      return One;
   
    case One:
      return BehaviorTreeSpace::Prev;

    case Two: 
      ResetData12();
      *TestData = Zero;
      return Zero;

    case Three:
      *TestData = Two;
      return Two;
  }

  return BehaviorTreeSpace::Prev;
}
