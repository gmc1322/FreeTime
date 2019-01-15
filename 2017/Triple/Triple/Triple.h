/*!------------------------------------------------------------------------------
\file   Triple.h

\author Garrett Conti

\par    Project: Triple
\par    Course:  S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

template< typename Type >
using UnrefedType = typename std::_Unrefwrap< Type >::type;

template< typename FirstType, typename SecondType, typename ThirdType >
struct Triple
{
  public:
    Triple() noexcept;

    Triple( const UnrefedType< FirstType > &FirstCopy, const UnrefedType< SecondType > &SecondCopy,
            const UnrefedType< ThirdType > &ThirdCopy ) noexcept;
    Triple( UnrefedType< FirstType > &&FirstMove, UnrefedType< SecondType > &&SecondMove, UnrefedType< ThirdType > &&ThirdMove ) noexcept;

    Triple( const Triple &Copy ) noexcept;
    Triple(       Triple &&Move ) noexcept;

  public:
    Triple & operator=( const Triple &Copy ) noexcept;
    Triple & operator=( Triple &&Move ) noexcept;

  public:
    FirstType First;
    SecondType Second;
    ThirdType Third;
};

template< typename FirstType, typename SecondType, typename ThirdType >
using UnrefedTriple = Triple< UnrefedType< FirstType >, UnrefedType< SecondType >, UnrefedType< ThirdType > >;

template< typename FirstType, typename SecondType, typename ThirdType >
constexpr UnrefedTriple< FirstType, SecondType, ThirdType > MakeTriple( FirstType &&First, SecondType &&Second, 
                                                                        ThirdType &&Third ) noexcept;

#include "Triple.cpp"
