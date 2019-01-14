/*!----------------------------------------------------------------------------
\file   Macros.h

\author Garrett Conti

\brief  Project: MultiObjectAllocator
         Course: FUN
---------------------------------------------------------------------------- */

#pragma once

/*!
 * \brief Checks at compile time if If is true then prints Text to the OutputWindow and stops compilation.
 * \param If   - The condition to check if true.
 * \param Text - The text to print on error.
 *
 * NOTE: This will not stop the compilation of other StaticAsserts! They will still be checked in some cases!
 */
#define StaticAssert( If, Text ) static_assert( !( If ), Text )

#if _DEBUG
  #define DebugException noexcept( false )

  #define DebugOnly( Code ) Code
  #define ReleaseOnly( Code )

  #define DebugElse( Code, Else ) Code

  #define DebugAssert( If, Throw ) if( If ) throw Throw;
#else // Release
  #define DebugException noexcept

  #define DebugOnly( Code )
  #define ReleaseOnly( Code ) Code

  #define DebugElse( Code, Else ) Else

  #define DebugAssert( If, Throw )
#endif
