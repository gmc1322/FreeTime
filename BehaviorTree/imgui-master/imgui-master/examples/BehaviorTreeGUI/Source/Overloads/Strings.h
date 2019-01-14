/*!------------------------------------------------------------------------------
\file   Strings.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

std::string & operator<<( std::string &String, const char *Text ) noexcept;
std::string & operator<<( std::string &String, std::string &Text ) noexcept;

std::string & operator<<( std::string &String, size_t Number ) noexcept;
std::string & operator<<( std::string &String, float Number ) noexcept;

std::string & operator<<( std::string &String, const ImVec2 &Vec ) noexcept;

std::string & operator<<( std::string &String, BehaviorTreeWindow_Base::WindowTypes WindowType ) noexcept;
