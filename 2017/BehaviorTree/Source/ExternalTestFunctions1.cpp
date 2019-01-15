#include "ETF2.h"
#include "BehaviorTree.h"

static BehaviorTreeSpace::BehaviorTree *TestTree;

static ETF2Num TestData = Two;

static ETF2Num TestDataChild1 = Two;
static ETF2Num TestDataChild2 = Two;

void ResetData12()
{
  TestDataChild1 = Two;
  TestDataChild2 = Two;
}

void ResetData()
{
  TestData = Two;
}

void ResetAll()
{
  ResetData();
  ResetData12();
}

void * GetTestData()
{
  return &TestData;
}

void * GetChildData()
{
  return &TestDataChild1;
}

void InitTree()
{
  // Create Tree, comes with trunk
  TestTree = new BehaviorTreeSpace::BehaviorTree( SmallTestFunction1, &TestData, 0, ResetAll );

  // Add first branch
  TestTree->AddBranch( TestSmallZero1 );

  // Add Second Branch
  TestTree->AddBranch( TestSmallOne1 );
}

bool RunTree()
{
  static auto Run = true;

  if( Run )
  {
    Run = TestTree->RunBehaviorTree();
  }

  return Run;
}

void ShutdownTree()
{
  delete TestTree;
}


BehaviorTreeSpace::BehaviorTreePtr InitTreeChild()
{
  static bool CreatedOneChild;

  // Create Tree, comes with trunk
  BehaviorTreeSpace::BehaviorTreePtr TestTreeChild;
  
  if( CreatedOneChild )
  {
    TestTreeChild = new BehaviorTreeSpace::BehaviorTree( SmallTestFunction1, &TestDataChild2, 0, ResetData12 );
  }
  else
  {
    TestTreeChild = new BehaviorTreeSpace::BehaviorTree( SmallTestFunction1, &TestDataChild1, 0, ResetData12 );

    CreatedOneChild = true;
  } 

  // Add first branch
  TestTreeChild->AddBranch( TestSmallZero1 );

  // Add Second Branch
  TestTreeChild->AddBranch( TestSmallOne1 );

  return TestTreeChild;
}

void InitMultiTree()
{
  // Create Tree, comes with trunk
  TestTree = new BehaviorTreeSpace::BehaviorTree( SmallMultiTestFunction, &TestData, 0, ResetAll );

  // Add first Tree
  TestTree->AddTree( InitTreeChild );

  // Add second Tree
  TestTree->AddTree( InitTreeChild );
}

bool RunMultiTree()
{
  static auto Run = true;

  if( Run )
  {
    Run = TestTree->RunBehaviorTree();
  }
  
  return Run;
}

void ShutdownMultiTree()
{
  delete TestTree;
}


void InitMultiParentTree()
{
  TestData = Three;

  // Create Tree, comes with trunk
  TestTree = new BehaviorTreeSpace::BehaviorTree( MediumMultiParentFunction, &TestData, 0, ResetData );

  // Add first Branch, second Branch, third Branch
  TestTree->AddBranch( SmallTestFunction1, 1 );
  TestTree->AddBranch( SmallTestFunction1, 1 );
  TestTree->AddBranch( SmallTestFunction1, 1 );

  // Add first/second Branches to all three parents
  TestTree->AddBranch( TestSmallZero1, 1, { { 0 },{ 1 },{ 2 } } );
  TestTree->AddBranch( TestSmallOne1, 1, { { 0 },{ 1 },{ 2 } } );

  // Add Data
  TestTree->AddData( &TestDataChild1, 1 );
}

bool RunMultiParentTree()
{
  static auto Run = true;

  if( Run )
  {
    Run = TestTree->RunBehaviorTree();
  }

  return Run;
}

void ShutdownMultiParentTree()
{
  delete TestTree;
}