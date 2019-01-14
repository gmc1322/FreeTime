/*!------------------------------------------------------------------------------
\file   InfixParser.h

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#pragma once

#include <string>
#include <unordered_map>

#include "Tracer.h"
#include "BinaryOperator.h"

class InfixParser
{
  public:
    typedef float ValueType;

  public:
    explicit InfixParser( const std::string &Infix ) noexcept;

  public:
    ValueType Evaluate( const std::unordered_map< std::string, float > &Variables = 
                          std::unordered_map< std::string, float >{} ) const ExceptDebug;

  public:
    static const std::unordered_map< unsigned char, BinaryOperator * > Operators;

  private:
    struct BadVariableName
    {
      explicit BadVariableName( std::string &&Error ) noexcept;
    };

  private:
    void PushBackToken( std::string::const_iterator &Iter, const std::string::const_iterator &End ) noexcept;

  private:
    std::string Postfix;
    const size_t Size;
};
