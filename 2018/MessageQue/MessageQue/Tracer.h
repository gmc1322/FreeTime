#pragma once

#include <fstream>
#include <string>

#ifdef RDEBUG
#define _DEBUG
#endif

class Tracer
{
  public:
    enum Importance
    {
      Release, // Outputs in release mode
      Debug,   // Outputs in debug mode
      Verbose  // Outputs always
    };

    enum Type
    {
      Information,
      Warning,
      Error
    };

  public:
    struct BadFile
    {
       explicit BadFile( const char *FileName ) noexcept;
       explicit BadFile( const std::string &FileName ) noexcept;
    };

  public:
    typedef std::ostream & ( *OutputStreamFunctionPtr )( std::ostream & );

  public:
    /**
     * \brief Sets the file to use as output.
     *
     * \param FileName - The name of the new file.
     *
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
     explicit Tracer( const char *FileName );

    /**
     * \brief Sets the file to use as output.
     *
     * \param FileName - The name of the new file.
     *
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
     explicit Tracer( const std::string &FileName );

  public:
     Tracer & operator<<( const char *Message ) noexcept;
     Tracer & operator<<( const std::string &Message ) noexcept;
     Tracer & operator<<( OutputStreamFunctionPtr OSFunction ) noexcept;
     Tracer & operator<<( Importance Level ) noexcept;
     Tracer & operator<<( Type MessageType ) noexcept;

  public:
     void SetImportance( Importance Level ) noexcept;
     void SetMessageType( Type MessageType ) noexcept;

    /**
     * \brief Sets a new file to use as output.
     *
     * \param FileName - The name of the new file.
     * 
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
     void SetOutputFile( const char *FileName );

    /**
     * \brief Sets a new file to use as output.
     *
     * \param FileName - The name of the new file.
     *
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
     void SetOutputFile( const std::string &FileName );

  private:
     void PrintType() noexcept;

  private:
    std::ofstream OutputFile;

    Importance ImportanceLevel = Verbose;
    Type MessageType = Information;

    bool PrintedType = false;
};

namespace std
{
   std::string to_string( Tracer::Importance Level ) noexcept;
   std::string to_string( Tracer::Type MessageType ) noexcept;
}

///////////////////////////////////////////////////////////////////////////////
// Use these for program halters, use the above to continue running after error
// Usually, you will use these below, but wrap them in a catch with a Trace
//  for better readability for the user (not us)

// Use this if you always want the check
#define AssertAlways( Case, Throw ) if( Case )        \
                                    {                 \
                                      throw( Throw ); \
                                    }

// Use this if you need to clean things up before throwing
#define AssertCleanupAlways( Case, Cleanup, Throw ) if( Case )        \
                                                    {                 \
                                                      Cleanup;        \
                                                                      \
                                                      throw( Throw ); \
                                                    }



// Catches an assert and gives the throw a name: Error, to be used in the throw-back
#define CatchAssert( Try, Catch, Throw ) try                      \
                                         {                        \
                                           Try;                   \
                                         }                        \
                                         catch( Catch &Error )    \
                                         {                        \
                                           throw( Throw );        \
                                         }

// Catches an assert, but does not throw-back
#define CatchAssert_NoThrow( Try, Catch ) try               \
                                          {                 \
                                            Try;            \
                                          }                 \
                                          catch( Catch );

#ifdef _DEBUG

// Use this at the end of a function to tell it to only throw in debug
#define ExceptDebug noexcept( false )

#define DebugOnly( Stuff ) Stuff
#define DebugElse( Stuff, Else ) Stuff

// Use this if you want the check in debug only
#define AssertDebug( Case, Throw ) AssertAlways( Case, Throw )

// Use this if you only want to throw and cleanup in debug
#define AssertCleanupDebug( Case, Cleanup, Throw ) AssertCleanupAlways( Case, Cleanup, Throw )

// Use this if the need the check in release, just no-throw
#define AssertDebugKeep( Keep, Throw ) AssertAlways( Keep, Throw )

// Use this if the function may return an error in debug, but the call is still needed in release
#define AssertDebugVar( Return, Throw ) AssertDebugKeep( auto Err = Return, Throw( Err ) )

#define CatchAssertDebug( Try, Catch, Throw ) CatchAssert( Try, Catch, Throw )
#define CatchAssertDebug_NoThrow( Try, Catch ) CatchAssert_NoThrow( Try, Catch )

#else
#define ExceptDebug noexcept( true )
#define DebugOnly( Stuff )
#define DebugElse( Stuff, Else ) Else
#define AssertDebug( Case, Throw )
#define AssertCleanupDebug( Case, Cleanup, Throw )
#define AssertDebugKeep( Keep, Throw ) Keep
#define AssertDebugVar( Return, Throw ) AssertDebugKeep( Return, Throw )
#define CatchAssertDebug( Try, Catch, Throw ) Try
#define CatchAssertDebug_NoThrow( Try, Catch ) Try
#endif
