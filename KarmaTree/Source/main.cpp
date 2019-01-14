#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <cstdio>

#include "GUIFunctions.h"

#include "imgui.h"
#include "ImGui/imgui_impl_glfw_gl3.h"
#include "TrunkWindow.h"

int X, Y;

ImFont *HelpFont;

static void error_callback(int error, const char* description)
{
  fprintf( stderr, "Error %d: %s\n", error, description );
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

  auto MainWindow = glfwCreateWindow( 1280, 720, "Karma Tree GUI", nullptr, nullptr );

  glfwMakeContextCurrent( MainWindow );

  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplGlfwGL3_Init( MainWindow, true );

  ImVec4 clear_color = ImColor( 114, 144, 154 );

  auto io = ImGui::GetIO();
  io.Fonts->AddFontDefault();
  io.Fonts->AddFontFromFileTTF( "DroidSans.ttf", 24 );
  HelpFont = io.Fonts->Fonts.back();

  // Main loop
  while( !glfwWindowShouldClose( MainWindow ) )
  {
    glfwPollEvents();
    ImGui_ImplGlfwGL3_NewFrame();

    glfwGetWindowSize( MainWindow, &X, &Y );

    RunKarmaTreeCanvas();

    RunKarmaTreeNodeWindow();

    RunHelpWindow();

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
