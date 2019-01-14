/*!----------------------------------------------------------------------------
\file   LoadFunctions.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <sstream>
#include <fstream>
#include <iostream>

#include "Strings.h"

#include "LoadFunctions.h"

#include "GUIFunctions.h"

#include "TrunkWindow.h"
#include "MultiWindow.h"

static std::string InputHolder;
static size_t StartIndex, EndIndex;

const std::string & FindChar( char Character, const std::string &Input = InputHolder, 
                              size_t &Start = StartIndex, size_t &End = EndIndex )
{
  static std::string TempString;

  End = Input.find_first_of( Character, Start );

  TempString = Input.substr( Start, End - Start );

  Start = End + 1;

  TempString.push_back( '\0' );

  return TempString;
}

class LoadBase
{
  public:
    LoadBase( BehaviorTreeWindow_BasePtr Base, bool Last = false ) noexcept { LoadNode( Base, Last ); }

  private:
    static void LoadNode( BehaviorTreeWindow_BasePtr Base, bool Last ) noexcept
    {
      Base->SetLocation( { std::stof( FindChar( ',' ) ), std::stof( FindChar( ',' ) ) } );
      
      auto Start1 = StartIndex;
      auto Start2 = StartIndex;

      FindChar( '}' );

      auto End = EndIndex;

      FindChar( '\n', InputHolder, Start2 );

      if( EndIndex > End )
      {
        for( size_t end; Start1 != End; )
        {
          Base->AddParent( std::stoul( FindChar( ',', InputHolder, Start1, end ) ) );
        }
      }
      else
      {
        StartIndex = Start1;
      }

      Base->SetNodeName( Last ? FindChar( '\n' ) : FindChar( ',' ) );
    }
};

class LoadMulti
{
  public:
    LoadMulti( BehaviorTreeWindow_MultiPtr Multi ) noexcept { LoadNode( Multi ); }

  private:
    static void LoadNode( BehaviorTreeWindow_MultiPtr Multi ) noexcept
    {
      LoadBase BaseLoader( Multi );

      if( !strcmp( FindChar( ',' ).c_str(), "True" ) )
      {
        Multi->SetGenerating();

        Multi->SetGenFunction( FindChar( ',' ) );

        Multi->SetGenCount( FindChar( ',' ) );

        Multi->SetRunFunction( FindChar( ',' ) );

        Multi->SetGenType( FindChar( '\n' ) );
      }
      else
      {
        FindChar( '\n' );
      }
    }
};

class LoadLevel1
{
  public:
    LoadLevel1( BehaviorTreeWindow_Level1Ptr Level1, bool Last = false ) noexcept { LoadNode( Level1, Last ); }

  private:
    static void LoadNode( BehaviorTreeWindow_Level1Ptr Level1, bool Last ) noexcept
    {
      LoadBase BaseLoader( Level1 );

      Level1->SetFunctionInput( Last ? FindChar( '\n' ) : FindChar( ',' ) );
    }
};

class LoadLevel2
{
  public:
    LoadLevel2( BehaviorTreeWindow_Level2Ptr Level2, bool Last = false ) noexcept { LoadNode( Level2, Last ); }

  private:
    static void LoadNode( BehaviorTreeWindow_Level2Ptr Level2, bool Last ) noexcept
    {
      LoadLevel1 LVL1Loader( Level2 );

      Level2->SetDataKey( Last ? FindChar( '\n' ) : FindChar( ',' ) );
    }
};

class LoadTrunk
{
  public:
    LoadTrunk( BehaviorTreeWindow_TrunkPtr Trunk ) noexcept { LoadNode( Trunk ); }

  private:
    static void LoadNode( BehaviorTreeWindow_TrunkPtr Trunk ) noexcept
    {
      FindChar( ',' );

      LoadLevel2 LVL2Loader( Trunk );

      Trunk->SetOnFinish( FindChar( ',' ) );

      Trunk->SetOnReset( FindChar( '\n' ) );
    }
};

static void LoadData( InputPtr InputData ) noexcept
{
  StartIndex = InputHolder.find_first_of( ',', EndIndex ) + 1;

  if( InputHolder.at( StartIndex ) != '\n' )
  {
    InputData->SetInput( FindChar( '\n' ) );
  }
  else
  {
    EndIndex = StartIndex + 1;
  }
}

void LoadTree( const char* FileName, std::map< size_t, InputPtr > *InputData ) noexcept
{
  std::ifstream file;
  std::string FullFileName;

  if( FileName[ 0 ] )
  {
    FullFileName << FileName << ".GDat";

    file.open( FullFileName, std::ios_base::in );
  }
  else
  {
    file.open( "TestFile.GDat", std::ios_base::in );
  }

  system( "CLS" );

  if( file.is_open() )
  {
    InputHolder.clear();

    StartIndex = EndIndex = 0;

    std::stringstream InputTemp;

    InputTemp << file.rdbuf();

    InputHolder = InputTemp.str();

    LoadData( InputData->begin()->second );

    while( InputData->size() > 1 )
    {
      RemoveData( InputData );
    }

    while( true )
    {
      EndIndex = InputHolder.find( "GDF", StartIndex );

      if( EndIndex == std::string::npos )
      {
        EndIndex = StartIndex;

        break;
      }

      auto Data = AddData( InputData );

      LoadData( Data );
    }

    auto AllWindows = BehaviorTreeWindow_Base::EditAllWindows();

    auto begin = AllWindows->begin();

    ++begin; // dont want to delete the trunk!

    for( auto end = AllWindows->end(); begin != end; )
    {
      AllWindows->erase( begin++ ); // use post increment since begin will be erased
    }

    StartIndex = InputHolder.find_first_of( ',', EndIndex ) + 1;

    LoadTrunk Trunk( static_cast< BehaviorTreeWindow_TrunkPtr >( AllWindows->at( 0 ) ) );

    while( StartIndex < InputHolder.size() )
    {
      switch( FindChar( ',' )[ 0 ] )
      {
        case 'B':
        {
          LoadLevel2 Branch( AddBranch( std::stoul( FindChar( ',' ) ) ), true );

          break;
        }

        case 'N':
        {
          LoadBase Null( AddNull( std::stoul( FindChar( ',' ) ) ), true );

          break;
        }

        case 'T':
        {
          LoadLevel1 Tree( AddTree( std::stoul( FindChar( ',' ) ) ), true );

          break;
        }

        case 'M':
        {
          LoadMulti Multi( AddMulti( std::stoul( FindChar( ',' ) ) ) );

          break;
        }
        
        case 'C':
        {
          LoadLevel2 Check( AddCheck( std::stoul( FindChar( ',' ) ) ), true );

          break;
        }
        
        case 'c':
        {
          LoadLevel1 Child( AddChild( std::stoul( FindChar( ',' ) ) ), true );

          break;
        }

        case '!':
          std::cout << "ERROR! UNKNOWN TYPE!";
          return;
      }
    }

    std::cout << "Successfully read file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.GDat" ) << "!" << std::endl;

    return;
  }

  std::cout << "Could not open file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.GDat" ) << " for Read!" << std::endl;
}
