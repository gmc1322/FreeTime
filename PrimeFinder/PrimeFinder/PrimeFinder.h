/*!----------------------------------------------------------------------------
\file   PrimeFinder.h

\author Garrett Conti
             
\brief  Project: PrimeFinder
        Course:  GAM300
---------------------------------------------------------------------------- */

#pragma once

#include "Public/Utils/Macros.h"

#include <vector>

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
    static bool PrimeFunction_Fast( size_t PrimeNumber ) NoExcept;

  private:
    static std::vector< size_t > IndexCache;

    static size_t NextCheckNumber;
};
