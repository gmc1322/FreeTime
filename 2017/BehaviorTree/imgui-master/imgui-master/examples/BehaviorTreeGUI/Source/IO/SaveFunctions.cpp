/*!----------------------------------------------------------------------------
\file   SaveFunctions.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <fstream>
#include <iostream>

#include "Strings.h"

#include "SaveFunctions.h"

#include "NullWindow.h"
#include "TreeWindow.h"
#include "TrunkWindow.h"
#include "BranchWindow.h"
#include "MultiWindow.h"
#include "CheckWindow.h"

#include "MyMultiLevelQue.h"
#include "ChildWindow.h"

static std::string FileOutput;

static void SaveInput( InputDoubleConstPtr InputData ) noexcept
{
  FileOutput << "GDF" << "," << InputData->GetInput() << "\n";
}

class BaseWindowSaver
{
  public:
    BaseWindowSaver( BehaviorTreeWindow_BaseDoubleConstPtr BaseWindow ) { SaveNode( BaseWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_BaseDoubleConstPtr BaseWindow ) noexcept
    {
      FileOutput << BaseWindow->GetWindowType() << "," << BaseWindow->GetID() << "," << BaseWindow->GetLocation() << ",";

      auto AllWindows = BehaviorTreeWindow_Base::GetAllWindows();

      auto Temp = BaseWindow->GetOtherParents();

      if( !Temp.empty() )
      {
        for( auto iterOuter : Temp )
        {
          BehaviorTreeWindow_BasePtr ParentWindow = nullptr;

          for( auto iterInner : AllWindows )
          {
            if( &iterInner.second->GetLocation() == iterOuter.second.first )
            {
              ParentWindow = iterInner.second;
            }
          }

          FileOutput << ParentWindow->GetID() << ",";
        }

        FileOutput << "}";
      }
    }
};

class NullWindowSaver
{
  public:
    NullWindowSaver( BehaviorTreeWindow_NullDoubleConstPtr NullWindow ) { SaveNode( NullWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_NullDoubleConstPtr NullWindow ) noexcept
    {
      BaseWindowSaver BaseSaver( NullWindow );

      FileOutput << "Null Node\n";
    }
};

class MultiWindowSaver
{
  public:
    MultiWindowSaver( BehaviorTreeWindow_MultiDoubleConstPtr MultiWindow ) { SaveNode( MultiWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_MultiDoubleConstPtr MultiWindow ) noexcept
    {
      BaseWindowSaver BaseSaver( MultiWindow );

      auto IsGen = MultiWindow->GetIsGenerating();

      FileOutput << MultiWindow->GetNodeName() << "," << ( IsGen ? "True" : "False" ) << ",";

      if( IsGen )
      {
        FileOutput << MultiWindow->GetGenFunction() << "," << MultiWindow->GetGenCount() << "," << MultiWindow->GetRunFunction();

        FileOutput << "," << MultiWindow->GetGenType();
      }

      FileOutput << "\n";
    }
};

class Level1WindowSaver
{
  public:
    Level1WindowSaver( BehaviorTreeWindow_Level1DoubleConstPtr Level1Window ) { SaveNode( Level1Window ); }

  private:
    static void SaveNode( BehaviorTreeWindow_Level1DoubleConstPtr Level1Window ) noexcept
    {
      BaseWindowSaver BaseSaver( Level1Window );

      FileOutput << Level1Window->GetNodeName() << "," << Level1Window->GetFunctionName();
    }
};

class TreeWindowSaver
{
  public:
    TreeWindowSaver( BehaviorTreeWindow_TreeDoubleConstPtr TreeWindow ) { SaveNode( TreeWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_TreeDoubleConstPtr TreeWindow ) noexcept
    {
      Level1WindowSaver Level1Saver( TreeWindow );

      FileOutput <<  "\n";
    }
};

class Level2WindowSaver
{
  public:
    Level2WindowSaver( BehaviorTreeWindow_Level2DoubleConstPtr Level2Window ) { SaveNode( Level2Window ); }

  private:
    static void SaveNode( BehaviorTreeWindow_Level2DoubleConstPtr Level2Window ) noexcept
    {
      Level1WindowSaver Level1Saver( Level2Window );

      FileOutput << "," << Level2Window->GetDataKey();
    }
};

class BranchWindowSaver
{
  public:
    BranchWindowSaver( BehaviorTreeWindow_BranchDoubleConstPtr BranchWindow ) { SaveNode( BranchWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_BranchDoubleConstPtr BranchWindow ) noexcept
    {
      Level2WindowSaver Level2Saver( BranchWindow );

      FileOutput << "\n";
    }
};

class ChildWindowSaver
{
  public:
  ChildWindowSaver( BehaviorTreeWindow_ChildDoubleConstPtr ChildWindow ) { SaveNode( ChildWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_ChildDoubleConstPtr ChildWindow ) noexcept
    {
      Level1WindowSaver Level1Saver( ChildWindow );

      FileOutput << "\n";
    }
};

class CheckWindowSaver
{
  public:
    CheckWindowSaver( BehaviorTreeWindow_CheckDoubleConstPtr CheckWindow ) { SaveNode( CheckWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_CheckDoubleConstPtr CheckWindow ) noexcept
    {
      Level2WindowSaver Level2Saver( CheckWindow );

      FileOutput << "\n";
    }
};

class TrunkWindowSaver
{
  public:
    TrunkWindowSaver( BehaviorTreeWindow_TrunkDoubleConstPtr TrunkWindow ) { SaveNode( TrunkWindow ); }

  private:
    static void SaveNode( BehaviorTreeWindow_TrunkDoubleConstPtr TrunkWindow ) noexcept
    {
      Level2WindowSaver Level2Saver( TrunkWindow );

      FileOutput << "," << TrunkWindow->GetOnFinishName() << "," << TrunkWindow->GetOnResetName() << "\n";
    }
};

void SaveTree( const char* FileName, const std::map< size_t, InputPtr > &InputData ) noexcept
{
  std::ofstream file;
  std::string FullFileName;

  if( FileName[ 0 ] )
  {
    FullFileName << FileName << ".GDat";

    file.open( FullFileName );
  }
  else
  {
    file.open( "TestFile.GDat" );
  }

  system( "CLS" );

  if( file.is_open() )
  {
    FileOutput.clear();

    for( auto iter : InputData )
    {
      SaveInput( iter.second );
    }

    MutliLevelQue MyMultiLevelQue;

    auto AllWindows = BehaviorTreeWindow_Base::GetAllWindows();

    for( auto iter : AllWindows )
    {
      MyMultiLevelQue.AddToQue( iter.second->GetLocation(), iter.second );
    }

    std::map < size_t, BehaviorTreeWindow_BaseDoubleConstPtr > Windows;

    MyMultiLevelQue.FillMap( Windows );

    for( auto iter : Windows )
    {
      switch( iter.second->GetWindowType() )
      {
        case BehaviorTreeWindow_Base::Branch:
          BranchWindowSaver( static_cast< BehaviorTreeWindow_BranchDoubleConstPtr >( iter.second ) );
          break;

        case BehaviorTreeWindow_Base::Null: 
          NullWindowSaver( static_cast< BehaviorTreeWindow_NullDoubleConstPtr >( iter.second ) );
          break;

        case BehaviorTreeWindow_Base::Tree: 
          TreeWindowSaver( static_cast< BehaviorTreeWindow_TreeDoubleConstPtr >( iter.second ) );
          break;

        case BehaviorTreeWindow_Base::Trunk: 
          TrunkWindowSaver( static_cast< BehaviorTreeWindow_TrunkDoubleConstPtr >( iter.second ) );
          break;

        case BehaviorTreeWindow_Base::Multi:
          MultiWindowSaver( static_cast< BehaviorTreeWindow_MultiDoubleConstPtr >( iter.second ) );
          break;

        case BehaviorTreeWindow_Base::Check:
          CheckWindowSaver( static_cast< BehaviorTreeWindow_CheckDoubleConstPtr >( iter.second ) );
          break;
        
        case BehaviorTreeWindow_Base::Child:
          ChildWindowSaver( static_cast< BehaviorTreeWindow_ChildDoubleConstPtr >( iter.second ) );
          break;
      }
    }

    file << FileOutput;

    file.flush();

    std::cout << "Successfully saved file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.GDat" ) << "!" << std::endl;

    return;
  }

  std::cout << "Could not open file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.GDat" ) << " for Write!" << std::endl;
}
