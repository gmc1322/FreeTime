/*!----------------------------------------------------------------------------
\file   PrimeFinder.cpp

\author Garrett Conti
             
\brief  Project: PrimeFinder
        Course:  GAM
---------------------------------------------------------------------------- */

#include "Public/Utils/Math/PrimeFinder.h"

#include <future>

std::unordered_map< Size_T, size_t > PrimeFinderClass::PrimeCache{ { 1, 2 } };
std::unordered_map< size_t, Size_T > PrimeFinderClass::IndexCache{ { 2, 1 } };

      size_t PrimeFinderClass::CurrentLargestPrime = 2;
const Size_T PrimeFinderClass::ThreadCount         = _Thrd_hardware_concurrency() * 2 - 1; // One less to account for this thread;

size_t PrimeFinderClass::GetPrimeAtIndex( const Size_T PrimeIndex ) NoExcept
{
  size_t Prime = 0;

  do
  {
    const auto Iter = PrimeCache.find( PrimeIndex );

    if( Iter != PrimeCache.cend() )
    {
      Prime = Iter->second;
    }
    else
    {
      std::vector< std::future< PrimeResult > > PrimeFinders;
      std::vector< size_t > Primes;
      std::vector< size_t > PrimeOverflow;

      PrimeFinders.reserve( ThreadCount );
      Primes.reserve( ThreadCount );
      PrimeOverflow.reserve( ThreadCount );

      do
      {
        ++CurrentLargestPrime;

        for( Size_T i = 0; i < ThreadCount; ++i, ++CurrentLargestPrime )
        {
          PrimeFinders.emplace_back( std::async( std::launch::async, PrimeFunction, CurrentLargestPrime ) );
        }

        const PrimeResult PotentialPrime = PrimeFunction( CurrentLargestPrime );

        for( auto &PrimeIter : PrimeFinders )
        {
          for( ; ; )
          {
            if( PrimeIter.wait_for( std::chrono::nanoseconds{ 305 } ) == std::future_status::ready ) // 305 = tested number
            {
              if( const PrimeResult Value = PrimeIter.get() )
              {
                Primes.emplace_back( Value.PrimeNumber );
              }

              break;
            }

            if( const PrimeResult PotentialPrimeOverflow = PrimeFunction( ++CurrentLargestPrime ) )
            {
              PrimeOverflow.emplace_back( PotentialPrimeOverflow.PrimeNumber );
            }
          }
        }

        if( PotentialPrime )
        {
          Primes.emplace_back( PotentialPrime.PrimeNumber );
        }

        Primes.insert( Primes.end(), PrimeOverflow.begin(), PrimeOverflow.end() );

        auto Index = static_cast< Size_T >( PrimeCache.size() );

        for( const auto PrimeIter : Primes )
        {
          PrimeCache.emplace( ++Index, PrimeIter );
          IndexCache.emplace( PrimeIter, Index );
        }

        PrimeFinders.clear();
        PrimeOverflow.clear();
      }
      while( Primes.empty() );
    }
  }
  while( !Prime );

  return Prime;
}

Size_T PrimeFinderClass::GetNearestPrimeIndex( size_t Number ) NoExcept
{
  Size_T Index;

  if( Number > CurrentLargestPrime )
  {
    for( auto CurrentIndex = static_cast< Size_T >( PrimeCache.size() ) + 1; ; ++CurrentIndex )
    {
      const size_t Prime = GetPrimeAtIndex( CurrentIndex );

      if( Prime > Number )
      {
        Index = CurrentIndex - 1;

        break;
      }
      else if( Prime == Number )
      {
        Index = CurrentIndex;

        break;
      }
    }
  }
  else if( PrimeFunction( Number ).IsPrime )
  {
    Index = IndexCache.find( Number )->second;
  }
  else
  {
    std::vector< std::future< PrimeResult > > PrimeFinders;
    std::vector< size_t > Primes;
    std::vector< size_t > PrimeOverflow;

    PrimeFinders.reserve( ThreadCount );
    Primes.reserve( ThreadCount );
    PrimeOverflow.reserve( ThreadCount );

    for( ; ; )
    {
      --Number;

      for( Size_T i = 0; i < ThreadCount; ++i, --Number )
      {
        if( Number > 2 )
        {
          PrimeFinders.emplace_back( std::async( std::launch::async, PrimeFunction, Number ) );
        }
        else break;
      }

      const PrimeResult PotentialPrime = PrimeFunction( Number );

      for( auto &PrimeIter : PrimeFinders )
      {
        for( ; ; )
        {
          if( PrimeIter.wait_for( std::chrono::nanoseconds{ 305 } ) == std::future_status::ready ) // 305 = tested number
          {
            if( const PrimeResult Value = PrimeIter.get() )
            {
              Primes.emplace_back( Value.PrimeNumber );
            }

            break;
          }

          if( Number > 2 )
          {
            if( const PrimeResult PotentialPrimeOverflow = PrimeFunction( --Number ) )
            {
              PrimeOverflow.emplace_back( PotentialPrimeOverflow.PrimeNumber );
            }
          }
        }
      }

      if( PotentialPrime )
      {
        Primes.emplace_back( PotentialPrime.PrimeNumber );
      }

      Primes.insert( Primes.end(), PrimeOverflow.begin(), PrimeOverflow.end() );

      if( Primes.empty() )
      {
        PrimeFinders.clear();
        PrimeOverflow.clear();
      }
      else
      {
        std::sort( Primes.begin(), Primes.end() );

        Index = IndexCache.find( Primes.back() )->second;

        break;
      }
    }
  }

  return Index; // NRVO
}

PrimeFinderClass::PrimeResult::PrimeResult() NoExcept : IsPrime( false ), PrimeNumber( 0 ) {}

PrimeFinderClass::PrimeResult::PrimeResult( const bool IsPrime, const size_t PrimeNumber ) NoExcept : IsPrime( IsPrime ), PrimeNumber( PrimeNumber ) {}

PrimeFinderClass::PrimeResult::operator bool() const NoExcept { return IsPrime; }

void PrimeFinderClass::PrimeResult::operator=( const PrimeResult &Copy ) NoExcept { IsPrime = Copy.IsPrime; const_cast< size_t& >( PrimeNumber ) = Copy.PrimeNumber; }

PrimeFinderClass::PrimeResult PrimeFinderClass::PrimeFunction( const size_t PrimeNumber ) NoExcept
{
  PrimeResult Result{ true, PrimeNumber };

  for( size_t i = 2; i * i <= PrimeNumber; ++i )
  {
    if( PrimeNumber % i == 0 )
    {
      Result.IsPrime = false;

      break;
    }
  }

  return Result;
}
