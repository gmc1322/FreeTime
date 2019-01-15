/*!------------------------------------------------------------------------------
\file   InfixParser.cpp

\author Garrett Conti

\par    Project: GAME
\par    Course:  GAM200S17
   ------------------------------------------------------------------------------ */

#include "InfixParser.h"

#include <iostream>

#include "Plus.h"
#include "Minus.h"
#include "Multiply.h"
#include "Divide.h"
#include "Modulo.h"
#include "Exponent.h"

const std::unordered_map< unsigned char, BinaryOperator * > InfixParser::Operators{ 
  { '+', new Plus{} }, { '-', new Minus{} }, { '*', new Multiply{} }, { '/', new Divide{} }, { '%', new Modulo{} }, { '^', new Exponent{} } 
};

InfixParser::InfixParser( const std::string &Infix ) noexcept : Size( Infix.size() )
{
  Postfix.reserve( Size );

  size_t Index = 0;
  size_t MaxCount = 0;

  // Only need half the characters since there will always be one operator per 2 characters (excluding the right parens)
  char *Stack = new char[ Size / 2 ]; // div by integer (not float) = floor function

  DebugOnly( std::cout << "Before = " << Infix << std::endl );

  for( std::string::const_iterator Iter = Infix.begin(), End = Infix.end(); Iter != End; )
  {
    if( ( *Iter >= '0' && *Iter <= '9' ) || *Iter == '_' )
    {
      PushBackToken( Iter, End );

      continue;
    }

    if( ( *Iter >= 'A' && *Iter <= 'Z' ) || ( *Iter >= 'a' && *Iter <= 'z' ) )
    {     
      PushBackToken( Iter, End );

      continue;
    } 
    
    if( *Iter == ')' )
    {
      while( Stack[ --Index ] != '(' )
      {
        Postfix.push_back( Stack[ Index ] );
      }
    }
    else if( *Iter == '^' )
    {
      Stack[ Index ] = '^';

      ++Index;
    }
    else
    {
      while( Index && Stack[ Index - 1 ] == '^' )
      {
        Postfix.push_back( Stack[ --Index ] );
      }

      if( Index && ( *Iter == '-' || *Iter == '+' ) )
      {
        for( ; ; )
        {
          const char &Char = Stack[ Index - 1 ];

          if( Char != '*' && Char != '/' && Char != '%' )
          {
            break;
          }

          Postfix.push_back( Stack[ --Index ] );

          if( !Index )
          {
            break;
          }
        }
      }

      Stack[ Index ] = *Iter;

      ++Index;

      if( Index > MaxCount )
      {
        MaxCount = Index;
      }
    }

    ++Iter;
  }

  while( Index )
  {
    Postfix.push_back( Stack[ --Index ] );
  }

  DebugOnly( std::cout << "After = " << Postfix << std::endl );

  delete[] Stack;
}

InfixParser::ValueType InfixParser::Evaluate( const std::unordered_map< std::string, float > &Variables ) const ExceptDebug
{
  size_t Index = 0;

  // Only need half the characters plus one since there will always be 2 characters per operator
  ValueType *Stack = new ValueType[ static_cast< size_t >( Size / 2.0f + 1 ) ]; // add one, cast to int = ceil function

  size_t Count = 0;

  for( std::string::const_iterator Iter = Postfix.begin(), End = Postfix.end(); Iter != End; ++Count )
  {
    if( ( *Iter >= '0' && *Iter <= '9' ) || *Iter == '_' )
    {
      const size_t Amount = Postfix.find( ',', Count ) - Count;

      std::string Number = Postfix.substr( Count, Amount );

      Number.push_back( '\0' );

      if( Number[ 0 ] == '_' )
      {
        Number[ 0 ] = '-';
      }

      Stack[ Index ] = std::stof( Number );

      ++Index;

      Iter += Amount + 1;

      Count += Amount;

      continue;
    }

    if( ( *Iter >= 'A' && *Iter <= 'Z' ) || ( *Iter >= 'a' && *Iter <= 'z' ) )
    {
      const size_t Amount = Postfix.find( ',' , Count ) - Count;

      std::string Word = Postfix.substr( Count, Amount );

      DebugElse(
        const auto WordIter = Variables.find( Word );

        AssertAlways( WordIter == Variables.end(), ( BadVariableName{ std::move( Word ) } ) );

        Stack[ Index ] = WordIter->second;

        ,

        Stack[ Index ] = Variables.find( Word )->second;
      )

      Iter += Amount + 1;

      Count += Amount;

      ++Index;

      continue;
    }

    Stack[ Index - 2 ] = Operators.find( *Iter )->second->operator()( Stack[ Index - 2 ], Stack[ Index - 1 ] );

    --Index;

    ++Iter;
  }

  const ValueType Value = *Stack;

  delete[] Stack;

  return Value; // Named RVO
}

InfixParser::BadVariableName::BadVariableName( std::string &&Error ) noexcept
{
  std::cout << "Could not find a value for the Variable Named '" << Error.c_str() << "'!" << std::endl;
}

void InfixParser::PushBackToken( std::string::const_iterator &Iter, const std::string::const_iterator &End ) noexcept
{
  for( ; ; )
  {
    Postfix.push_back( *Iter );

    if( ++Iter == End )
    {
      break;
    }

    for( const auto OpIter : Operators )
    {
      if( *Iter == OpIter.first )
      {
        goto WordEnd;
      }
    }

    if( *Iter == '(' || *Iter == ')' )
    {
      break;
    }
  }

  WordEnd:

  Postfix.push_back( ',' );
}
