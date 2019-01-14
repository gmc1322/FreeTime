#include <fstream>
#include <iostream>
#include <vector>

#include "Strings.h"

#include "MyMultiLevelQue.h"

#include "TrunkWindow.h"
#include "TreeWindow.h"
#include "BranchWindow.h"
#include "NullWindow.h"
#include "MultiWindow.h"
#include "CheckWindow.h"
#include "ChildWindow.h"

static const auto & TrunkID = BehaviorTreeWindow_Base::TrunkID;

static std::string FileOutput;

static std::vector< std::string > DataPrototypes;
static std::vector< std::string > BranchPrototypes;
static std::vector< std::string > TreePrototypes;

static std::map< size_t, std::string > KeyErrors;

static std::map< size_t, BehaviorTreeWindow_BaseDoubleConstPtr > Windows;

static std::map< size_t, size_t > IDTable;

static void SetBehaviorTreeReturnFunction( const char *Function, const char *Name )
{
  TreePrototypes.push_back( Function );

  std::string Temp( "BehaviorTreeSpace::BehaviorTreePtr " );

  Temp << Function << "();\n";

  std::string Temp2( "BehaviorTreeSpace::BehaviorTreePtr " );

  Temp2 << Name;

  FileOutput.insert( FileOutput.find( Temp2 ), Temp );
}

static void SetKeyError( size_t Key ) noexcept
{
  std::string Temp( "Get Data function with Key: " );

  Temp << Key << " has no function to run!";

  KeyErrors.emplace( std::make_pair( Key, Temp ) );
}

static void FindParents( size_t StartID )
{
  std::string Reverse( "} " );

  while( true )
  {
    auto ParentsID = Windows.find( IDTable.find( StartID )->second )->second->GetParentID();

    auto Parent = Windows.find( ParentsID == TrunkID ? 0 : IDTable.find( ParentsID + 1 )->second )->second;

    if( Parent->GetWindowType() != BehaviorTreeWindow_Base::Check )
    {
      size_t Count = 0;

      std::map< size_t, size_t > Children;

      auto ParentChildren = Parent->GetChildren();

      auto end = ParentChildren.end();

      for( auto iter : Windows )
      {
        if( Count >= ParentChildren.size() )
        {
          break;
        }

        if( ParentChildren.find( iter.second->GetID() - 1 ) != end )
        {
          Children.emplace( std::make_pair( iter.second->GetID(), Count ) );

          ++Count;
        }     
      }

      Count = Children.find( StartID )->second;

      if( ParentsID == TrunkID )
      {
        Reverse << Count << " ";

        break;
      }

      Reverse << Count << " ,";
    }

    StartID = ParentsID + 1;
  }

  Reverse << "{";

  std::reverse( Reverse.begin(), Reverse.end() );

  FileOutput << Reverse;
}

static void AddParents( BehaviorTreeWindow_BaseDoubleConstPtr CurrentNode ) noexcept
{
  auto ParentID = CurrentNode->GetParentID();

  auto OtherParents = CurrentNode->GetOtherParents();

  if( ParentID != TrunkID || OtherParents.size() > 0 )
  {    
    FileOutput << ", { ";

    FindParents( ParentID + 1 );

    for( auto iter : OtherParents )
    {
      if( iter.first != TrunkID )
      {
        FileOutput << ", ";

        FindParents( iter.first + 1 );
      }
    }

    FileOutput << " }";
  }

  FileOutput << " );\n";
}

static bool RunMetaDataFunction( const std::pair< size_t, InputPtr > &Data, const char *Name ) noexcept
{
  auto Function = Data.second->GetInput();

  if( !( Function[ 0 ] ) )
  {
    SetKeyError( Data.first );

    return false;
  }

  FileOutput << "  Tree->AddData( " << Function << "(), " << Data.first << " );\n";

  for( auto iter : DataPrototypes )
  {
    if( !iter.compare( Function ) )
    {
      return true;
    }
  }

  DataPrototypes.push_back( Function );

  std::string Temp( "void * " );

  Temp << Function << "();\n";

  std::string Temp2( "BehaviorTreeSpace::BehaviorTreePtr " );

  Temp2 << Name;

  FileOutput.insert( FileOutput.find( Temp2 ), Temp );

  return true;
}

static bool RunMetaBranchFunction( BehaviorTreeWindow_BranchDoubleConstPtr CurrentNode, const char *Name,
                                   const std::map< size_t, InputPtr > &InputData ) noexcept
{
  auto Return = false;

  auto BranchKey = CurrentNode->GetDataKey();

  if( BranchKey >= InputData.size() )
  {
    SetKeyError( BranchKey );

    Return = true;
  }

  auto Function = CurrentNode->GetFunctionName();

  if( !( Function[ 0 ] ) )
  {
    std::cout << "Child Branch: " << CurrentNode->GetNodeName() << " has no function to run!" << std::endl;

    return false;
  }

  if( !( Name[ 0 ] ) || Return )
  {
    return false;
  }

  FileOutput << "  Tree->AddBranch( " << Function << ", " << BranchKey;

  AddParents( CurrentNode );

  for( auto iter : BranchPrototypes )
  {
    if( !iter.compare( Function ) )
    {
      return true;
    }
  }

  BranchPrototypes.push_back( Function );

  std::string Temp( "int " );

  Temp << Function << "( void *Data );\n";

  std::string Temp2( "BehaviorTreeSpace::BehaviorTreePtr " );

  Temp2 << Name;

  FileOutput.insert( FileOutput.find( Temp2 ), Temp );

  return true;
}

static bool RunMetaTreeFunction( BehaviorTreeWindow_TreeDoubleConstPtr CurrentNode, const char *Name ) noexcept
{
  auto Function = CurrentNode->GetFunctionName();

  if( !( Function[ 0 ] ) )
  {
    std::cout << "Tree: " << CurrentNode->GetNodeName() << " has no initializer function!" << std::endl;

    return false;
  }

  if( !( Name[ 0 ] ) )
  {
    return false;
  }

  FileOutput << "  Tree->AddTree( " << Function;
  
  AddParents( CurrentNode );

  for( auto iter : TreePrototypes )
  {
    if( !iter.compare( Function ) )
    {
      return true;
    }
  }

  SetBehaviorTreeReturnFunction( Function, Name );

  return true;
}

static void RunMetaNullFunction( BehaviorTreeWindow_NullDoubleConstPtr CurrentNode ) noexcept
{
  FileOutput << "  Tree->AddBranch( nullptr, 0";

  AddParents( CurrentNode );
}

static bool RunMetaTrunkFunction( BehaviorTreeWindow_TrunkDoubleConstPtr CurrentNode, const char * *Name,
                                  const std::map< size_t, InputPtr > &InputData, size_t *Key ) noexcept
{
  *Name = CurrentNode->GetNodeName();

  *Key = CurrentNode->GetDataKey();

  auto Return = false;

  auto DataIter = InputData.find( *Key );

  const char *Data = nullptr;

  if( DataIter == InputData.end() )
  {
    SetKeyError( *Key );

    Return = true;
  }
  else
  {
    Data = DataIter->second->GetInput();

    if( !( Data[ 0 ] ) )
    {
      SetKeyError( *Key );

      Return = true;
    }
  }

  auto Function = CurrentNode->GetFunctionName();

  if( !( Function[ 0 ] ) )
  {
    std::cout << "No function for the Trunk to run!" << std::endl;

    Return = true;
  }

  if( !( Name[ 0 ] ) )
  {
    std::cout << "No name for the Behavior Tree!" << std::endl;

    Return = true;
  }

  if( Return )
  {
    return false;
  }

  BranchPrototypes.push_back( Function );
  DataPrototypes.push_back( Data );

  FileOutput << "#include <BehaviorTree.h>\n";
  FileOutput << "int " << Function << "( void *Data );\n";
  FileOutput << "void * " << Data << "();\n";

  auto OnFinish = CurrentNode->GetOnFinishName();

  if( OnFinish[ 0 ] )
  {
    FileOutput << "void " << OnFinish << "();\n";
  }

  auto OnReset = CurrentNode->GetOnResetName();

  if( OnReset[ 0 ] && !std::string( OnFinish ).compare( OnReset ) )
  {
    FileOutput << "void " << OnReset << "();\n";
  }

  FileOutput << "BehaviorTreeSpace::BehaviorTreePtr " << *Name << "() noexcept\n";
  FileOutput << "{\n";
  FileOutput << "  auto Tree = new BehaviorTreeSpace::BehaviorTree( " << Function << ", " << Data << "(), " << *Key << ", ";

  if( OnFinish[ 0 ] )
  {
    FileOutput << OnFinish;
  }
  else
  {
    FileOutput << "nullptr";
  }

  if( OnReset[ 0 ] )
  {
    FileOutput << ", " << OnReset;
  }

  FileOutput << " );\n";

  return true;
}

static bool RunMetaMultiFunction( BehaviorTreeWindow_MultiDoubleConstPtr CurrentNode, const char *Name ) noexcept
{
  auto Return = false;

  if( !( Name[ 0 ] ) )
  {
    Return = true;
  }

  if( CurrentNode->GetChildren().empty() )
  {
    std::cout << "Multi Branch: " << CurrentNode->GetNodeName() << " needs at least one non-generated child!" << std::endl;

    Return = true;
  }

  if( CurrentNode->GetIsGenerating() )
  {
    auto GenFunc = CurrentNode->GetGenFunction();

    if( !( GenFunc[ 0 ] ) )
    {
      std::cout << "Multi Branch: " << CurrentNode->GetNodeName() << " has no gen function to run!" << std::endl;

      Return = true;
    }

    auto RunFunc = CurrentNode->GetRunFunction();

    if( !( RunFunc[ 0 ] ) )
    {
      std::cout << "Multi Branch: " << CurrentNode->GetNodeName() << " has no function to receive the generator!" << std::endl;

      Return = true;
    }

    auto Count = CurrentNode->GetGenCount();

    if( !std::stoul( Count ) )
    {
      std::cout << "Multi Branch: " << CurrentNode->GetNodeName() << " is generating 0 instances!" << std::endl;

      return false;
    }
    
    if( Return )
    {
      return false;
    }

    FileOutput << "  Tree->AddThreaded( " << GenFunc << ", " << Count << ", ";
    FileOutput << RunFunc << ", " << "BehaviorTreeSpace::ThreadedBranch::" << CurrentNode->GetGenType();

    auto SkipGenFunc = false;

    for( auto iter : BranchPrototypes )
    {
      if( !iter.compare( GenFunc ) )
      {
        SkipGenFunc = true;

        break;
      }
    }

    std::string Temp2( "BehaviorTreeSpace::BehaviorTreePtr " );

    Temp2 << Name;

    if( !SkipGenFunc )
    {
      BranchPrototypes.push_back( GenFunc );

      std::string Temp( "void * " );

      Temp << GenFunc << "();\n";

      FileOutput.insert( FileOutput.find( Temp2 ), Temp );
    }
    
    for( auto iter : BranchPrototypes )
    {
      if( !iter.compare( RunFunc ) )
      {
        goto SkipRunFunc; // This calls all the needed destructors
      }
    }

    BranchPrototypes.push_back( RunFunc );

    std::string Temp( "void " );

    Temp << RunFunc << "( void *Data );\n";

    FileOutput.insert( FileOutput.find( Temp2 ), Temp );
  }
  else if( Return )
  {
    return false;
  }
  else
  {
    FileOutput << "  Tree->AddThreaded( ";
  }

SkipRunFunc:
  AddParents( CurrentNode );

  return true;
}

static bool RunMetaCheckFunction( BehaviorTreeWindow_CheckDoubleConstPtr CurrentNode, const char *Name,
                                  const std::map< size_t, InputPtr > &InputData )
{
  auto Return = false;

  auto CheckKey = CurrentNode->GetDataKey();

  if( CheckKey >= InputData.size() )
  {
    SetKeyError( CheckKey );

    Return = true;
  }

  auto Function = CurrentNode->GetFunctionName();

  if( !( Function[ 0 ] ) )
  {
    std::cout << "Child CheckBranch: " << CurrentNode->GetNodeName() << " has no function to run!" << std::endl;

    return false;
  }

  if( !( Name[ 0 ] ) || Return )
  {
    return false;
  }

  FileOutput << "  Tree->AddCheck( " << Function << ", " << CheckKey;

  AddParents( CurrentNode );

  for( auto iter : BranchPrototypes )
  {
    if( !iter.compare( Function ) )
    {
      return true;
    }
  }

  BranchPrototypes.push_back( Function );

  std::string Temp( "bool " );

  Temp << Function << "( void *Data );\n";

  std::string Temp2( "BehaviorTreeSpace::BehaviorTreePtr " );

  Temp2 << Name;

  FileOutput.insert( FileOutput.find( Temp2 ), Temp );

  return true;
}

static bool RunMetaChildFunction( BehaviorTreeWindow_ChildDoubleConstPtr CurrentNode, const char *Name ) noexcept
{
  auto Return = false;

  auto Function = CurrentNode->GetFunctionName();

  if( !( Function[ 0 ] ) )
  {
    std::cout << "Child Tree: " << CurrentNode->GetNodeName() << " has no function to get its tree!" << std::endl;

    return false;
  }

  if( !( Name[ 0 ] ) || Return )
  {
    return false;
  }

  FileOutput << "  Tree->AddChildTree( " << Function;

  AddParents( CurrentNode );

  for( auto iter : BranchPrototypes )
  {
    if( !iter.compare( Function ) )
    {
      return true;
    }
  }

  SetBehaviorTreeReturnFunction( Function, Name );

  return true;
}

void CreateTree( const char *FileName, const std::map< size_t, InputPtr > &InputData ) noexcept
{
  std::ofstream file;
  std::string FullFileName;

  if( FileName[ 0 ] )
  {
    FullFileName << FileName << ".cpp";

    file.open( FullFileName );
  }
  else
  {
    file.open( "TestFile.cpp" );
  }

  system( "CLS" );

  if( file.is_open() )
  {
    FileOutput.clear();

    DataPrototypes.clear();

    BranchPrototypes.clear();

    TreePrototypes.clear();

    KeyErrors.clear();

    Windows.clear();

    IDTable.clear();

    {
      MutliLevelQue MyMultiLevelQue;

      auto AllWindows = BehaviorTreeWindow_Base::GetAllWindows();

      for( auto iter : AllWindows )
      {
        MyMultiLevelQue.AddToQue( iter.second->GetLocation(), iter.second );
      }

      MyMultiLevelQue.FillMap( Windows );

      for( auto iterOuter : AllWindows )
      {
        auto outerID = iterOuter.second->GetID();

        for( auto iterInner : Windows )
        {
          if( iterInner.second->GetID() == outerID )
          {
            IDTable.emplace( iterOuter.first, iterInner.first );

            break;
          }
        }
      }

      auto Return = false;

      size_t Key = 0;

      const char *Name = nullptr;

      for( auto iter : Windows )
      {
        if( iter.first == 0 )
        {
          if( !RunMetaTrunkFunction( static_cast< BehaviorTreeWindow_TrunkDoubleConstPtr >( iter.second ), &Name, InputData, &Key ) )
          {
            Return = true;
          }
        }
        else
        {
          switch( iter.second->GetWindowType() )
          {
            case BehaviorTreeWindow_Base::Branch:
              if( !RunMetaBranchFunction( static_cast< BehaviorTreeWindow_BranchDoubleConstPtr >( iter.second ), Name, InputData ) )
              {
                Return = true;
              }
              break;

            case BehaviorTreeWindow_Base::Null: 
              RunMetaNullFunction( static_cast< BehaviorTreeWindow_NullDoubleConstPtr >( iter.second ) );
              break;

            case BehaviorTreeWindow_Base::Tree: 
              if( !RunMetaTreeFunction( static_cast< BehaviorTreeWindow_TreeDoubleConstPtr >( iter.second ), Name ) )
              {
                Return = true;
              }
              break;

            case BehaviorTreeWindow_Base::Multi:
              if( !RunMetaMultiFunction( static_cast< BehaviorTreeWindow_MultiDoubleConstPtr >( iter.second ), Name ) )
              {
                Return = true;
              }
              break;            

            case BehaviorTreeWindow_Base::Check:
              if( !RunMetaCheckFunction( static_cast< BehaviorTreeWindow_CheckDoubleConstPtr >( iter.second ), Name, InputData ) )
              {
                Return = true;
              }
              break;
            
            case BehaviorTreeWindow_Base::Child:
              if( !RunMetaChildFunction( static_cast< BehaviorTreeWindow_ChildDoubleConstPtr >( iter.second ), Name ) )
              {
                Return = true;
              }
              break;
          }        
        }
      }

      for( auto iter : InputData )
      {
        if( iter.first != Key )
        {
          if( !RunMetaDataFunction( iter, Name ) )
          {
            Return = true;
          }
        }
      }

      if( Return )
      {
        for( auto iter : KeyErrors )
        {
          std::cout << iter.second << std::endl;
        }

        return;
      }
    }

    FileOutput << "  return Tree;\n";

    FileOutput << "}\n";

    file << FileOutput;

    file.flush();

    std::cout << FileOutput << std::endl;

    std::cout << "Successfully created file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.cpp" ) << "!" << std::endl;

    return;
  }

  std::cout << "Could not create file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.cpp" ) << " for Write!" << std::endl;
}
