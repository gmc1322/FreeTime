#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#include "imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"

#include "Conversation/ConversationDisplay.h"
#include "Conversation/KarmaConversationDisplay.h"

using namespace KarmaTreeSpace;

ImFont *HelpFont;

static void error_callback(int error, const char* description)
{
  fprintf( stderr, "Error %d: %s\n", error, description );
}

ConversationTree MakeTree()
{
  ConversationTreeCreator This{ "Hello!\nHow are you?" };

  auto ID0 = This.AddText( "Great actually! How are you!", "Quite well thank you!" );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID0 );

  auto ID1 = This.AddText( "Good", "Im glad your having a good day!" );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID1 );

  auto ID2 = This.AddText( "Ok", "Well I hope your day gets better!" );
  auto ID2_1 = This.AddText( "Why thank you for your consideration!", "Any time!", ID2 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2_1 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2 );

  This.AddText( "Bad" );
  This.AddText( "Get outa my sight!", DefaultResponseOptions::CustomOption + 1 );

  return ConversationTree{ std::move( This ) };
}

KarmaTree MakeKarmaTree()
{
  KarmaTreeCreator This{ "Hello!\nHow are you?" };

  auto ID0 = This.AddText( "Great actually! How are you!", "Quite well thank you!", KarmaTreeCreator::DefaultID, 50, 50 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID0 );

  auto ID1 = This.AddText( "Good", "Im glad your having a good day!", KarmaTreeCreator::DefaultID, 25, 25 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID1 );

  auto ID2 = This.AddText( "Ok", "Well I hope your day gets better!" );
  auto ID2_1 = This.AddText( "Why thank you for your consideration!", "Any time!", ID2, 15 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2_1 );
  This.AddText( "End Conversation", DefaultResponseOptions::EndConversation, ID2 );

  This.AddText( "Bad", KarmaTreeCreator::EndConversation, KarmaTreeCreator::DefaultID, -25, -25 );
  This.AddText( "Get outa my sight!", DefaultResponseOptions::CustomOption + 1, KarmaTreeCreator::DefaultID, -50, -50 );

  return KarmaTree{ std::move( This ) };
}

int main()
{
  // Setup window
  glfwSetErrorCallback( error_callback );

  if( !glfwInit() )
  {
    return 1;
  }

  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
  glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

  auto MainWindow = glfwCreateWindow( 1280, 720, "Conversation Display", nullptr, nullptr );

  glfwMakeContextCurrent( MainWindow );

  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init( MainWindow, true );

  const ImVec4 clear_color = ImColor( 114, 144, 154 );

  /*auto io = ImGui::GetIO();
  io.Fonts->AddFontDefault();
  io.Fonts->AddFontFromFileTTF( "DroidSans.ttf", 24 );
  HelpFont = io.Fonts->Fonts.back();*/

  //const ConversationDisplay Display{ MakeTree(), 100, 100 };

  auto Tree = MakeKarmaTree();

  //Tree.SetKarmaKey( 0, 1 );

  const KarmaConversationDisplay Display{ std::move( Tree ), -50, Tree.GetBadKarmaThreshold(), Tree.GetGoodKarmaThreshold(), 100, 100 };

  // Main loop
  while( !glfwWindowShouldClose( MainWindow ) )
  {
    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();

    int Width, Hight;
    glfwGetWindowSize( MainWindow, &Width, &Hight );

    const auto Return = Display.Draw( Width, Hight );
    if( Return.first > -1 )
    {
      // Handle Conversation End 
      Return.second; // Karma Change
    }

    //ImGui::ShowTestWindow();

    // Rendering
    int display_w, display_h;
    glfwGetFramebufferSize( MainWindow, &display_w, &display_h );
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();
    glfwSwapBuffers( MainWindow );
  }

  // Cleanup
  ImGui_ImplGlfwGL3_Shutdown();
  glfwTerminate();

  return 0;
}
