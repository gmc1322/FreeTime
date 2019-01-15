#pragma once

#include <map>

typedef class Input *InputPtr;

void CreateTree( const char *FileName, const std::map< size_t, InputPtr > &InputData ) noexcept;
