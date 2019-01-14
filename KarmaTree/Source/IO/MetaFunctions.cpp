#include <fstream>
#include <iostream>
#include <vector>

#include "Strings.h"

#include "PlayerWindow.h"
#include "NPCWindow.h"
#include "TrunkWindow.h"

static std::string FileOutput;

static size_t IDCount;

static void RunMetaPlayerFunction( KarmaTreeWindow_PlayerDoubleConstPtr CurrentNode, size_t ID ) noexcept
{
  if( auto Child = CurrentNode->GetChild() )
  {
    const size_t ChildID = IDCount++;

    FileOutput << "  auto ID" << ChildID << " = Tree.AddText( \"" << CurrentNode->GetText() << "\", \"" << Child->GetNPCText() << "\", ID";
    FileOutput << ID << ", " << CurrentNode->GetKarma() << ", { " << CurrentNode->GetKeys() << " } );\n";

    for( auto Iter : Child->GetChildren() )
    {
      RunMetaPlayerFunction( Iter.second, ChildID );
    }
  }
  else
  {
    FileOutput << "  Tree.AddText( \"" << CurrentNode->GetText() << "\", " << CurrentNode->GetResponseType() << ", ID" << ID << ", ";
    FileOutput << CurrentNode->GetKarma() << ", { " << CurrentNode->GetKeys() << " } );\n";
  }
}

static void RunMetaPlayerFunction( KarmaTreeWindow_PlayerDoubleConstPtr CurrentNode ) noexcept
{
  if( auto Child = CurrentNode->GetChild() )
  {
    const size_t ID = IDCount++;

    FileOutput << "  auto ID" << ID << " = Tree.AddText( \"" << CurrentNode->GetText() << "\", \"" << Child->GetNPCText();
    FileOutput << "\", KarmaTreeCreator::DefaultID, " << CurrentNode->GetKarma() << ", { " << CurrentNode->GetKeys() << " } );\n";

    for( auto Iter : Child->GetChildren() )
    {
      RunMetaPlayerFunction( Iter.second, ID );
    }
  }
  else
  {
    FileOutput << "  Tree.AddText( \"" << CurrentNode->GetText() << "\", " << CurrentNode->GetResponseType();
    FileOutput << ", KarmaTreeCreator::DefaultID, " << CurrentNode->GetKarma() << ", { " << CurrentNode->GetKeys() << " } );\n";
  }
}

static bool RunMetaNPCFunction_Trunk( KarmaTreeWindow_TrunkDoubleConstPtr CurrentNode ) noexcept
{
  const char *Name = CurrentNode->GetTreeName();

  if( !( Name[ 0 ] ) )
  {
    std::cout << "No name for the Karma Tree!" << std::endl;

    return false;
  }

  FileOutput << "#include \"KarmaTree.h\"\n";

  FileOutput << "KarmaTree " << Name << "() noexcept\n";
  FileOutput << "{\n";
  FileOutput << "  auto Tree = KarmaTreeCreator{ \"" << CurrentNode->GetNPCText() << "\", " << CurrentNode->GetBadThreshold() << ", ";
  FileOutput << CurrentNode->GetGoodThreshold() << " };\n";

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

    if( !RunMetaNPCFunction_Trunk( static_cast< KarmaTreeWindow_TrunkDoubleConstPtr >( 
                                     KarmaTreeWindow_Base::GetAllWindows().begin()->second ) ) )
    {
      return;
    }

    FileOutput << "  return KarmaTree{ std::move( Tree ) };\n";

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
