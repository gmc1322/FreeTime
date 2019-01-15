#pragma once

#include <fstream>
#include <string>

#ifdef TRACER_EXPORTS
#define TRACER_API __declspec(dllexport)   
#else
#define TRACER_API __declspec(dllimport)   
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
    TRACER_API explicit Tracer( const char *FileName );
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

    TRACER_API void SetOutputFile( const char *FileName );
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
  TRACER_API string to_string( Tracer::Importance Level ) noexcept;
  TRACER_API string to_string( Tracer::Type MessageType ) noexcept;
}

// Use this if you always want the check
#define AssertAlways( Case, Throw ) if( Case )        \
                                    {                 \
                                      throw( Throw ); \
                                    }

#ifdef _DEBUG

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
