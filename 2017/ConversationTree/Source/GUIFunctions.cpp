#include "GUIFunctions.h"

#include "Button.h"
#include "imgui.h"
#include "NPCWindow.h"
#include "MetaFunctions.h"

static const float Padding = 10;

static const ImVec2 ZeroVec( 0.0f, 0.0f );
static const ImVec2 ClosedHelpSize( 200, 35 );

static bool isHelpClosed;
static auto isDescripClosed = true;

void RunConversationTreeCanvas() noexcept
{
  static ConversationTreeWindow_NPC NPCTrunk{};

  ImGui::SetNextWindowSize( ImVec2( X - 450.0f / 2 + Padding * 2.5f, static_cast< float >( Y ) ) );

  ImGui::SetNextWindowPos( ZeroVec );

  ImGui::Begin( "Conversation Tree Canvas", nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                                       ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
                                                       ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings ) );

  ConversationTreeWindow_Base::RunAllWindows();

  ImGui::End();
}

void RunConversationTreeNodeWindow() noexcept
{
  static Button CreateConversationTreeCode( ImColor( 1.0f, 0.0f, 0.0f ), "Create Code" );
  static Button SaveConversationTreeFile( ImColor( 0.0f, 1.0f, 0.0f ), "Save Tree" );
  static Button LoadConversationTreeFile( ImColor( 1.0f, 0.0f, 0.0f ), "Load Tree" );
  static Input ConversationTreeFileName( "File Name" );

  ImGui::SetNextWindowContentSize( ImVec2( 325, 0 ) );

  ImGui::SetNextWindowSize( ImVec2( 200, Y - ( isHelpClosed && isDescripClosed ? ClosedHelpSize.y : 0 ) ) );

  ImGui::SetNextWindowPos( ImVec2( X - 200.0f, 0 ) );

  if( !ConversationTreeWindow_Base::GetIsFocused() )
  {
    ImGui::SetNextWindowFocus();
  }

  ImGui::Begin( "Conversation Tree File Window", nullptr, ZeroVec, 1.0f, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                                                                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar |
                                                                           ImGuiWindowFlags_NoSavedSettings ) );

  ConversationTreeFileName.RunInput();

  ImGui::NewLine();

  // These are variables to keep the buttons rendered when in the IF statements!
  auto CreatedTree = CreateConversationTreeCode.RunButton();
  /*auto SavedTree =*/ SaveConversationTreeFile.RunButton();
  /*auto LoadedTree = */LoadConversationTreeFile.RunButton();

  if( CreatedTree )
  {
    CreateConversation( ConversationTreeFileName.GetInput() );
  }/*
  else if( SavedTree )
  {
    SaveTree( ConversationTreeFileName.GetInput() );
  }
  else if( LoadedTree )
  {
    LoadTree( ConversationTreeFileName.GetInput(), nullptr );
  }*/

  ImGui::End();
}

void RunHelpWindow() noexcept
{
  static Button OpenHelpMenu( ImColor( 0.0f, 0.0f, 1.5f ), "Help Menu" );
  static Button CloseHelpMenu( "Close Help Menu" );
  static Button OpenDescripMenu( ImColor( 0.0f, 0.0f, 1.5f ), "Node Descrips" );
  static Button CloseDescripMenu( "Close Node Help" );

  static const ImVec2 Location( Padding, Padding * 2 );

  if( !isHelpClosed || !isDescripClosed )
  {
    ImGui::SetNextWindowSize( ImVec2( X - 200 - Padding * 2, Y - Padding * 3 ) );

    ImGui::SetNextWindowPos( Location );
  }
  else
  {
    ImGui::SetNextWindowSize( ClosedHelpSize );

    ImGui::SetNextWindowPos( ImVec2( X - ClosedHelpSize.x, Y - ClosedHelpSize.y ) );
  }

  if( !ConversationTreeWindow_Base::GetIsFocused() )
  {
    ImGui::SetNextWindowFocus();
  }

  ImGui::Begin( !isDescripClosed ? "Node Descriptions" : "Help Menu", nullptr, ZeroVec, ( isHelpClosed ? 1.0f : -1.0f ), 
                ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
                ( isHelpClosed && isDescripClosed ? 0 : ImGuiWindowFlags_ShowBorders ) | 
                ( isHelpClosed && isDescripClosed ? ImGuiWindowFlags_NoTitleBar : 0 ) );

  if( !isHelpClosed )
  {
    extern ImFont *HelpFont;

    ImGui::PushFont( HelpFont );

    ImGui::TextWrapped( "On the left is the window to add Branches to your tree, add other trees, and add functions to give data to your Tree!" );

    ImGui::NewLine();

    ImGui::TextWrapped( "To delete a node, click on it and press Delete!" );

    ImGui::NewLine();

    ImGui::TextWrapped( "You can move a node around my left clicking it and draging to where you want it!" );
    ImGui::TextWrapped( "You can also right click on the canvas to move all the nodes at once!" );

    ImGui::NewLine();

    ImGui::TextWrapped( "To add more connections to your nodes (shown in green), double left click on the child node then left click once on the new parent node!" );
    ImGui::TextWrapped( "While adding more connections, you can right click at any time to stop adding or if you left click on a child node then right click on a parent, it will remove the green connection!" );

    ImGui::NewLine();

    ImGui::TextWrapped( "When turning your Tree into code, look at the console window for the output information!" );

    ImGui::NewLine();

    ImGui::TextWrapped( "Remember! Nodes are run left to right, top to bottom!" );

    ImGui::NewLine();

    isHelpClosed = CloseHelpMenu.RunButton();

    ImGui::PopFont();
  }
  else if ( !isDescripClosed )
  {
    extern ImFont *HelpFont;

    ImGui::PushFont( HelpFont );

    ImGui::TextWrapped( "The Branch Node can have multiple children and parents. It can only run one child at a time." );

    ImGui::NewLine();

    ImGui::TextWrapped( "The Tree Node will run a new Behavior Tree. It cannot have children, but it can have multiple parents." );

    ImGui::NewLine();

    ImGui::TextWrapped( "The Null Node will do nothing once, then call its parent. It cannot have children, but it can have multiple parents." );

    ImGui::NewLine();

    ImGui::TextWrapped( "The Multi Node can have multiple children and parents. It can run all of its children at the same time. The first child determains when to stop running the children, this cannot be a Generated Child! The Generated children will always be after the non-generated. A Multi Node cannot be the child of another Multi Node! Be carefull, the children are not run in any order!" );

    ImGui::NewLine();

    ImGui::TextWrapped( "The Check Node can have multiple parents but only one child; however, the child can have multiple children! The Check Node will always run its own function before running its child or its child's children! A Check Node cannot be the child of another Check Node!" );

    ImGui::NewLine();

    isDescripClosed = CloseDescripMenu.RunButton();

    ImGui::PopFont();
  }
  else
  {
    isHelpClosed = !OpenHelpMenu.RunButton();

    ImGui::SameLine();

    isDescripClosed = !OpenDescripMenu.RunButton();
  }

  ImGui::End();
}
