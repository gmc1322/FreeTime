﻿/*!----------------------------------------------------------------------------
\file   Triple.cpp

\author Garrett Conti

\par    Project: Triple
\par    Course:  S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <utility>

#include "Triple.h"

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType >::Triple() noexcept : First( FirstType{} ), Second( SecondType{} ), Third( ThirdType{} )
{
}

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType >::Triple( const FirstType &FirstCopy, const SecondType &SecondCopy,
                                                    const ThirdType &ThirdCopy ) noexcept : 
  First( FirstCopy ), Second( SecondCopy ), Third( ThirdCopy )
{
}

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType >::Triple( FirstType &&FirstMove, SecondType &&SecondMove, ThirdType &&ThirdMove ) noexcept :
  First( std::move( FirstMove ) ), Second( std::move( SecondMove ) ), Third( std::move( ThirdMove ) )
{
}

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType >::Triple( const Triple &Copy ) noexcept :
  First( Copy.First ), Second( Copy.Second ), Third( Copy.Third )
{
}

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType >::Triple( Triple &&Move ) noexcept : 
  First( std::move( Move.First ) ), Second( std::move( Move.Second ) ), Third( std::move( Move.Third ) )
{
}

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType > & Triple< FirstType, SecondType, ThirdType >::operator=( const Triple &Copy ) noexcept
{
  First = Copy.First;
  Second = Copy.Second;
  Third = Copy.Third;

  return *this;
}

template< typename FirstType, typename SecondType, typename ThirdType >
Triple< FirstType, SecondType, ThirdType> & Triple< FirstType, SecondType, ThirdType >::operator=( Triple &&Move ) noexcept
{
  First = std::move( Move.First );
  Second = std::move( Move.Second );
  Third = std::move( Move.Third );

  return *this;
}

template< typename FirstType, typename SecondType, typename ThirdType >
constexpr UnrefedTriple< FirstType, SecondType, ThirdType > MakeTriple( FirstType &&First, SecondType &&Second, 
                                                                        ThirdType &&Third ) noexcept
{
  return UnrefedTriple< FirstType, SecondType, ThirdType >( std::forward< FirstType >( First ),
                                                            std::forward< SecondType >( Second ),
                                                            std::forward< ThirdType >( Third ) );
}
