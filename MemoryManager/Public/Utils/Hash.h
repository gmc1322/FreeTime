/*!----------------------------------------------------------------------------
\file   Hash.h

\author Garrett Conti

\brief  Project: Golems_Magic
         Course: GAM300

\par    COPYRIGHT © 2018 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
---------------------------------------------------------------------------- */

#pragma once

#include "Public/Utils/Macros.h"
#include "Public/Utils/WrappingUtils.h"
#include "Public/Utils/PrimeFinder.h"

#include <cmath> // lround, pow
#include <type_traits>

template< typename HashType >
class BaseHash
{
#define PowXFunc( Size ) std::lround( std::pow( Size, 0.6 ) ) // 0.6 = tested number

  protected:
    using Type = std::conditional_t< std::is_pointer_v< HashType >, std::add_pointer_t< const std::remove_pointer_t< HashType > >, HashType >;

  public:
    BaseHash() NoExcept = default;

    BaseHash( const Size_T TableSize ) NoExcept : 
      TableSize( static_cast< Size_T >( PrimeFinderClass::GetPrimeAtIndex( PrimeFinderClass::GetNearestPrimeIndex( static_cast< size_t >( TableSize ) ) ) ) ),
      PowX( PowXFunc( TableSize ) ) {}

    BaseHash( const BaseHash  &Copy ) NoExcept = default;
    BaseHash(       BaseHash &&Move ) NoExcept = default;

    virtual ~BaseHash() NoExcept = default;

  public:
    BaseHash & operator=( const BaseHash  &Copy ) NoExcept = default;
    BaseHash & operator=(       BaseHash &&Move ) NoExcept = default;

  public:
    Size_T GetTableSize() const NoExcept { return TableSize; }

  public:
    /*!
     * \brief Call this to get the InitialIndex in an array of size 'TableSize'.
     *
     * \param Key - The string conversion of your data.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * First check the returned Index, then if that Index was already taken or it was not the correct data, call GetInitialProbeIndex.
     */
    virtual Size_T GetInitialIndex( const Type Key ) const NoExcept = 0;

    /*!
     * \brief Call this if the InitialIndex into an array of size 'TableSize' was already taken or if it was not the data you were looking for.
     *
     * \param Key - The string conversion of your data.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * First check the returned Index, then if that Index was already taken or it was not the correct data, start probing.
     */
    virtual Size_T GetInitialProbeIndex( const Type Key ) const NoExcept = 0;

  public:
    /*!
     * \brief Call this every time an Index into an array of size 'TableSize' was already taken or if it was not the data you were looking for.
     *
     * \param Index        - The current Index into the array.
     * \param InitialProbe - The Index returned from a call to GetInitialProbeIndex.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * If the returned Index is equal to the InitialIndex, there is no free space or the data was not found.
     */
    Size_T ProbeArray( Size_T Index, const Size_T InitialProbe ) const NoExcept
    {
      Index += InitialProbe;

      return ( Index >= TableSize ? UWrappingUtils::WrapSize_Max( Index, 0, TableSize - 1 ) : Index );
    }

  public:
    virtual void ResetTableSize( const Size_T Size ) NoExcept 
    { 
      TableSize = static_cast< Size_T >( PrimeFinderClass::GetPrimeAtIndex( PrimeFinderClass::GetNearestPrimeIndex( static_cast< size_t >( Size ) ) ) );
      PowX      = PowXFunc( Size ); 
    }

  protected:
    Size_T TableSize = 0;
    Size_T PowX      = 0;

#undef PowXFunc
};

class Hash : public BaseHash< char* >
{
  public:
    Hash() NoExcept = default;

    Hash( Size_T TableSize ) NoExcept;

    Hash( const Hash  &Copy ) NoExcept;
    Hash(       Hash &&Move ) NoExcept = default;

    ~Hash() NoExcept override = default;

  public:
    Hash & operator=( const Hash  &Copy ) NoExcept;
    Hash & operator=(       Hash &&Move ) NoExcept = default;

  public:
    /*!
     * \brief Call this to get the InitialIndex in an array of size 'TableSize'.
     *
     * \param Key - The string conversion of your data.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * First check the returned Index, then if that Index was already taken or it was not the correct data, call GetInitialProbeIndex.
     */
    Size_T GetInitialIndex( const Type Key ) const NoExcept override;

    /*!
     * \brief Call this if the InitialIndex into an array of size 'TableSize' was already taken or if it was not the data you were looking for.
     *
     * \param Key - The string conversion of your data.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * First check the returned Index, then if that Index was already taken or it was not the correct data, start probing.
     */
    Size_T GetInitialProbeIndex( const Type Key ) const NoExcept override;

  public:
    void ResetTableSize( Size_T Size ) NoExcept override;

  private:
    Size_T ConstRand = 0;
};

class PtrHash : public BaseHash< void* >
{
  public:
    PtrHash() NoExcept = default;
    PtrHash( Size_T TableSize ) NoExcept;

    PtrHash( const PtrHash  &Copy ) NoExcept = default;
    PtrHash(       PtrHash &&Move ) NoExcept = default;

    ~PtrHash() NoExcept override = default;

  public:
    PtrHash & operator=( const PtrHash  &Copy ) NoExcept = default;
    PtrHash & operator=(       PtrHash &&Move ) NoExcept = default;

  public:
    /*!
     * \brief Call this to get the InitialIndex in an array of size 'TableSize'.
     *
     * \param Key - The string conversion of your data.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * First check the returned Index, then if that Index was already taken or it was not the correct data, call GetInitialProbeIndex.
     */
    Size_T GetInitialIndex( const Type Ptr ) const NoExcept override;

    /*!
     * \brief Call this if the InitialIndex into an array of size 'TableSize' was already taken or if it was not the data you were looking for.
     *
     * \param Key - The string conversion of your data.
     *
     * \return Size_T - The Index into an array of size 'TableSize' for your data.
     *
     * First check the returned Index, then if that Index was already taken or it was not the correct data, start probing.
     */
    Size_T GetInitialProbeIndex( const Type Ptr ) const NoExcept override;
};
