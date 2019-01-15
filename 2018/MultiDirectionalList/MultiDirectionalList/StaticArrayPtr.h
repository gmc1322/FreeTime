#pragma once

#include <ostream>

#include "Tracer.h"

#ifdef _DEBUG
// Taking the size of this will not yield sizeof(void*), you must subtract sizeof(size_t)!
template< typename PointerType, size_t Size >
class StaticArrayPtr
{ 
  public:
    StaticArrayPtr( PointerType *Ptr, size_t Pos ) noexcept;
    /**
     * \brief This is the move constructor.
     */
    StaticArrayPtr( PointerType* *Ptr, size_t Pos ) noexcept;

    StaticArrayPtr( const StaticArrayPtr &Copy ) noexcept;
    StaticArrayPtr(       StaticArrayPtr &&Move ) noexcept;

  public:
    StaticArrayPtr & operator=( const std::pair< PointerType*, size_t > &PtrPair ) noexcept;
    StaticArrayPtr & operator=(       std::pair< PointerType*, size_t > &&PtrPair ) noexcept;
    StaticArrayPtr & operator=( const StaticArrayPtr &Copy ) noexcept;
    StaticArrayPtr & operator=(       StaticArrayPtr &&Move ) noexcept;

          PointerType & operator[]( int Index ) ExceptDebug;
    const PointerType & operator[]( int Index ) const ExceptDebug;

    StaticArrayPtr operator+( int Index ) ExceptDebug;
    StaticArrayPtr< const PointerType, Size > operator+( int Index ) const ExceptDebug;
    StaticArrayPtr< const PointerType, Size > operator+( const StaticArrayPtr &Rhs ) const noexcept = delete;

    StaticArrayPtr operator-( int Index ) ExceptDebug;
    StaticArrayPtr< const PointerType, Size > operator-( int Index ) const ExceptDebug;
    int operator-( const StaticArrayPtr &Rhs ) const noexcept;
  
    StaticArrayPtr & operator+=( int Index ) ExceptDebug;

    StaticArrayPtr & operator-=( int Index ) ExceptDebug;

          PointerType & operator*() noexcept;
    const PointerType & operator*() const noexcept;

    PointerType       *       * operator&() noexcept;
    PointerType const * const * operator&() const noexcept;

    friend std::ostream & operator<<( std::ostream &os, const StaticArrayPtr &Rhs ) noexcept
    {
      return os << Rhs.Ptr;
    }

  private:
    struct IndexOutOfBounds
    {
      explicit IndexOutOfBounds( int Index, size_t Pos ) noexcept;
    };

  private:
    PointerType *Ptr;
    size_t Pos;
};

// Taking the size of this will not yield sizeof(void*), you must subtract sizeof(size_t)!
template< typename PointerType, size_t Size >
class ConstStaticArrayPtr
{
  public:
    ConstStaticArrayPtr( PointerType *Ptr, size_t Pos ) noexcept;
    /**
     * \brief This is the move constructor.
     */
    ConstStaticArrayPtr( PointerType* *Ptr, size_t Pos ) noexcept;

    ConstStaticArrayPtr( const ConstStaticArrayPtr &Copy ) noexcept;
    ConstStaticArrayPtr(       ConstStaticArrayPtr &&Move ) noexcept;

  public:
    ConstStaticArrayPtr & operator=( const std::pair< PointerType*, size_t > &PtrPair ) noexcept;
    ConstStaticArrayPtr & operator=(       std::pair< PointerType*, size_t > &&PtrPair ) noexcept;
    ConstStaticArrayPtr & operator=( const ConstStaticArrayPtr &Copy ) noexcept;
    ConstStaticArrayPtr & operator=(       ConstStaticArrayPtr &&Move ) noexcept;

          PointerType & operator[]( int Index ) ExceptDebug;
    const PointerType & operator[]( int Index ) const ExceptDebug;

         StaticArrayPtr< PointerType, Size > operator+( int Index ) ExceptDebug;
    ConstStaticArrayPtr< const PointerType, Size > operator+( int Index ) const ExceptDebug;
    ConstStaticArrayPtr< const PointerType, Size > operator+( const ConstStaticArrayPtr &Rhs ) const noexcept = delete;

         StaticArrayPtr< PointerType, Size > operator-( int Index ) ExceptDebug;
    ConstStaticArrayPtr< const PointerType, Size > operator-( int Index ) const ExceptDebug;
    int operator-( const ConstStaticArrayPtr &Rhs ) const noexcept;
  
    ConstStaticArrayPtr & operator+=( int Index ) ExceptDebug = delete;

    ConstStaticArrayPtr & operator-=( int Index ) ExceptDebug = delete;

          PointerType & operator*() noexcept;
    const PointerType & operator*() const noexcept;

    PointerType       * const * operator&() noexcept;
    PointerType const * const * operator&() const noexcept;

    friend std::ostream & operator<<( std::ostream &os, const ConstStaticArrayPtr &Rhs ) noexcept
    {
      return os << Rhs.Ptr;
    }

  private:
    struct IndexOutOfBounds
    {
      explicit IndexOutOfBounds( int Index, size_t Pos ) noexcept;
    };

  private:
    PointerType * const Ptr;
    size_t Pos;
};

#include "StaticArrayPtr.cpp"

#define StaticArrayPtr( Type, Size ) StaticArrayPtr< Type, Size >
#define ConstantStaticArrayPtr( Type, Size ) ConstStaticArrayPtr< Type, Size >
#else
#define StaticArrayPtr( Type, Size ) Type *
#define ConstantStaticArrayPtr( Type, Size ) Type * const
#endif
