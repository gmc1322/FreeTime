#pragma once

template<>
class BitField< ulong_BitCount >
{
  public:
             BitField()                                                      noexcept = default;
    template< MaxBits Count >
    explicit BitField( const BitField< Count > &Copy )                       ExceptDebug;
    template< MaxBits Count >
    explicit BitField( BitField< Count > &&Move )                            ExceptDebug;
             BitField( std::initializer_list< bool > &&Bits )                noexcept;
             BitField( std::initializer_list< bool > &&Bits, MaxBits Start ) noexcept;

  public:
    static constexpr uchar BitCount = ulong_BitCount;

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

    friend std::ostream & operator<<( std::ostream &os, BitField const &Rhs )       noexcept;
    const      BitField   operator<<( MaxBits Shift )                         const noexcept;
    const      BitField   operator>>( MaxBits Shift )                         const noexcept;

    BitField & operator<<=( MaxBits Shift ) noexcept;
    BitField & operator>>=( MaxBits Shift ) noexcept;

    template< MaxBits Count >
    const BitField< ulong_BitCount + Count > operator+( const BitField< Count > &Rhs  ) const noexcept;
    template< MaxBits Count >
    const BitField< ulong_BitCount + Count > operator+(       BitField< Count > &&Rhs ) const noexcept;

    template< MaxBits Count >
    BitField & operator=( const BitField< Count > &Rhs  )              ExceptDebug;
    template< MaxBits Count >
    BitField & operator=(       BitField< Count > &&Rhs )              ExceptDebug;
    BitField & operator=(       std::initializer_list< bool > &&Bits ) noexcept;

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
    const BitField Get( MaxBits Start, MaxBits End ) const ExceptDebug;

  public:
    static constexpr ushort Sizeof() noexcept;

  public:
    ulong Number() const noexcept;

    /**
     * \brief This will put the intervals' numbers into the vector.
     *
     * \param Intervals - The multiple ranges of bits to count.
     *
     * \return std::vector< ulong > - The numbers from each range.
     *
     * The range is [Start,End].
     * \par
     * The intervals do not need to be connected.
     */
    const std::vector< ulong > Number( std::initializer_list< std::pair< size_t, size_t > > &&Intervals ) const ExceptDebug;

  private:
    ulong Bits = 0;

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



#include "BitField32.cpp"
