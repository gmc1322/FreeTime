
#include <iostream>

#include "Tracer.h"

#ifdef _DEBUG
#define ImportanceSwitch         \
if( ImportanceLevel == Release ) \
{                                \
  return *this;                  \
}    
#else
#define ImportanceSwitch       \
if( ImportanceLevel == Debug ) \
{                              \
  return *this;                \
}       
#endif

Tracer::BadFile::BadFile( const char* FileName ) noexcept
{
  std::cerr << "Could not open/create file: " << FileName << "!" << std::endl;
}

Tracer::BadFile::BadFile( const std::string &FileName ) noexcept
{
  std::cerr << "Could not open/create file: " << FileName.c_str() << "!" << std::endl;
}

Tracer::Tracer( const char *FileName ) noexcept( false )
{
  std::string File{ FileName };
  File.append( ".txt" );

  OutputFile.open( File );

  AssertAlways( !( OutputFile.is_open() ), BadFile( File ) );
}

Tracer::Tracer( const std::string &FileName ) noexcept( false )
{
  const std::string File{ FileName + ".txt" };

  OutputFile.open( File );

  AssertAlways( !( OutputFile.is_open() ), BadFile( File ) );
}

Tracer & Tracer::operator<<( const char *Message ) noexcept
{
  ImportanceSwitch

  Repeat.append( Message );

  return *this;
}

Tracer & Tracer::operator<<( const std::string &Message ) noexcept
{
  ImportanceSwitch

  Repeat.append( Message );

  return *this;
}

Tracer & Tracer::operator<<( OutputStreamFunctionPtr OSFunction ) noexcept
{
  ImportanceSwitch

  if( OSFunction == static_cast< std::basic_ostream< char > & ( * )( std::basic_ostream< char > & ) >( &std::endl ) )
  {
    if( Repeat != LastRepeat )
    {
      if( !PrintedType )
      {
        PrintType();
      }

      switch( MessageType )
      {
        case Information:
          std::cout << Repeat.c_str();
          break;

        case Warning:
        case Error:
          std::cerr << Repeat.c_str();
          break;
      }

      OutputFile << Repeat.c_str();

      RepeatPos = OutputFile.tellp();

      switch( MessageType )
      {
        case Information:
          std::cout << OSFunction;
          break;

        case Warning:
        case Error:
          std::cerr << OSFunction;
          break;
      }

      OutputFile << OSFunction;

      LastRepeat = Repeat;

      RepeatCount = 1;
    }
    else
    {
      OutputFile.seekp( RepeatPos );

      OutputFile << " InternalRepeatCount: " << ++RepeatCount << std::endl;
    }

    Repeat.clear();

#ifdef _DEBUG // Always flush in debug
    OutputFile.flush();

#else // Only flush in release if bad things happen
    if( MessageType == Error )
    {
      OutputFile.flush();
    }
#endif

    if( OldImportance )
    {
      ImportanceLevel = OldImportance;

      OldImportance = NullImportance;
    }

    if( OldType )
    {
      MessageType = OldType;

      OldType = NullType;
    }

    PrintedType = false;
  }

  return *this;
}

Tracer & Tracer::operator<<( Importance Level ) noexcept
{
  OldImportance = ImportanceLevel;

  ImportanceLevel = Level;

  return *this;
}

Tracer & Tracer::operator<<( Type MessageType_ ) noexcept
{
  OldType = MessageType;

  MessageType = MessageType_;

  PrintType();

  return *this;
}

void Tracer::SetImportance( Importance Level ) noexcept
{
  ImportanceLevel = Level;
}

void Tracer::SetMessageType( Type MessageType_ ) noexcept
{
  MessageType = MessageType_;
}

void Tracer::SetOutputFile( const char *FileName )
{
  OutputFile.close();

  std::string File{ FileName };
  File.append( ".txt" );

  OutputFile.open( File );

  AssertAlways( !( OutputFile.is_open() ), BadFile( File ) );
}

void Tracer::SetOutputFile( const std::string &FileName )
{
  OutputFile.close();

  std::string File{ FileName };
  File.append( ".txt" );

  OutputFile.open( File );

  AssertAlways( !( OutputFile.is_open() ), BadFile( File ) );
}

void Tracer::PrintType() noexcept
{
  switch( MessageType )
  {
    case Information:
      std::cout << "Information: ";
      break;

    case Warning:
      std::cerr << "WARNING! ";
      break;

    case Error:
      std::cerr << "ERROR!! ";
      break;
  }

  OutputFile << std::to_string( MessageType ) << ": ";

  PrintedType = true;
}

std::string std::to_string( Tracer::Importance Level ) noexcept
{
  switch( Level )
  {
    case Tracer::Release:
      return string{ "Release" };

    case Tracer::Debug:
      return string{ "Debug" };

    case Tracer::Verbose:
      return string{ "Verbose" };

    default:
      return string{};
  }
}

std::string std::to_string( Tracer::Type MessageType ) noexcept
{
  switch( MessageType )
  {
    case Tracer::Information:
      return string{ "Information" };

    case Tracer::Warning:
      return string{ "Warning" };

    case Tracer::Error:
      return string{ "Error" };

    default:
      return string{};
  }
}

#undef ImportanceSwitch
