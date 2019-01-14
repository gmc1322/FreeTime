#pragma once

#include <iostream>
#include <vector>

#include "Tracer.h"

typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

constexpr uchar BitsPerByte = 8;

constexpr uchar uchar_BitCount  = BitsPerByte * sizeof( uchar );
constexpr uchar ushort_BitCount = BitsPerByte * sizeof( ushort );
constexpr uchar ulong_BitCount  = BitsPerByte * sizeof( ulong );
constexpr uchar ullong_BitCount = BitsPerByte * sizeof( ullong );

typedef ushort MaxBits;

template< MaxBits N  >
class BitField
{
  public:
             BitField()                                                            noexcept = default;
    template< MaxBits Count >
    explicit BitField( const BitField< Count > &Copy )                             ExceptDebug;
    template< MaxBits Count >
    explicit BitField(       BitField< Count > &&Move )                            ExceptDebug;
             BitField(       std::initializer_list< bool > &&Bits )                noexcept;
             BitField(       std::initializer_list< bool > &&Bits, MaxBits Start ) noexcept;

  public:
    static constexpr uchar BitCount = ( N >= ullong_BitCount ? ullong_BitCount : ( N >= ulong_BitCount ? ulong_BitCount : ( N >= ushort_BitCount ? ushort_BitCount : uchar_BitCount ) ) );

    typedef BitField< BitCount > BitsType;
    typedef BitField< ( N > BitCount ? N - BitCount : 0 ) > RestType;

  public:
    template< MaxBits Count >
    BitField & operator^=( const BitField< Count > &Rhs ) noexcept;
    template< MaxBits Count >
    BitField & operator&=( const BitField< Count > &Rhs ) noexcept;
    template< MaxBits Count >
    BitField & operator|=( const BitField< Count > &Rhs ) noexcept;

    template< MaxBits Count >
    const BitField operator^( const BitField< Count > &Rhs ) const noexcept;
    template< MaxBits Count >
    const BitField operator&( const BitField< Count > &Rhs ) const noexcept;
    template< MaxBits Count >
    const BitField operator|( const BitField< Count > &Rhs ) const noexcept;

          BitField & operator~()       noexcept;
    const BitField   operator!() const noexcept;

    /**
     * \brief Outputs the bits inside the field.
     *
     * \tparam Count - The number of bits in the field.
     *
     * \param os  - Where to put the output.
     * \param Rhs - The bits to output.
     *
     * \return std::ostream& - The output.
     * 
     * Default bit output is sectioning the bits into the actual storage setup.
     * \par
     * To use a custom bit output formatting create a BitOutput{ X BitsPerSet } before outputing.
     */
    template< MaxBits Count > // Declair template again, since this is technically not part of the class
    friend std::ostream & operator<<( std::ostream &os, const BitField< Count > &Rhs )       noexcept;
    const      BitField   operator<<( MaxBits Shift )                                  const noexcept;
    const      BitField   operator>>( MaxBits Shift )                                  const noexcept;

    BitField & operator<<=( MaxBits Shift ) noexcept;
    BitField & operator>>=( MaxBits Shift ) noexcept;

    template< MaxBits Count >
    const BitField< N + Count > operator+( const BitField< Count > &Rhs  ) const noexcept;
    template< MaxBits Count >
    const BitField< N + Count > operator+(       BitField< Count > &&Rhs ) const noexcept;

    template< MaxBits Count >
    BitField & operator=( const BitField< Count > &Rhs  )              ExceptDebug;
    template< MaxBits Count >
    BitField & operator=(       BitField< Count > &&Rhs )              ExceptDebug;
    BitField & operator=(       std::initializer_list< bool > &&Bits ) ExceptDebug;

  public:
    void Set   ( MaxBits Index )                 ExceptDebug;
    void Set   ( MaxBits Index, bool Bit )       ExceptDebug;
    bool Get   ( MaxBits Index )           const ExceptDebug;
    void Clear ( MaxBits Index )                 ExceptDebug;
    void Negate( MaxBits Index )                 ExceptDebug;

    /**
     * \brief Gets the bits in the range [Start, End].
     *
     * \tparam Start - The first bit to get.
     * \tparam End   - The last bit to get.
     *
     * \return BitField< End - Start + 1 > - The resulting range of bits.
     */
    template< MaxBits Start, MaxBits End >
    constexpr BitField< End - Start + 1 > Get() const ExceptDebug;
    
    /**
     * \brief Gets the bits in the range [Start, End].
     *
     * \param Start - The first bit to get.
     * \param End   - The last bit to get.
     *
     * \return BitField - The resulting range of bits.
     * 
     * Only use this if you dont know the start or end at compile time!
     */
    constexpr BitField Get( MaxBits Start, MaxBits End ) const ExceptDebug;

  public:
    static constexpr MaxBits Sizeof() noexcept;

  public:
    /**
     * \brief This will wrap if you use more than 64 bits!
     *
     * \return ullong - The number represented by the bits in the BitField.
     */
    ullong Number() const noexcept;
  
    /**
     * \brief This will split the intervals into 64 bit chunks and put thier numbers into the vectors.
     *
     * \param Intervals - The multiple ranges of bits to count.
     *
     * \return std::vector< std::vector< ullong > > - The numbers from each range represented by 64 bit chunks in the inner vector.
     * 
     * The range is [Start,End].
     * \par
     * The intervals do not need to be connected.
     */
    const std::vector< std::vector< ullong > > Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) const ExceptDebug;

  private:
    std::ostream & Print1( std::ostream &os ) const noexcept;
    std::ostream & Print2( std::ostream &os ) const noexcept;

  private:    
    BitsType Bits;
    RestType Rest;

  private:
    struct IndexTooLarge
    {
      explicit IndexTooLarge( MaxBits Index ) noexcept;
    };

    struct BitFieldTooLarge
    {
      explicit BitFieldTooLarge( MaxBits Size ) noexcept;
    };
};

#pragma warning( push )
#pragma warning( disable : 4127 ) // Disable constant if statement in the Asserts
#pragma warning( disable : 4244 ) // Disable conversion warnings, these are handled by the asserts

#include "BitField.cpp"

#include "BitField64.h"
#include "BitField32.h"
#include "BitField16.h"
#include "BitField8.h"
#include "BitFieldEnd.h"

#pragma warning( pop )

#include "BitOutput.h"
