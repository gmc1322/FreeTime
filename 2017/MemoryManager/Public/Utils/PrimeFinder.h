/*!----------------------------------------------------------------------------
\file   PrimeFinder.h

\author Garrett Conti
             
\brief  Project: PrimeFinder
        Course:  GAM
---------------------------------------------------------------------------- */

#pragma once

#include "Public/Utils/Macros.h"

#include <unordered_map>

class PrimeFinderClass
{
  public:
    ~PrimeFinderClass() NoExcept = default;

  public:
    /*!
     * \brief Finds the Nth Prime, where N is the PrimeIndex.
     *
     * \param PrimeIndex - The Index of the Prime to find.
     *
     * \return size_t - The Prime number at Index.
     */
    static size_t GetPrimeAtIndex( Size_T PrimeIndex )  NoExcept;

    /*!
     * \brief Finds the PrimeIndex closest to Number.  This will not check for primes above Number.
     *
     * \param Number - The Number to get a Prime for.
     *
     * \return Size_T - The PrimeIndex closest to Number.
     */
    static Size_T GetNearestPrimeIndex( size_t Number ) NoExcept;

  private:
    PrimeFinderClass() NoExcept = default;

    PrimeFinderClass( const PrimeFinderClass  &Copy ) NoExcept = default;
    PrimeFinderClass(       PrimeFinderClass &&Move ) NoExcept = default;

  private:
    struct PrimeResult
    {
      PrimeResult() NoExcept;

      PrimeResult( bool IsPrime, size_t PrimeNumber ) NoExcept;

      operator bool() const NoExcept;

      void operator=( const PrimeResult &Copy ) NoExcept;

            bool   IsPrime;
      const size_t PrimeNumber;
    };

  private:
    static PrimeResult PrimeFunction( size_t PrimeNumber ) NoExcept;

  private:
    static std::unordered_map< Size_T, size_t > PrimeCache;
    static std::unordered_map< size_t, Size_T > IndexCache;

    static       size_t CurrentLargestPrime;
    static const Size_T ThreadCount;
};
