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

static std::string InputHolder;
static size_t StartIndex, EndIndex;

const std::string & FindChar( char Character, const std::string &Input = InputHolder, size_t &Start = StartIndex,
                              size_t &End = EndIndex )
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
    LoadBase( KarmaTreeWindow_BasePtr Base ) noexcept { LoadNode( Base ); }

  private:
    static void LoadNode( KarmaTreeWindow_BasePtr Base ) noexcept
    {
      Base->SetLocation( { std::stof( FindChar( ',' ) ), std::stof( FindChar( ',' ) ) } );
    }
};

void LoadTree( const char* FileName ) noexcept
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

    auto AllWindows = KarmaTreeWindow_Base::EditAllWindows();

    auto begin = AllWindows->begin();

    ++begin; // dont want to delete the trunk!

    for( auto end = AllWindows->end(); begin != end; )
    {
      AllWindows->erase( begin++ ); // use post increment since begin will be erased
    }

    StartIndex = InputHolder.find_first_of( ',', EndIndex ) + 1;

    //LoadTrunk Trunk( static_cast< KarmaTreeWindow_NPCPtr >( AllWindows->at( 0 ) ) );

    while( StartIndex < InputHolder.size() )
    {
      switch( FindChar( ',' )[ 0 ] )
      {
        case 'P':
        {
          /*LoadLevel2 Branch( AddBranch( std::stoul( FindChar( ',' ) ) ), true );*/

          break;
        }

        case 'N':
        {
          /*LoadBase Null( AddNull( std::stoul( FindChar( ',' ) ) ), true );*/

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
