#pragma once

#include <fstream>
#include <string>

#ifdef TRACER_EXPORTS
#define TRACER_API __declspec(dllexport)   
#else
#define TRACER_API __declspec(dllimport)   
#endif

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
      TRACER_API explicit BadFile( const char *FileName ) noexcept;
      TRACER_API explicit BadFile( const std::string &FileName ) noexcept;
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
    TRACER_API explicit Tracer( const char *FileName );

    /**
     * \brief Sets the file to use as output.
     *
     * \param FileName - The name of the new file.
     *
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
    TRACER_API explicit Tracer( const std::string &FileName );

  public:
    TRACER_API Tracer & operator<<( const char *Message ) noexcept;
    TRACER_API Tracer & operator<<( const std::string &Message ) noexcept;
    TRACER_API Tracer & operator<<( OutputStreamFunctionPtr OSFunction ) noexcept;
    TRACER_API Tracer & operator<<( Importance Level ) noexcept;
    TRACER_API Tracer & operator<<( Type MessageType ) noexcept;

  public:
    TRACER_API void SetImportance( Importance Level ) noexcept;
    TRACER_API void SetMessageType( Type MessageType ) noexcept;

    /**
     * \brief Sets a new file to use as output.
     *
     * \param FileName - The name of the new file.
     * 
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
    TRACER_API void SetOutputFile( const char *FileName );

    /**
     * \brief Sets a new file to use as output.
     *
     * \param FileName - The name of the new file.
     *
     * \except BadFile - The file could not be opened!
     *
     * \return void
     */
    TRACER_API void SetOutputFile( const std::string &FileName );

  private:
    TRACER_API void PrintType() noexcept;

  private:
    std::ofstream OutputFile;

    Importance ImportanceLevel = Verbose;
    Type MessageType = Information;

    bool PrintedType = false;
};

namespace std
{
  TRACER_API std::string to_string( Tracer::Importance Level ) noexcept;
  TRACER_API std::string to_string( Tracer::Type MessageType ) noexcept;
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

#ifdef _DEBUG

// Use this at the end of a function to tell it to only throw in debug
#define ExceptDebug noexcept( false )

#define DebugOnly( Stuff ) Stuff
#define DebugElse( Stuff, Else ) Stuff

// Use this if you want the check in debug only
#define AssertDebug( Case, Throw ) AssertAlways( Case, Throw )

// Use this if the need the check in release, just no-throw
#define AssertDebugKeep( Keep, Throw ) AssertAlways( Keep, Throw )

// Use this if the function may return an error in debug, but the call is still needed in release
#define AssertDebugVar( Return, Throw ) AssertDebugKeep( auto Err = Return, Throw( Err ) )

#else
#define ExceptDebug noexcept( true )
#define DebugOnly( Stuff )
#define DebugElse( Stuff, Else ) Else
#define AssertDebug( Case, Throw )
#define AssertDebugKeep( Keep, Throw ) Keep
#define AssertDebugVar( Return, Throw ) AssertDebugKeep( Return, Throw )
#endif
