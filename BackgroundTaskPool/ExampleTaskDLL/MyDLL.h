/*!----------------------------------------------------------------------------
\file   MyDLL.h

\author Garrett Conti

\brief  Project: BackgroundTaskPool
        Course:  FUN
---------------------------------------------------------------------------- */

#pragma once

#ifdef MyDLL_EXPORTS
  #define MyDLL_API __declspec( dllexport )   
#else
  #ifdef MyDLL_API_DEBUG
    #define MyDLL_API
  #else
    #define MyDLL_API __declspec( dllimport )   
  #endif
#endif

#include <fstream>

void MyDLL_API SharedMemFunc( void *Args, void *Result ) noexcept;

void MyDLL_API FileReadWriteFunc( std::ifstream &InFile, std::ofstream &OutFile ) noexcept;
