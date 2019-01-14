#include "GUIFunctions.h"

#include "MetaFunctions.h"
#include "LoadFunctions.h"
#include "SaveFunctions.h"

#include "TrunkWindow.h"

#define unreferenced( p ) ( p )

static const float Padding = 10;

static const ImVec2 ZeroVec( 0.0f, 0.0f );
static const ImVec2 ClosedHelpSize( 200, 35 );

static bool isHelpClosed;
static auto isDescripClosed = true;

BehaviorTreeWindow_BranchPtr AddBranch( size_t ID ) noexcept
{
  return new BehaviorTreeWindow_Branch( ID );
}

BehaviorTreeWindow_TreePtr AddTree( size_t ID ) noexcept
{
  return new BehaviorTreeWindow_Tree( ID );
}

BehaviorTreeWindow_NullPtr AddNull( size_t ID ) noexcept
{
  return new BehaviorTreeWindow_Null( ID );
}

BehaviorTreeWindow_MultiPtr AddMulti( size_t ID ) noexcept
{
  return new BehaviorTreeWindow_Multi( ID );
}

BehaviorTreeWindow_CheckPtr AddCheck( size_t ID ) noexcept
{
  return new BehaviorTreeWindow_Check( ID );
}

BehaviorTreeWindow_ChildPtr AddChild( size_t ID ) noexcept
{
  return new BehaviorTreeWindow_Child( ID );
}

InputPtr AddData( void *Data ) noexcept
{
  auto InputMap = static_cast< std::map< size_t, InputPtr > * >( Data );

  auto Size = InputMap->size();

  auto Return = new Input( "Data Key ID: " + std::to_string( Size ) );

  InputMap->emplace( std::make_pair( Size, Return ) );

  return Return;
}

static void AddBranch_NoReturn( void *Data ) noexcept
{
  unreferenced( Data );

  AddBranch();
}

static void AddTree_NoReturn( void *Data ) noexcept
{
  unreferenced( Data );

  AddTree();
}

static void AddNull_NoReturn( void *Data ) noexcept
{
  unreferenced( Data );

  AddNull();
}

static void AddMulti_NoReturn( void *Data ) noexcept
{
  unreferenced( Data );

  AddMulti();
}

static void AddCheck_NoReturn( void *Data ) noexcept
{
  unreferenced( Data );

  AddCheck();
}

static void AddChild_NoReturn( void *Data ) noexcept
{
  unreferenced( Data );

  AddChild();
}

static void AddData_NoReturn( void *Data ) noexcept
{
  AddData( Data );
}

void RemoveData( void *Data ) noexcept
{
  auto InputMap = static_cast< std::map< size_t, InputPtr > * >( Data );

  if( InputMap->size() > 1 )
  {
    auto NewEnd = InputMap->end();

    --NewEnd;

    InputMap->erase( NewEnd );
  }
}

void RunBehaviorTreeCanvas() noexcept
{
  static BehaviorTreeWindow_Trunk Trunk{};

  ImGui::SetNextWindowSize( ImVec2( X - 450.0f / 2 + Padding * 2.5f, static_cast< float >( Y ) ) );

  ImGui::SetNextWindowPos( ZeroVec );

  ImGui::Begin( "Behavior Tree Canvas", nullptr, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                                   ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove |
                                                   ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings ) );

  BehaviorTreeWindow_Base::RunAllWindows();

  ImGui::End();
}

void RunBehaviorTreeNodeWindow() noexcept
{
  static Button CreateBehaviorTreeCode( ImColor( 1.0f, 0.0f, 0.0f ), "Create Behavior Tree Code" );
  static Button SaveBehaviorTreeFile( ImColor( 0.0f, 1.0f, 0.0f ), "Save Behavior Tree File" );
  static Button LoadBehaviorTreeFile( ImColor( 1.0f, 0.0f, 0.0f ), "Load Behavior Tree File" );
  static Input BehaviorTreeFileName( "Behavior Tree File Name" );

  static Button AddBranchButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add Branch Node", AddBranch_NoReturn );
  static Button AddTreeButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add Tree Node", AddTree_NoReturn );
  static Button AddNullButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add Null Node", AddNull_NoReturn );
  static Button AddMultiButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add Multi Node", AddMulti_NoReturn );
  static Button AddCheckButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add Check Node", AddCheck_NoReturn );
  static Button AddChildButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add ChildTree Node", AddChild_NoReturn );

  static Button AddDataButton( ImColor( 0.0f, 1.0f, 0.0f ), "Add Get Data Function", AddData_NoReturn );
  static Button RemoveDataButton( ImColor( 1.0f, 0.0f, 0.0f ), "Remove Data Function", RemoveData );

  static std::map< size_t, InputPtr > InputData( { std::make_pair( 0, new Input( "Data Key ID: 0" ) ) } );

  ImGui::SetNextWindowContentSize( ImVec2( 325, 0 ) );

  ImGui::SetNextWindowSize( ImVec2( 200, Y - ( isHelpClosed && isDescripClosed ? ClosedHelpSize.y : 0 ) ) );

  ImGui::SetNextWindowPos( ImVec2( X - 200.0f, 0 ) );

  if( !BehaviorTreeWindow_Base::GetIsFocused() )
  {
    ImGui::SetNextWindowFocus();
  }

  ImGui::Begin( "Behavior Tree Node Window", nullptr, ZeroVec, 1.0f, ( ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | 
                                                                       ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar |
                                                                       ImGuiWindowFlags_NoSavedSettings ) );

  // These are variables to keep the buttons rendered when in the IF statements!
  auto CreatedTree = CreateBehaviorTreeCode.RunButton();
  auto SavedTree = SaveBehaviorTreeFile.RunButton();
  auto LoadedTree = LoadBehaviorTreeFile.RunButton();

  if( CreatedTree )
  {
    CreateTree( BehaviorTreeFileName.GetInput(), InputData );
  }
  else if( SavedTree )
  {
    SaveTree( BehaviorTreeFileName.GetInput(), InputData );
  }
  else if( LoadedTree )
  {
    LoadTree( BehaviorTreeFileName.GetInput(), &InputData );
  }

  BehaviorTreeFileName.RunInput();

  ImGui::NewLine();

  AddBranchButton.RunButton();

  AddTreeButton.RunButton();

  AddNullButton.RunButton();

  AddMultiButton.RunButton();

  AddCheckButton.RunButton();

  AddChildButton.RunButton();

  ImGui::NewLine();

  AddDataButton.RunButton( &InputData );

  RemoveDataButton.RunButton( &InputData );

  for( auto iter : InputData )
  {
    iter.second->RunInput();
  }

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

  if( !BehaviorTreeWindow_Base::GetIsFocused() )
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
