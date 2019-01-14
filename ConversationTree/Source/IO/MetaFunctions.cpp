#include <fstream>
#include <iostream>
#include <vector>

#include "Strings.h"

#include "PlayerWindow.h"
#include "NPCWindow.h"

static const auto & TrunkID = ConversationTreeWindow_Base::TrunkID;

static std::string FileOutput;

static size_t IDCount;

static void RunMetaNPCFunction( ConversationTreeWindow_NPCDoubleConstPtr CurrentNode ) noexcept;

static void RunMetaPlayerFunction( ConversationTreeWindow_PlayerDoubleConstPtr CurrentNode ) noexcept
{
  auto Child = CurrentNode->GetChild();

  if( Child )
  {
    FileOutput << "  auto ID" << IDCount << " = Tree.AddText( \"" << CurrentNode->GetText() << "\", \"";

    RunMetaNPCFunction( Child );
  }
  else
  {
    FileOutput << "  Tree.AddText( \"" << CurrentNode->GetText() << "\", " << CurrentNode->GetResponseType() << " );\n";
  }
}

static void RunMetaPlayerFunction( ConversationTreeWindow_PlayerDoubleConstPtr CurrentNode, size_t ID ) noexcept
{
  auto Child = CurrentNode->GetChild();

  if( Child )
  {
    FileOutput << "  auto ID" << IDCount << " = Tree.AddText( \"" << CurrentNode->GetText() << "\", \"";

    RunMetaNPCFunction( Child );
  }
  else
  {
    FileOutput << "  Tree.AddText( \"" << CurrentNode->GetText() << "\", " << CurrentNode->GetResponseType() << ", ID" << ID <<" );\n";
  }
}

static void RunMetaNPCFunction( ConversationTreeWindow_NPCDoubleConstPtr CurrentNode ) noexcept
{
  FileOutput << CurrentNode->GetNPCText() << "\"";

  if( IDCount )
  {
    FileOutput << ", ID" << IDCount - 1;
  }

  ++IDCount;

  FileOutput << " );\n";

  for( auto Iter : CurrentNode->GetChildren() )
  {
    RunMetaPlayerFunction( Iter.second, IDCount - 1 );
  }
}

static bool RunMetaNPCFunction_Trunk( ConversationTreeWindow_NPCDoubleConstPtr CurrentNode ) noexcept
{
  const char *Name = CurrentNode->GetTreeName();

  if( !( Name[ 0 ] ) )
  {
    std::cout << "No name for the Conversation Tree!" << std::endl;

    return false;
  }

  FileOutput << "#include <ConversationTree.h>\n";

  FileOutput << "ConversationTree " << Name << "() noexcept\n";
  FileOutput << "{\n";
  FileOutput << "  auto Tree = ConversationTreeCreator( \"" << CurrentNode->GetNPCText() << "\" );\n";

  for( auto Iter : CurrentNode->GetChildren() )
  {
    RunMetaPlayerFunction( Iter.second );
  }

  return true;
}

void CreateConversation( const char *FileName ) noexcept
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

    IDCount = 0;

    if( !RunMetaNPCFunction_Trunk( 
          static_cast< ConversationTreeWindow_NPCDoubleConstPtr >( ConversationTreeWindow_Base::GetAllWindows().find( 0 )->second ) 
        ) 
      )
    {
      return;
    }

    FileOutput << "  return ConversationTree{ std::move( Tree ) };\n";

    FileOutput << "}\n";

    size_t Offset = 0;
    for( ; ; )
    {
      Offset = FileOutput.find( '"', Offset );

      if( Offset == FileOutput.npos )
      {
        break;
      }

      const size_t TempOffset1 = FileOutput.find( '\n', Offset );

      const size_t TempOffset2 = FileOutput.find( '"', Offset + 1 );

      if( TempOffset1 < TempOffset2 )
      {
        FileOutput.insert( TempOffset1, "\\n" );

        FileOutput.erase( TempOffset1 + 2, 1 );
      }

      Offset = TempOffset2 + 2;
    }

    file << FileOutput;

    file.flush();

    std::cout << FileOutput << std::endl;

    std::cout << "Successfully created file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.cpp" ) << "!" << std::endl;

    return;
  }

  std::cout << "Could not create file: " << ( !FullFileName.empty() ? FullFileName : "TestFile.cpp" ) << " for Write!" << std::endl;
}
