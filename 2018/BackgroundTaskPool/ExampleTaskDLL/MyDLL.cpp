/*!----------------------------------------------------------------------------
\file   MyDLL.h

\author Garrett Conti

\brief  Project: BackgroundTaskPool
        Course:  FUN
---------------------------------------------------------------------------- */

#include "MyDLL.h"

void SharedMemFunc( void *const Args, void *const Result ) noexcept
{
  auto *const UArgs = reinterpret_cast< unsigned * >( Args );

  ++( *UArgs );

  *( reinterpret_cast< unsigned * >( Result ) ) = *UArgs;
}

void FileReadWriteFunc( std::ifstream &InFile, std::ofstream &OutFile ) noexcept
{
  unsigned Int;

  InFile >> Int;

  OutFile << Int;
}
