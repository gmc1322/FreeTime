/*!----------------------------------------------------------------------------
\file   Strings.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <sstream>
#include <iomanip>

#include "Strings.h"

std::string & operator<<( std::string &String, const char *Text ) noexcept
{
  String.append( Text );

  return String;
}

std::string & operator<<( std::string &String, std::string &Text ) noexcept
{
  String.append( Text );

  return String;
}

std::string & operator<<( std::string &String, size_t Number ) noexcept
{
  String.append( std::to_string( Number ) );

  return String;
}

std::string & operator<<( std::string &String, float Number ) noexcept
{
  std::ostringstream Temp;

  Temp << std::setprecision( 6 ) << Number;

  String << Temp.str().c_str();

  return String;
}

std::string & operator<<( std::string &String, const ImVec2 &Vec ) noexcept
{
  String << Vec.x << "," << Vec.y;

  return String;
}

std::string & operator<<( std::string &String, KarmaTreeWindow_Base::WindowTypes WindowType) noexcept
{
  switch( WindowType )
  {
    case KarmaTreeWindow_Base::Player:
      String << "P";
      break;

    case KarmaTreeWindow_Base::NPC: 
      String << "N";
      break;

    default:
      String << "!";
  }

  return String;
}
