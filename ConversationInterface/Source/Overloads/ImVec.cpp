/*!----------------------------------------------------------------------------
\file   ImVec.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ---------------------------------------------------------------------------- */

#include "ImVec.h"

#include "imgui.h"

ImVec2 operator/( const ImVec2 &Vec, const ImVec2 &Div ) noexcept
{
  return ImVec2( Vec.x / Div.x, Vec.y / Div.y );
}

ImVec2 operator/( const ImVec2 &Vec, float Div ) noexcept
{
  return ImVec2( Vec.x / Div, Vec.y / Div );
}

const ImVec2 & operator/=( ImVec2 &Vec, float Div ) noexcept
{
  Vec.x /= Div;
  Vec.y /= Div;

  return Vec;
}

const ImVec2 & operator+=( ImVec2 &Vec, const ImVec2 &Add ) noexcept
{
  Vec.x += Add.x;
  Vec.y += Add.y;

  return Vec;
}

ImVec2 operator+( const ImVec2 &Vec, const ImVec2 &Add ) noexcept
{
  return ImVec2( Vec.x + Add.x, Vec.y + Add.y );
}

ImVec2 operator*( const ImVec2 &Vec, float Scale ) noexcept
{
  return ImVec2( Vec.x * Scale, Vec.y * Scale );
}

const ImVec2 & operator*=( ImVec2 &Vec, float Scale ) noexcept
{
  Vec.x *= Scale;
  Vec.y *= Scale;

  return Vec;
}

ImVec2 operator*( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept
{
  return ImVec2( Vec1.x * Vec2.x, Vec1.y * Vec2.y );
}

bool operator!=( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept
{
  return ( Vec1.x != Vec2.x || Vec1.y != Vec2.y );
}

ImVec2 operator-( const ImVec2 &Vec, const ImVec2 &Add ) noexcept
{
  return  ImVec2( Vec.x - Add.x, Vec.y - Add.y );
}

bool operator<=( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept
{
  return ( Vec1.x <= Vec2.x && Vec1.y <= Vec2.y );
}

bool operator>=( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept
{
  return ( Vec1.x >= Vec2.x && Vec1.y >= Vec2.y );
}
