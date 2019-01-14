/*!------------------------------------------------------------------------------
\file   SaveFunctions.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include <map>

typedef class Input *InputPtr;

void SaveTree( const char *FileName, const std::map< size_t, InputPtr > &InputData ) noexcept;
