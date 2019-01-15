#pragma once

#include <initializer_list>

#include "Tracer.h"

template< typename VecType, size_t VecSize >
struct Vector : Vector< VecType, 4 >
{
  public:
    typedef VecType Type;

  public:
    constexpr Vector() noexcept = default;

    /**
     * \brief Initializes all components with the Initializer.
     *
     * \param Initializer - The value to initialize the components with.
     *
     * Can't be used with braces! IE. Vector{1} -> Vector(1)
     */
    constexpr explicit Vector( const VecType &Initializer ) noexcept;

    /**
     * \brief This takes the item and MOVES it into Z.
     *
     * \param Items - A pointer to an array of items.
     */
    constexpr explicit Vector( const VecType *Items ) noexcept; 
    constexpr explicit Vector( std::initializer_list< VecType > &&Items ) noexcept;

    constexpr Vector( const Vector< VecType, VecSize > &Copy ) noexcept;
    constexpr Vector( Vector< VecType, VecSize > &&Move ) noexcept;
  
    template< typename OtherType >
    constexpr explicit Vector( const Vector< OtherType, VecSize > &Copy ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( Vector< OtherType, VecSize > &&Move ) noexcept;
  
  public:
    VecType & operator[]( size_t Index ) ExceptDebug;
    constexpr const VecType & operator[]( size_t Index ) const ExceptDebug;

    Vector< VecType, VecSize > & operator+=( const Vector< VecType, VecSize > &Rhs ) noexcept;
    Vector< VecType, VecSize > & operator+=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, VecSize > operator+( const Vector< VecType, VecSize > &Rhs ) const noexcept;
    constexpr const Vector< VecType, VecSize > operator+( const VecType &Rhs ) const noexcept;

    Vector< VecType, VecSize > & operator-=( const Vector< VecType, VecSize > &Rhs ) noexcept;
    Vector< VecType, VecSize > & operator-=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, VecSize > operator-( const Vector< VecType, VecSize > &Rhs ) const noexcept;
    constexpr const Vector< VecType, VecSize > operator-( const VecType &Rhs ) const noexcept;

    Vector< VecType, VecSize > & operator*=( const Vector< VecType, VecSize > &Rhs ) noexcept;
    Vector< VecType, VecSize > & operator*=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, VecSize > operator*( const Vector< VecType, VecSize > &Rhs ) const noexcept;
    constexpr const Vector< VecType, VecSize > operator*( const VecType &Rhs ) const noexcept;

    Vector< VecType, VecSize > & operator/=( const Vector< VecType, VecSize > &Rhs ) noexcept;
    Vector< VecType, VecSize > & operator/=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, VecSize > operator/( const Vector< VecType, VecSize > &Rhs ) const noexcept;
    constexpr const Vector< VecType, VecSize > operator/( const VecType &Rhs ) const noexcept;

    Vector< VecType, VecSize > & operator=( const Vector< VecType, VecSize > &Copy ) noexcept;
    Vector< VecType, VecSize > & operator=( Vector< VecType, VecSize > &&Move ) noexcept;

    template< typename NewType >
    constexpr operator Vector< NewType, VecSize >() const noexcept;

    template< typename Type, size_t Size >
    friend std::ostream & operator<<( std::ostream &Lhs, const Vector< Type, Size > &Rhs ) noexcept;

    bool operator==( const Vector< VecType, VecSize > &Rhs ) const noexcept;

  private:
    struct IndexTooLarge
    {
      explicit IndexTooLarge( size_t Index ) noexcept;
    };

  private:
    Vector< VecType, VecSize - 4 > Others;
};

template< typename VecType >
struct Vector< VecType, 4 > : Vector< VecType, 3 >
{
  public:
    typedef VecType Type;

  public:
    constexpr Vector() noexcept = default;
     
    /**
     * \brief Initializes all components with the Initializer.
     *
     * \param Initializer - The value to initialize the components with.
     *
     * Can't be used with braces! IE. Vector{1} -> Vector(1)
     */
    constexpr explicit Vector( const VecType &Initializer ) noexcept;

    /**
     * \brief This takes the item and MOVES it into Z.
     *
     * \param Items - A pointer to an item.
     */
    constexpr explicit Vector( const VecType *Items ) noexcept;
    constexpr explicit Vector( std::initializer_list< VecType > &&Items ) noexcept;

    constexpr Vector( const Vector< VecType, 4 > &Copy ) noexcept;
    constexpr Vector( Vector< VecType, 4 > &&Move ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( const Vector< OtherType, 4 > &Copy ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( Vector< OtherType, 4 > &&Move ) noexcept;

  public:
    VecType & operator[]( size_t Index ) ExceptDebug;
    constexpr const VecType & operator[]( size_t Index ) const ExceptDebug;

    Vector< VecType, 4 > & operator+=( const Vector< VecType, 4 > &Rhs ) noexcept;
    Vector< VecType, 4 > & operator+=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 4 > operator+( const Vector< VecType, 4 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 4 > operator+( const VecType &Rhs ) const noexcept;

    Vector< VecType, 4 > & operator-=( const Vector< VecType, 4 > &Rhs ) noexcept;
    Vector< VecType, 4 > & operator-=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 4 > operator-( const Vector< VecType, 4 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 4 > operator-( const VecType &Rhs ) const noexcept;

    Vector< VecType, 4 > & operator*=( const Vector< VecType, 4 > &Rhs ) noexcept;
    Vector< VecType, 4 > & operator*=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 4 > operator*( const Vector< VecType, 4 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 4 > operator*( const VecType &Rhs ) const noexcept;

    Vector< VecType, 4 > & operator/=( const Vector< VecType, 4 > &Rhs ) noexcept;
    Vector< VecType, 4 > & operator/=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 4 > operator/( const Vector< VecType, 4 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 4 > operator/( const VecType &Rhs ) const noexcept;

    Vector< VecType, 4 > & operator=( const Vector< VecType, 4 > &Copy ) noexcept;
    Vector< VecType, 4 > & operator=( Vector< VecType, 4 > &&Move ) noexcept;

    template< typename NewType >
    constexpr operator Vector< NewType, 4 >() const noexcept;

    template< typename Type >
    friend std::ostream & operator<<( std::ostream &Lhs, const Vector< Type, 4 > &Rhs ) noexcept;

    bool operator==( const Vector< VecType, 4 > &Rhs ) const noexcept;

  public:
    VecType w = VecType();

  private:
    struct IndexTooLarge
    {
      explicit IndexTooLarge( size_t Index ) noexcept;
    };
};

template< typename VecType >
struct Vector< VecType, 3 > : Vector< VecType, 2 >
{
  public:
    typedef VecType Type;

  public:
    constexpr Vector() noexcept = default;
    
    /**
     * \brief Initializes all components with the Initializer.
     *
     * \param Initializer - The value to initialize the components with.
     *
     * Can't be used with braces! IE. Vector{1} -> Vector(1)
     */
    constexpr explicit Vector( const VecType &Initializer ) noexcept;

    /**
     * \brief This takes the item and MOVES it into Z.
     *
     * \param Items - A pointer to an item.
     */
    constexpr explicit Vector( const VecType *Items ) noexcept;
    constexpr explicit Vector( std::initializer_list< VecType > &&Items ) noexcept;

    constexpr Vector( const Vector< VecType, 3 > &Copy ) noexcept;
    constexpr Vector( Vector< VecType, 3 > &&Move ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( const Vector< OtherType, 3 > &Copy ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( Vector< OtherType, 3 > &&Move ) noexcept;

    public:
    VecType & operator[]( size_t Index ) ExceptDebug;
    constexpr const VecType & operator[]( size_t Index ) const ExceptDebug;

    Vector< VecType, 3 > & operator+=( const Vector< VecType, 3 > &Rhs ) noexcept;
    Vector< VecType, 3 > & operator+=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 3 > operator+( const Vector< VecType, 3 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 3 > operator+( const VecType &Rhs ) const noexcept;

    Vector< VecType, 3 > & operator-=( const Vector< VecType, 3 > &Rhs ) noexcept;
    Vector< VecType, 3 > & operator-=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 3 > operator-( const Vector< VecType, 3 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 3 > operator-( const VecType &Rhs ) const noexcept;

    Vector< VecType, 3 > & operator*=( const Vector< VecType, 3 > &Rhs ) noexcept;
    Vector< VecType, 3 > & operator*=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 3 > operator*( const Vector< VecType, 3 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 3 > operator*( const VecType &Rhs ) const noexcept;

    Vector< VecType, 3 > & operator/=( const Vector< VecType, 3 > &Rhs ) noexcept;
    Vector< VecType, 3 > & operator/=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 3 > operator/( const Vector< VecType, 3 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 3 > operator/( const VecType &Rhs ) const noexcept;

    Vector< VecType, 3 > & operator=( const Vector< VecType, 3 > &Copy ) noexcept;
    Vector< VecType, 3 > & operator=( Vector< VecType, 3 > &&Move ) noexcept;

    template< typename NewType >
    constexpr operator Vector< NewType, 3 >() const noexcept;

    template< typename Type >
    friend std::ostream & operator<<( std::ostream &Lhs, const Vector< Type, 3 > &Rhs ) noexcept;

    bool operator==( const Vector< VecType, 3 > &Rhs ) const noexcept;

  public:
    VecType z = VecType();

  private:
    struct IndexTooLarge
    {
      explicit IndexTooLarge( size_t Index ) noexcept;
    };
};

template< typename VecType >
struct Vector< VecType, 2 >
{
  public:
    typedef VecType Type;

  public:
    constexpr Vector() noexcept = default;

    /**
     * \brief Initializes all components with the Initializer.
     *
     * \param Initializer - The value to initialize the components with.
     *
     * Can't be used with braces! IE. Vector{1} -> Vector(1)
     */
    constexpr explicit Vector( const VecType &Initializer ) noexcept;

    /**
     * \brief This takes the item and MOVES it into Z.
     *
     * \param Items - A pointer to an item.
     */
    constexpr explicit Vector( const VecType *Items ) noexcept;
    constexpr explicit Vector( std::initializer_list< VecType > &&Items ) noexcept;

    constexpr Vector( const Vector< VecType, 2 > &Copy ) noexcept;
    constexpr Vector( Vector< VecType, 2 > &&Move ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( const Vector< OtherType, 2 > &Copy ) noexcept;

    template< typename OtherType >
    constexpr explicit Vector( Vector< OtherType, 2 > &&Move ) noexcept;

  public:
    VecType & operator[]( size_t Index ) ExceptDebug;
    constexpr const VecType & operator[]( size_t Index ) const ExceptDebug;

    Vector< VecType, 2 > & operator+=( const Vector< VecType, 2 > &Rhs ) noexcept;
    Vector< VecType, 2 > & operator+=( const VecType &Rhs ) noexcept;

    constexpr const Vector< VecType, 2 > operator+( const Vector< VecType, 2 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 2 > operator+( const VecType &Rhs ) const noexcept;

    Vector< VecType, 2 > & operator-=( const Vector< VecType, 2 > &Rhs ) noexcept;
    Vector< VecType, 2 > & operator-=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 2 > operator-( const Vector< VecType, 2 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 2 > operator-( const VecType &Rhs ) const noexcept;

    Vector< VecType, 2 > & operator*=( const Vector< VecType, 2 > &Rhs ) noexcept;
    Vector< VecType, 2 > & operator*=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 2 > operator*( const Vector< VecType, 2 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 2 > operator*( const VecType &Rhs ) const noexcept;

    Vector< VecType, 2 > & operator/=( const Vector< VecType, 2 > &Rhs ) noexcept;
    Vector< VecType, 2 > & operator/=( const VecType &Rhs ) noexcept;
    constexpr const Vector< VecType, 2 > operator/( const Vector< VecType, 2 > &Rhs ) const noexcept;
    constexpr const Vector< VecType, 2 > operator/( const VecType &Rhs ) const noexcept;

    Vector< VecType, 2 > & operator=( const Vector< VecType, 2 > &Copy ) noexcept;
    Vector< VecType, 2 > & operator=( Vector< VecType, 2 > &&Move ) noexcept;

    template< typename NewType >
    constexpr operator Vector< NewType, 2 >() const noexcept;

    template< typename Type >
    friend std::ostream & operator<<( std::ostream &Lhs, const Vector< Type, 2 > &Rhs ) noexcept;

    bool operator==( const Vector< VecType, 2 > &Rhs ) const noexcept;

  public:
    VecType x = VecType();
    VecType y = VecType();

  private:
    struct IndexTooLarge
    {
      explicit IndexTooLarge( size_t Index ) noexcept;
    };
};

#define SplitVec2( Vector ) Vector.x, Vector.y
#define SplitVec3( Vector ) SplitVec2( Vector ), Vector.z
#define SplitVec4( Vector ) SplitVec3( Vector ), Vector.w

/**
 * \brief Make a Vector from a list of Items.
 *
 * \tparam Type - The Type of the items.
 * \tparam Size - The Number of items.
 *
 * \param Items - The Items for the vector.
 *
 * \return auto - The created Vector
 *
 * A std::initializer_list can be use in place of a static array.  Ex: { 1,2,3 }
 * Can't be used to create constexpr Vectors.  Must create them manualy: constexpr Vector< int, 3 > ConstIntVec{ 1, 2, 3 };
 */
template< typename Type, size_t Size >
constexpr auto MakeVector( const Type( &&Items )[ Size ] ) noexcept;

#include "Vector.cpp"

