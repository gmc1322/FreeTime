#pragma once

template<>
class BitField< 0 >
{
  public:
             BitField()                                                      noexcept = default;
    template< MaxBits Count >
    explicit BitField( const BitField< Count > &Copy )                       ExceptDebug;
    template< MaxBits Count >
    explicit BitField( BitField< Count > &&Move )                            ExceptDebug;
             BitField( std::initializer_list< bool > &&Bits )                noexcept;
             BitField( std::initializer_list< bool > &&Bits, MaxBits Start ) noexcept;

  // Technically has size of 8 bits (1 byte) so we can take the address of it
  public:
    static constexpr uchar BitCount = 0;

  public:
    template< MaxBits Count >
    BitField & operator^=( const BitField< Count > &Rhs ) noexcept;
    template< MaxBits Count >
    BitField & operator&=( const BitField< Count > &Rhs ) noexcept;
    template< MaxBits Count >
    BitField & operator|=( const BitField< Count > &Rhs ) noexcept;

    template< MaxBits Count >
    constexpr BitField operator^( const BitField< Count > &Rhs ) const noexcept;
    template< MaxBits Count >
    constexpr BitField operator&( const BitField< Count > &Rhs ) const noexcept;
    template< MaxBits Count >
    constexpr BitField operator|( const BitField< Count > &Rhs ) const noexcept;

              BitField & operator~()       noexcept;
    constexpr BitField   operator!() const noexcept;

    friend std::ostream & operator<<( std::ostream &os, const BitField &Rhs )       noexcept;
    constexpr  BitField   operator<<( MaxBits Shift )                         const noexcept;
    constexpr  BitField   operator>>( MaxBits Shift )                         const noexcept;

    BitField & operator<<=( MaxBits Shift ) noexcept;
    BitField & operator>>=( MaxBits Shift ) noexcept;

    template< MaxBits Count >
    const BitField< 0 + Count > operator+( const BitField< Count > &Rhs  ) const noexcept;
    template< MaxBits Count >
    const BitField< 0 + Count > operator+(       BitField< Count > &&Rhs ) const noexcept;

    template< MaxBits Count >
    BitField & operator=( const BitField< Count > &Rhs )               ExceptDebug;
    template< MaxBits Count >
    BitField & operator=(       BitField< Count > &&Rhs )              ExceptDebug;
    BitField & operator=(       std::initializer_list< bool > &&Bits ) noexcept;

  public:
    static           void Set   ( MaxBits Index )           ExceptDebug;
    static           void Set   ( MaxBits Index, bool Bit ) ExceptDebug;
    static constexpr bool Get   ( MaxBits Index )           ExceptDebug;
    static           void Clear ( MaxBits Index )           ExceptDebug;
    static           void Negate( MaxBits Index )           ExceptDebug;

    /**
     * \brief Gets the bits in the range [Start, End].
     *
     * \tparam Start - The first bit to get.
     * \tparam End   - The last bit to get.
     *
     * \return BitField< 1 > - The resulting range of bits.
     */
    template< MaxBits Start, MaxBits End >
    static constexpr BitField< 1 > Get() ExceptDebug;

    /**
     * \brief Gets the bits in the range [Start, End].
     *
     * \param Start - The first bit to get.
     * \param End   - The last bit to get.
     *
     * \return BitField< 1 > - The resulting range of bits.
     *
     * Only use this if you dont know the start or end at compile time!
     */
    static const BitField< 1 > Get( MaxBits Start, MaxBits End ) ExceptDebug;

  public:
    static constexpr ushort Sizeof() noexcept;

  public:
    static constexpr ushort Number() noexcept;

    /**
     * \brief This will put the intervals' numbers into the vector.
     *
     * \param Intervals - The multiple ranges of bits to count.
     *
     * \return std::vector< ushort > - The numbers from each range.
     *
     * The range is [Start,End].
     * \par
     * The intervals do not need to be connected.
     */
    static const std::vector< ushort > Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) ExceptDebug;


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

#include "BitFieldEnd.cpp"
