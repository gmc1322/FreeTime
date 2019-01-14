/*!------------------------------------------------------------------------------
\file   LoadFunctions.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

typedef class Input *InputPtr;

void LoadTree( const char *FileName, std::map< size_t, InputPtr > *InputData ) noexcept;
