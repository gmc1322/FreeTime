/*!------------------------------------------------------------------------------
\file   ImVec.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

struct ImVec2;

ImVec2 operator/( const ImVec2 &Vec, const ImVec2 &Div ) noexcept;
ImVec2 operator/( const ImVec2 &Vec, float Div ) noexcept;
const ImVec2 & operator/=( ImVec2 &Vec, float Div ) noexcept;

const ImVec2 & operator+=( ImVec2 &Vec, const ImVec2 &Add ) noexcept;
ImVec2 operator+( const ImVec2 &Vec, const ImVec2 &Add ) noexcept;

ImVec2 operator*( const ImVec2 &Vec, float Scale ) noexcept;
const ImVec2 & operator*=( ImVec2 &Vec, float Scale ) noexcept;
ImVec2 operator*( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept;

bool operator!=( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept;

ImVec2 operator-( const ImVec2 &Vec, const ImVec2 &Sub ) noexcept;

bool operator<=( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept;
bool operator>=( const ImVec2 &Vec1, const ImVec2 &Vec2 ) noexcept;
