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

#include "PlayerWindow.h"
#include "NPCWindow.h"

static std::string FileOutput;

class BaseWindowSaver
{
  public:
    BaseWindowSaver( ConversationTreeWindow_BaseDoubleConstPtr BaseWindow ) { SaveNode( BaseWindow ); }

  private:
    static void SaveNode( ConversationTreeWindow_BaseDoubleConstPtr BaseWindow ) noexcept
    {
      FileOutput << BaseWindow->GetWindowType() << "," << BaseWindow->GetID() << "," << BaseWindow->GetLocation() << ",";

      auto AllWindows = ConversationTreeWindow_Base::GetAllWindows();
    }
};

class NPCWindowSaver
{
  public:
  NPCWindowSaver( ConversationTreeWindow_NPCDoubleConstPtr NPCWindow ) { SaveNode( NPCWindow ); }

  private:
  static void SaveNode( ConversationTreeWindow_NPCDoubleConstPtr NPCWindow ) noexcept
  {
    BaseWindowSaver BaseSaver( NPCWindow );
  }
};

class PlayerWindowSaver
{
  public:
    PlayerWindowSaver( ConversationTreeWindow_PlayerDoubleConstPtr PlayerWindow ) { SaveNode( PlayerWindow ); }

  private:
    static void SaveNode( ConversationTreeWindow_PlayerDoubleConstPtr PlayerWindow ) noexcept
    {
      BaseWindowSaver BaseSaver( PlayerWindow );

      FileOutput << "\n";
    }
};

void SaveTree( const char* FileName ) noexcept
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

   

    file << FileOutput;

    file.flush();

    std::cout << "Successfully saved file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.GDat" ) << "!" << std::endl;

    return;
  }

  std::cout << "Could not open file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.GDat" ) << " for Write!" << std::endl;
}
