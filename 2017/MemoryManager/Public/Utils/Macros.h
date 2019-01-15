/*!------------------------------------------------------------------------------
\file   Macros.h

\author Garrett Conti

\par    Project: Golems_Magic
\par    Course:  SUMMER
------------------------------------------------------------------------------ */

#pragma once

#include <iostream>

#define TEXT( Code ) Code

/*!
 * \brief Logs Text as Type to 'To'.
 *
 * \param Text - The text to log (wide version).
 * \param Type - The verbosity of the log.
 * \param To   - The log file to output to.
 */
#define LogWideTypeTo( Text, Type, To, ... ) std::printf( Text, __VA_ARGS__ )

/*!
 * \brief Logs Text as Type to 'To'.
 *
 * \param Text - The text to log.
 * \param Type - The verbosity of the log.
 * \param To   - The log file to output to.
 */
#define LogTypeTo( Text, Type, To, ... ) std::printf( Text, __VA_ARGS__ )

/*!
 * \brief Logs Text as Display to 'To'.
 *
 * \param Text - The text to log (wide version).
 * \param To   - The log file to output to.
 */
#define LogWideTo( Text, To, ... ) LogWideTypeTo( To, Display, Text, __VA_ARGS__ )

/*!
 * \brief Logs Text as Display to 'To'.
 *
 * \param Text - The text to log.
 * \param To   - The log file to output to.
 */
#define LogTo( Text, To, ... ) LogWideTo( To, TEXT( Text ), __VA_ARGS__ )

/*!
 * \brief Logs Text as Type to Temp.
 *
 * \param Text - The text to log (wide version).
 * \param Type - The verbosity of the log.
 */
#define LogWideType( Text, Type, ... ) LogWideTypeTo( Text, Type, LogTemp, __VA_ARGS__ )

/*!
 * \brief Logs Text as Type to Temp.
 *
 * \param Text - The text to log.
 * \param Type - The verbosity of the log.
 */
#define LogType( Text, Type, ... ) LogWideTypeTo( TEXT( Text ), Type, LogTemp, __VA_ARGS__ )

 /*!
 * \brief Checks if If is true then Logs Text as Type to Temp.
 *
 * \param Text - The text to log (wide version).
 * \param Type - The verbosity of the log.
 */
#define LogWideTypeIf( If, Text, Type, ... ) if( If ){ LogWideType( Text, Type, __VA_ARGS__ ) }

 /*!
 * \brief Checks if If is true then Logs Text as Type to Temp.
 *
 * \param Text - The text to log.
 * \param Type - The verbosity of the log.
 */
#define LogTypeIf( If, Text, Type, ... ) LogWideTypeIf( If, TEXT( Text ), Type, LogTemp, __VA_ARGS__ )

 /*!
 * \brief Logs Text as Display to Temp.
 *
 * \param Text - The text to Display and log (wide version).
 */
#define LogWide( Text, ... ) LogWideType( Text, Display, __VA_ARGS__ )

/*!
 * \brief Checks if If is true then Logs Text as Display to Temp.
 *
 * \param If   - The condition to check if true.
 * \param Text - The text to Display and log (wide verison).
 */
#define LogWideIf( If, Text, ... ) LogWideTypeIf( If, Text, Display, __VA_ARGS__ )

/*!
 * \brief Checks if If is true then logs Text as an Error to Temp and runs the Return code.
 *
 * \param If     - The condition to check if true.
 * \param Text   - The text to log (wide version).
 * \param Return - The code to run after logging, can be used to run cleanup before returning.
 * 
 * NOTE: This will not return unless you put 'return' in the Return argument!
 */
#define AssertWide( If, Text, Return, ... ) if( If ) { LogWideType( Text, Error, __VA_ARGS__ ); Return; }

/*!
 * \brief Checks if If is true then logs Text as an Error to 'To' and runs the Return code.
 *
 * \param If     - The condition to check if true.
 * \param Text   - The text to log (wide version).
 * \param To     - The log file to output to.
 * \param Return - The code to run after logging, can be used to run cleanup before returning.
 *
 * NOTE: This will not return unless you put 'return' in the Return argument!
 */
#define AssertWideTo( If, Text, To, Return, ... ) if( If ) { LogWideTypeTo( Text, Error, To, __VA_ARGS__ ); Return; }

 /*!
  * \brief Checks at compile time if If is true then prints Text to the Output Window and stops compilation.
  * \param If   - The condition to check if true.
  * \param Text - The text to print on error.
  *
  * NOTE: This will not stop the compilation of other StaticAsserts! They will still be checked!
  */
#define StaticAssert( If, Text ) static_assert( !( If ), Text )

/*!
 * \brief Logs Text as Display to Temp.
 *
 * \param Text - The text to Display and log.
 */
#define Log( Text, ... ) LogWide( TEXT( Text ), __VA_ARGS__ )

/*!
 * \brief Checks if If is true then Logs Text as Display to Temp.
 *
 * \param If   - The condition to check if true.
 * \param Text - The text to Display and log.
 */
#define LogIf( If, Text, ... ) LogWideIf( If, TEXT( Text ), __VA_ARGS__ )

/*!
 * \brief Check if a statment is true, and if so logs an error to temp and runs the Return code.
 *
 * \param If     - The condition to check if true.
 * \param Text   - The text to log.
 * \param Return - The code to run after logging, can be used to run cleanup before returning.
 * 
 * NOTE: This will not return unless you put 'return' in the Return argument!
 */
#define Assert( If, Text, Return, ... ) AssertWide( If, TEXT( Text ), Return, __VA_ARGS__ )

/*!
 * \brief Checks if If is true then logs Text as an Error to 'To' and runs the Return code.
 *
 * \param If     - The condition to check if true.
 * \param Text   - The text to log.
 * \param To     - The log file to output to.
 * \param Return - The code to run after logging, can be used to run cleanup before returning.
 *
 * NOTE: This will not return unless you put 'return' in the Return argument!
 */
#define AssertTo( If, Text, To, Return, ... ) AssertWideTo( If, TEXT( Text ), To, Return, __VA_ARGS__ )

// Force Unreal to accecpt my ManualOverride of thier macro for diffrent VS configs
#ifdef WITH_EDITOR
#undef WITH_EDITOR
#endif

//! TODO Change to 0 when Shipping
//! Don't run shipping through VS! Run UE4 in DevEdit, then change to 0, then package game in editor
#if 1
#define DebugMode 1   // Only use when DebugOnly does not work
#define ReleaseMode 0 // Only use when DebugOnly does not work

#define WITH_EDITOR 1 // Only use when DebugMode does not work...

#define DebugLogTypeTo( Text, Type, File,  ... ) LogTypeTo( Text, Type, File, __VA_ARGS__ )

#define DebugLogType( Text, Type, ... ) LogType( Text, Type, __VA_ARGS__ )

#define DebugLogWide( Text, ... ) LogWide( Text, __VA_ARGS__ )
#define DebugLogWideIf( If, Text, ... ) LogWideIf( If, Text, __VA_ARGS__ )
#define DebugLogWideElseIf( If, Text, ... ) else LogWideIf( If, Text, __VA_ARGS__ )

#define DebugAssertWide( If, Text, Return, ... ) AssertWide( If, Text, Return, __VA_ARGS__ )
#define DebugAssertElseWide( If, Text, Return, ... ) else AssertWide( If, Text, Return, __VA_ARGS__ )
#define DebugAssertWideTo( If, Text, To, Return, ... ) AssertWideTo( If, Text, To, Return, __VA_ARGS__ )
#define DebugAssertElseWideTo( If, Text, To, Return, ... ) else AssertWideTo( If, Text, To, Return, __VA_ARGS__ )

#define DebugLog( Text, ... ) Log( Text, __VA_ARGS__ )
#define DebugLogIf( If, Text, ... ) LogIf( If, Text, __VA_ARGS__ )
#define DebugLogElseIf( If, Text, ... ) else LogIf( If, Text, __VA_ARGS__ )

#define DebugAssert( If, Text, Return, ... ) Assert( If, Text, Return, __VA_ARGS__ )
#define DebugAssertElse( If, Text, Return, ... ) else Assert( If, Text, Return, __VA_ARGS__ )
#define DebugAssertTo( If, Text, To, Return, ... ) AssertTo( If, Text, To, Return, __VA_ARGS__ )
#define DebugAssertElseTo( If, Text, To, Return, ... ) else AssertTo( If, Text, To, Return, __VA_ARGS__ )

#define DebugOnly( Code ) Code
#define ReleaseOnly( Code )

#define DebugElse( Code, Else ) Code

#define NoExcept noexcept
#else // Release Mode
#define DebugMode 0   // Only use when DebugOnly does not work  
#define ReleaseMode 1 // Only use when DebugOnly does not work

#define WITH_EDITOR 0 // Only use when DebugMode does not work...

#define DebugLogTypeTo( Text, Type, File,  ... )

#define DebugLogType( Text, Type, ... )

#define DebugLogWide( Text, ... )
#define DebugLogWideIf( If, Text, ... )
#define DebugLogWideElseIf( If, Text, ... )

#define DebugAssertWide( If, Text, Return, ... )
#define DebugAssertElseWide( If, Text, Return, ... )
#define DebugAssertWideTo( If, Text, To, Return, ... )
#define DebugAssertElseWideTo( If, Text, To, Return, ... )

#define DebugLog( Text, ... )
#define DebugLogIf( If, Text, ... )
#define DebugLogElseIf( If, Text, ... )

#define DebugAssert( If, Text, Return, ... )
#define DebugAssertElse( If, Text, Return, ... )
#define DebugAssertTo( If, Text, To, Return, ... )
#define DebugAssertElseTo( If, Text, To, Return, ... )

#define DebugOnly( Code )
#define ReleaseOnly( Code ) Code

#define DebugElse( Code, Else ) Else

// Unreal does not throw exceptions in release, so no need for noexcept (it actualy won't compile)
#define NoExcept 
#endif

using Size_T = unsigned int;
using UChar  = unsigned char;
