#pragma once

#include <initializer_list>
#include <ostream>
#include <type_traits>
#include <vector>

#include "Tracer.h"

#include "StaticArrayPtr.h"

template< typename StaticArrayType, size_t FirstSize, size_t ... SizePack >
class StaticArray
{
  public:
    typedef StaticArray< StaticArrayType, SizePack... > Type;
    typedef std::vector< typename std::remove_const< Type >::type > StaticArrayInitType;

#ifdef _DEBUG
    template< typename Type, size_t Size >
    using ArrayPtr = StaticArrayPtr< Type, Size >;

    template< typename Type, size_t Size >
    using ConstArrayPtr = ConstStaticArrayPtr< Type, Size >;
#else
    template< typename Type, size_t Size >
    using ArrayPtr = Type*;

    template< typename Type, size_t Size >
    using ConstArrayPtr = Type* const;
#endif

  public:
    StaticArray() noexcept = default;

    StaticArray( const StaticArray &Copy ) noexcept;
    StaticArray(       StaticArray &&Move ) noexcept;

             StaticArray( const std::initializer_list< Type > &Args ) ExceptDebug;
    explicit StaticArray( const StaticArrayInitType           &Args ) ExceptDebug;
    explicit StaticArray(       StaticArrayInitType           &&Args ) ExceptDebug;
     
  public:
    void operator=( const StaticArray &Copy ) noexcept;
    void operator=(       StaticArray &&Move ) noexcept;

    void operator=( const std::initializer_list< Type > &Args ) ExceptDebug;
    void operator=( const StaticArrayInitType           &Args ) ExceptDebug;
    void operator=(       StaticArrayInitType           &&Args ) ExceptDebug;

    // These are built-in when in release mode
#ifdef _DEBUG
          Type & operator[]( size_t Index ) ExceptDebug;
    const Type & operator[]( size_t Index ) const ExceptDebug;

         ArrayPtr< Type, FirstSize > operator+( size_t Index ) ExceptDebug;
    ConstArrayPtr< const Type, FirstSize > operator+( size_t Index ) const ExceptDebug;

    int operator-( const StaticArray &Rhs ) const noexcept;

          Type & operator*() noexcept;
    const Type & operator*() const noexcept;
#endif

    StaticArray operator+( const StaticArray &Rhs ) const noexcept = delete;

    operator      ArrayPtr< Type, FirstSize >() noexcept;
    operator ConstArrayPtr< const Type, FirstSize >() const noexcept;

    friend std::ostream & operator<<( std::ostream &os, const StaticArray< StaticArrayType, FirstSize, SizePack... > &Rhs ) noexcept
    {
      return os << Rhs.Array;
    }

  public:
    template< size_t Dimension = 1 >
    static constexpr size_t Size() noexcept;

    Type * begin() noexcept;
    const Type * begin() const noexcept;

    Type * end() noexcept;
    const Type * end() const noexcept;

  private:
    struct TooManyInitializers
    {
      explicit TooManyInitializers( size_t Count ) noexcept;
    };

    struct IndexOutOfBounds
    {
      explicit IndexOutOfBounds( size_t Index ) noexcept;
    };

  private:
    Type Array[ FirstSize ];
};

template< typename ArrayType, size_t LastSize >
class StaticArray< ArrayType, LastSize >
{
  public:
    typedef ArrayType Type;
    typedef std::vector< typename std::remove_const< ArrayType >::type > StaticArrayInitType;

#ifdef _DEBUG
    template< typename Type, size_t Size >
    using ArrayPtr = StaticArrayPtr< Type, Size >;

    template< typename Type, size_t Size >
    using ConstArrayPtr = ConstStaticArrayPtr< Type, Size >;
#else
    template< typename Type, size_t Size >
    using ArrayPtr = Type*;

    template< typename Type, size_t Size >
    using ConstArrayPtr = Type* const;
#endif

  public:
    StaticArray() noexcept = default;

    StaticArray( const StaticArray &Copy ) noexcept;
    StaticArray(       StaticArray &&Move ) noexcept;

             StaticArray( const std::initializer_list< ArrayType > &Args ) ExceptDebug;
    explicit StaticArray( const StaticArrayInitType                &Args ) ExceptDebug;
    explicit StaticArray(       StaticArrayInitType                &&Args ) ExceptDebug;

  public:
    void operator=( const StaticArray &Copy ) noexcept;
    void operator=(       StaticArray &&Move ) noexcept;

    void operator=( const std::initializer_list< ArrayType > &Args ) ExceptDebug;
    void operator=( const StaticArrayInitType                &Args ) ExceptDebug;
    void operator=(       StaticArrayInitType                &&Args ) ExceptDebug;

  // These are built-in when in release mode
#ifdef _DEBUG
          ArrayType & operator[]( size_t Index ) ExceptDebug;
    const ArrayType & operator[]( size_t Index ) const ExceptDebug;

         ArrayPtr< ArrayType, LastSize > operator+( size_t Index ) ExceptDebug;
    ConstArrayPtr< const ArrayType, LastSize > operator+( size_t Index ) const ExceptDebug;

    int operator-( const StaticArray &Rhs ) const noexcept;

          ArrayType & operator*() noexcept;
    const ArrayType & operator*() const noexcept;
#endif

    StaticArray operator+( const StaticArray &Rhs ) const noexcept = delete;

    operator      ArrayPtr< ArrayType, LastSize >() noexcept;
    operator ConstArrayPtr< const ArrayType, LastSize >() const noexcept;

    friend std::ostream & operator<<( std::ostream &os, const StaticArray< ArrayType, LastSize > &Rhs ) noexcept
    {
      return os << Rhs.Array;
    }

  public:
    template< size_t Dimension = 1 >
    static constexpr size_t Size() noexcept;

          ArrayType * begin() noexcept;
    const ArrayType * begin() const noexcept;

          ArrayType * end() noexcept;
    const ArrayType * end() const noexcept;

  private:
    struct TooManyInitializers
    {
      explicit TooManyInitializers( size_t Count ) noexcept;
    };

    struct IndexOutOfBounds
    {
      explicit IndexOutOfBounds( size_t Index ) noexcept;
    };

  private:
    typename std::enable_if< !( std::is_reference< ArrayType >::value ), ArrayType >::type Array[ LastSize ];
};

#include "StaticArray.cpp"

/**
 * Only needed for the first type
 */
#define ArrayType( Array ) decltype( Array )::Type
