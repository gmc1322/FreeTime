/*!----------------------------------------------------------------------------
\file   Hash.h

\author Garrett Conti

\brief  Project: Golems_Magic
         Course: GAM300

\par    COPYRIGHT © 2018 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
---------------------------------------------------------------------------- */

#include "Public/Utils/Hash.h"

#include <cstdlib> // rand

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Hash::Hash( const Size_T TableSize ) NoExcept : BaseHash{ TableSize }, ConstRand( std::rand() ) {}

Hash::Hash( const Hash &Copy ) NoExcept : BaseHash{ Copy.TableSize }, ConstRand( std::rand() ) {}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Hash & Hash::operator=( const Hash &Copy ) NoExcept
{
  ResetTableSize( Copy.TableSize );

  return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Size_T Hash::GetInitialIndex( const char *Key ) const NoExcept
{
  Size_T HashIndex = 0;    // Initial value of hash
  Size_T Rand = ConstRand;

  const Size_T SizeMinusOne = TableSize - 1;

  // Process each char in string
  for( Size_T KeyCharacter = static_cast< Size_T >( *Key ); ; )
  {
    // Add in current char, keep within TableSize
    HashIndex = ( HashIndex + KeyCharacter ) % TableSize;

    // Get next char
    KeyCharacter = static_cast< Size_T >( *( ++Key ) );

    // If next char not '\0'
    if( KeyCharacter )
    {
      // Update Rand for next 'random' number
      Rand = ( Rand * ConstRand ) % SizeMinusOne;

      // Adjust hash total
      HashIndex *= Rand;
    }
    else break;
  }

  // Hash value is within 0 -> TableSize - 2 (unless there is only one letter in key, then it is: TableSize - 1 )
  return HashIndex;
}

Size_T Hash::GetInitialProbeIndex( const char *Key ) const NoExcept
{
  Size_T HashIndex = 0; // Initial value of hash

  // Process each char in the string
  for( Size_T KeyCharacter = static_cast< Size_T >( *Key ); ; )
  {
    // Add in current char and mod result
    HashIndex = ( ( HashIndex + KeyCharacter ) % PowX ) + 1; // +1 = can't have 0 increment

    // Get next char
    KeyCharacter = static_cast< Size_T >( *( ++Key ) );

    // If next char not '\0'
    if( KeyCharacter )
    {
      // Adjust hash total
      HashIndex *= ConstRand;
    }
    else break;
  }

  // Hash is within 1 -> round( TableSize ^ PowX )
  return HashIndex;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Hash::ResetTableSize( const Size_T NewSize ) NoExcept { ConstRand = std::rand(); BaseHash::ResetTableSize( NewSize ); }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PtrHash::PtrHash( const Size_T TableSize ) NoExcept : BaseHash{ TableSize } {}

Size_T PtrHash::GetInitialIndex( const void *Ptr ) const NoExcept
{
  size_t Key = reinterpret_cast< size_t >( Ptr ); // Initial value of PtrHash

  Key = ( ~Key ) + ( Key << 21 ); // Key = ( Key << 21 ) - Key - 1;
  Key ^= ( Key >> 24 );
  Key *= 265; // Key = ( Key + ( Key << 3 ) ) + ( Key << 8 )
  Key ^= ( Key >> 14 );
  Key *= 21; // Key = ( Key + ( Key << 2 ) ) + ( Key << 4 );
  Key ^= Key >> 28;
  Key += Key << 31;
  Key %= TableSize;

  // PtrHash value is within 0 -> TableSize - 1
  return static_cast< Size_T >( Key ); // NRVO
}

Size_T PtrHash::GetInitialProbeIndex( const void *Ptr ) const NoExcept
{
  size_t Key = reinterpret_cast< size_t >( Ptr ); // Initial value of PtrHash

  //Key = ( ~Key ) + ( Key << 18 );
  //Key ^= Key >> 31;
  //Key *= 21; // Key = ( Key + ( Key << 2 ) ) + ( Key << 4 );
  //Key ^= Key >> 11;
  //Key += Key << 6;
  //Key ^= Key >> 22;
  //Key %= std::lround( std::pow( TableSize, 0.6 ) );

  //++Key;  // ++ = can't have 0 increment

  Key = ( ~Key ) + ( Key << 18 );
  Key ^= Key >> 31;
  Key *= 21; // Key = ( Key + ( Key << 2 ) ) + ( Key << 4 );
  Key ^= Key >> 11;
  Key += Key << 6;
  Key = ( ( Key ^ ( Key >> 22 ) ) % PowX ) + 1; // +1 = can't have 0 increment

  // PtrHash value is within 1 -> round( TableSize ^ PowX )
  return static_cast< Size_T >( Key ); // NRVO
}
