#include "Vector.h"

#include <iostream>

template <typename VecType, size_t VecSize>
constexpr Vector<VecType, VecSize>::Vector( const VecType &Initializer ) noexcept : Vector<VecType, 4>{ Initializer }, 
                                                                                    Others{ Initializer }
{
}

template< typename VecType, size_t VecSize >
constexpr Vector< VecType, VecSize >::Vector( const VecType *Items ) noexcept : Vector< VecType, 4 >{ Items }, Others{ Items + 4 }
{
}

template< typename VecType, size_t VecSize >
constexpr Vector< VecType, VecSize >::Vector( std::initializer_list< VecType > &&Items ) noexcept : Vector< VecType, 4 >{ Items },
                                                                                                    Others{ Items.begin() + 4 }
{
}

template< typename VecType, size_t VecSize >
constexpr Vector< VecType, VecSize >::Vector( const Vector< VecType, VecSize > &Copy ) noexcept : Vector< VecType, 4 >{ Copy },
                                                                                                  Others{ Copy.Others }
{
}

template<typename VecType, size_t VecSize >
constexpr Vector< VecType, VecSize >::Vector( Vector< VecType, VecSize > &&Move ) noexcept : Vector< VecType, 4 >{ std::move( Move ) },
                                                                                             Others{ std::move( Move.Others ) }
{
}

template <typename VecType, size_t VecSize>
template <typename OtherType>
constexpr Vector<VecType, VecSize>::Vector( const Vector<OtherType, VecSize>& Copy ) noexcept : Vector<VecType, 4>{ Copy },
                                                                                                Others{ Copy.Others }
{
}

template <typename VecType, size_t VecSize>
template <typename OtherType>
constexpr Vector<VecType, VecSize>::Vector( Vector<OtherType, VecSize>&& Move ) noexcept : Vector<VecType, 4>{ std::move( Move ) },
                                                                                           Others{ std::move( Move.Others ) }
{
}

template< typename VecType, size_t VecSize >
VecType & Vector< VecType, VecSize >::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= VecSize, IndexTooLarge( Index ) );

  return Index > 4 ? Others[ Index - 4 ] : *( &x + Index );
}

template< typename VecType, size_t VecSize >
constexpr const VecType & Vector< VecType, VecSize >::operator[]( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= VecSize, IndexTooLarge( Index ) );

  return Index > 4 ? Others[ Index - 4 ] : *( &x + Index );
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator+=( const VecType& Rhs ) noexcept
{
  x += Rhs;
  y += Rhs;
  z += Rhs;
  w += Rhs;

  Others += Rhs;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator+( const VecType& Rhs ) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return + Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator-=( const VecType& Rhs ) noexcept
{
  x -= Rhs;
  y -= Rhs;
  z -= Rhs;
  w -= Rhs;

  Others -= Rhs;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator-( const VecType& Rhs ) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return - Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator*=( const VecType& Rhs ) noexcept
{
  x *= Rhs;
  y *= Rhs;
  z *= Rhs;
  w *= Rhs;

  Others *= Rhs;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator*( const VecType& Rhs ) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return * Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator/=( const VecType& Rhs ) noexcept
{
  x /= Rhs;
  y /= Rhs;
  z /= Rhs;
  w /= Rhs;

  Others /= Rhs;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator/( const VecType& Rhs ) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return / Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator+=(const Vector<VecType, VecSize>& Rhs) noexcept
{
  x += Rhs.x;
  y += Rhs.y;
  z += Rhs.z;
  w += Rhs.w;

  Others += Rhs.Others;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator+(const Vector<VecType, VecSize>& Rhs) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return += Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator-=(const Vector<VecType, VecSize>& Rhs) noexcept
{
  x -= Rhs.x;
  y -= Rhs.y;
  z -= Rhs.z;
  w -= Rhs.w;

  Others -= Rhs.Others;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator-(const Vector<VecType, VecSize>& Rhs) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return -= Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator*=(const Vector<VecType, VecSize>& Rhs) noexcept
{
  x *= Rhs.x;
  y *= Rhs.y;
  z *= Rhs.z;
  w *= Rhs.w;

  Others *= Rhs.Others;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator*(const Vector<VecType, VecSize>& Rhs) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return *= Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator/=(const Vector<VecType, VecSize>& Rhs) noexcept
{
  x /= Rhs.x;
  y /= Rhs.y;
  z /= Rhs.z;
  w /= Rhs.w;

  Others /= Rhs.Others;

  return *this;
}

template <typename VecType, size_t VecSize>
constexpr const Vector<VecType, VecSize> Vector<VecType, VecSize>::operator/(const Vector<VecType, VecSize>& Rhs) const noexcept
{
  Vector<VecType, VecSize> Return{ *this };

  Return /= Rhs;

  return Return; // Named RVO
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator=(const Vector<VecType, VecSize>& Copy) noexcept
{
  x = Copy.x;
  y = Copy.y;
  z = Copy.z;
  w = Copy.w;

  Others = Copy.Others;

  return *this;
}

template <typename VecType, size_t VecSize>
Vector<VecType, VecSize>& Vector<VecType, VecSize>::operator=(Vector<VecType, VecSize>&& Move) noexcept
{
  x = std::move( Move.x );
  y = std::move( Move.y );
  z = std::move( Move.z );
  w = std::move( Move.w );

  Others = std::move( Move.Others );

  return *this;
}

template <typename VecType, size_t VecSize>
template <typename NewType>
constexpr Vector<VecType, VecSize>::operator Vector<NewType, VecSize>() const noexcept
{
  return Vector< NewType, VecSize >{ *this };
}

template< typename VecType, size_t VecSize >
std::ostream & operator<<( std::ostream &Lhs, const Vector< VecType, VecSize > &Rhs ) noexcept
{
  return Lhs << Rhs.x << ", " << Rhs.y << ", " << Rhs.z << ", " << Rhs.w << ", " << Rhs.Others;
}

template <typename VecType, size_t VecSize>
bool Vector<VecType, VecSize>::operator==( const Vector<VecType, VecSize>& Rhs ) const noexcept
{
  return x == Rhs.x && y == Rhs.y && z == Rhs.z && w == Rhs.w && Others == Rhs.Others;
}

template < typename VecType, size_t VecSize >
Vector< VecType, VecSize >::IndexTooLarge::IndexTooLarge( size_t Index ) noexcept
{
  std::cerr << "Vector Index: " << Index << ", Vector Size: " << VecSize << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

template <typename VecType>
constexpr Vector<VecType, 4>::Vector( const VecType &Initializer ) noexcept : Vector<VecType, 3>( Initializer ), w( Initializer )
{
}

template <typename VecType>
constexpr Vector<VecType, 4>::Vector( const VecType* Items ) noexcept : Vector<VecType, 3>{ Items }, w( std::move( *( Items + 3 ) ) )
{
}

template <typename VecType>
constexpr Vector<VecType, 4>::Vector( std::initializer_list<VecType>&& Items ) noexcept : Vector<VecType, 3>{ std::move( Items ) },
                                                                                          w( std::move( *( Items.begin() + 3 ) ) )
{
}

template <typename VecType>
constexpr Vector<VecType, 4>::Vector( const Vector<VecType, 4>& Copy ) noexcept : Vector<VecType, 3>{ Copy }, w( Copy.w )
{
}

template <typename VecType>
constexpr Vector<VecType, 4>::Vector( Vector<VecType, 4>&& Move ) noexcept : Vector<VecType, 3>{ std::move( Move ) }, w( std::move( Move.w ) )
{
}

template <typename VecType>
template <typename OtherType>
constexpr Vector<VecType, 4>::Vector( const Vector<OtherType, 4>& Copy ) noexcept : Vector<VecType, 3>{ Copy },
                                                                                    w( static_cast< VecType >( Copy.w ) )
{
}

template <typename VecType>
template <typename OtherType>
constexpr Vector<VecType, 4>::Vector( Vector<OtherType, 4>&& Move ) noexcept : Vector<VecType, 3>{ std::move( Move ) },
                                                                               w( std::move( static_cast< VecType >( Move.w ) ) )
{
}

template <typename VecType>
VecType& Vector<VecType, 4>::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= 4, IndexTooLarge( Index ) );

  return Index < 3 ? Vector<VecType, 3>::operator[]( Index ) : w;
}

template <typename VecType>
constexpr const VecType& Vector<VecType, 4>::operator[]( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= 4, IndexTooLarge( Index ) );

  return Index < 3 ? Vector<VecType, 3>::operator[]( Index ) : w;
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator+=( const VecType& Rhs ) noexcept
{
  x += Rhs;
  y += Rhs;
  z += Rhs;
  w += Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator+( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return + Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator-=( const VecType& Rhs ) noexcept
{
  x -= Rhs;
  y -= Rhs;
  z -= Rhs;
  w -= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator-( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return - Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator*=( const VecType& Rhs ) noexcept
{
  x *= Rhs;
  y *= Rhs;
  z *= Rhs;
  w *= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator*( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return * Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator/=( const VecType& Rhs ) noexcept
{
  x /= Rhs;
  y /= Rhs;
  z /= Rhs;
  w /= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator/( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return / Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator+=( const Vector<VecType, 4>& Rhs ) noexcept
{
  x += Rhs.x;
  y += Rhs.y;
  z += Rhs.z;
  w += Rhs.w;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator+( const Vector<VecType, 4>& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return += Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator-=( const Vector<VecType, 4>& Rhs ) noexcept
{
  x -= Rhs.x;
  y -= Rhs.y;
  z -= Rhs.z;
  w -= Rhs.w;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator-( const Vector<VecType, 4>& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return -= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator*=( const Vector<VecType, 4>& Rhs ) noexcept
{
  x *= Rhs.x;
  y *= Rhs.y;
  z *= Rhs.z;
  w *= Rhs.w;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator*( const Vector<VecType, 4>& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return *= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator/=( const Vector<VecType, 4>& Rhs ) noexcept
{
  x /= Rhs.x;
  y /= Rhs.y;
  z /= Rhs.z;
  w /= Rhs.w;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 4> Vector<VecType, 4>::operator/( const Vector<VecType, 4>& Rhs ) const noexcept
{
  Vector<VecType, 4> Return{ *this };

  Return /= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator=( const Vector<VecType, 4>& Copy ) noexcept
{
  x = Copy.x;
  y = Copy.y;
  z = Copy.z;
  w = Copy.w;

  return *this;
}

template <typename VecType>
Vector<VecType, 4>& Vector<VecType, 4>::operator=( Vector<VecType, 4>&& Move ) noexcept
{
  x = std::move( Move.x );
  y = std::move( Move.y );
  z = std::move( Move.z );
  w = std::move( Move.w );

  return *this;
}

template <typename VecType>
template <typename NewType>
constexpr Vector<VecType, 4>::operator Vector<NewType, 4>() const noexcept
{
  return Vector<NewType, 4>{ *this };
}

template< typename VecType >
std::ostream & operator<<( std::ostream &Lhs, const Vector< VecType, 4 > &Rhs ) noexcept
{
  return Lhs << Rhs.x << ", " << Rhs.y << ", " << Rhs.z << ", " << Rhs.w;
}

template <typename VecType>
bool Vector<VecType, 4>::operator==( const Vector<VecType, 4>& Rhs ) const noexcept
{
  return x == Rhs.x && y == Rhs.y && z == Rhs.z && w == Rhs.w;
}

template <typename VecType>
Vector<VecType, 4>::IndexTooLarge::IndexTooLarge( size_t Index ) noexcept
{
  std::cerr << "Vector Index: " << Index << ", Vector Size: 4" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

template <typename VecType>
constexpr Vector<VecType, 3>::Vector( const VecType &Initializer ) noexcept : Vector<VecType, 2>( Initializer ), z( Initializer )
{
}

template <typename VecType>
constexpr Vector<VecType, 3>::Vector( const VecType* Items ) noexcept : Vector<VecType, 2>{ Items }, z( std::move( *( Items + 2 ) ) )
{
}

template <typename VecType>
constexpr Vector<VecType, 3>::Vector( std::initializer_list<VecType>&& Items ) noexcept : Vector<VecType, 2>{ std::move( Items ) },
                                                                                          z( std::move( *( Items.begin() + 2 ) ) )
{
}

template <typename VecType>
constexpr Vector<VecType, 3>::Vector( const Vector<VecType, 3>& Copy ) noexcept : Vector<VecType, 2>{ Copy }, z( Copy.z )
{
}

template <typename VecType>
constexpr Vector<VecType, 3>::Vector( Vector<VecType, 3>&& Move ) noexcept : Vector<VecType, 2>{ std::move( Move ) }, z( std::move( Move.z ) )
{
}

template <typename VecType>
template <typename OtherType>
constexpr Vector<VecType, 3>::Vector( const Vector<OtherType, 3>& Copy ) noexcept : Vector<VecType, 2>{ Copy },
                                                                                    z( static_cast< VecType >( Copy.z ) )
{
}

template <typename VecType>
template <typename OtherType>
constexpr Vector<VecType, 3>::Vector( Vector<OtherType, 3>&& Move ) noexcept : Vector<VecType, 2>{ std::move( Move ) },
                                                                               z( std::move( static_cast< VecType >( Move.z ) ) )
{
}

template <typename VecType>
VecType& Vector<VecType, 3>::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= 3, IndexTooLarge( Index ) );

  return Index < 2 ? Vector<VecType, 2>::operator[]( Index ) : z;
}

template <typename VecType>
constexpr const VecType& Vector<VecType, 3>::operator[]( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= 3, IndexTooLarge( Index ) );

  return Index < 2 ? Vector<VecType, 2>::operator[]( Index ) : z;
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator+=( const Vector<VecType, 3>& Rhs ) noexcept
{
  x += Rhs.x;
  y += Rhs.y;
  z += Rhs.z;

  return *this;
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator+=(const VecType& Rhs) noexcept
{
  x += Rhs;
  y += Rhs;
  z += Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator+(const VecType& Rhs) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return + Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator-=(const VecType& Rhs) noexcept
{
  x -= Rhs;
  y -= Rhs;
  z -= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator-(const VecType& Rhs) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return - Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator*=(const VecType& Rhs) noexcept
{
  x *= Rhs;
  y *= Rhs;
  z *= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator*(const VecType& Rhs) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return * Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator/=(const VecType& Rhs) noexcept
{
  x /= Rhs;
  y /= Rhs;
  z /= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator/(const VecType& Rhs) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return / Rhs;

  return Return; // Named RVO
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator+( const Vector<VecType, 3>& Rhs ) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return += Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator-=( const Vector<VecType, 3>& Rhs ) noexcept
{
  x -= Rhs.x;
  y -= Rhs.y;
  z -= Rhs.z;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator-( const Vector<VecType, 3>& Rhs ) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return -= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator*=( const Vector<VecType, 3>& Rhs ) noexcept
{
  x *= Rhs.x;
  y *= Rhs.y;
  z *= Rhs.z;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator*( const Vector<VecType, 3>& Rhs ) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return *= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator/=( const Vector<VecType, 3>& Rhs ) noexcept
{
  x /= Rhs.x;
  y /= Rhs.y;
  z /= Rhs.z;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 3> Vector<VecType, 3>::operator/( const Vector<VecType, 3>& Rhs ) const noexcept
{
  Vector<VecType, 3> Return{ *this };

  Return /= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator=( const Vector<VecType, 3>& Copy ) noexcept
{
  x = Copy.x;
  y = Copy.y;
  z = Copy.z;

  return *this;
}

template <typename VecType>
Vector<VecType, 3>& Vector<VecType, 3>::operator=( Vector<VecType, 3>&& Move ) noexcept
{
  x = std::move( Move.x );
  y = std::move( Move.y );
  z = std::move( Move.z );

  return *this;
}

template <typename VecType>
template <typename NewType>
constexpr Vector<VecType, 3>::operator Vector<NewType, 3>() const noexcept
{
  return Vector<NewType, 3>{ *this };
}

template< typename VecType >
std::ostream & operator<<( std::ostream &Lhs, const Vector< VecType, 3 > &Rhs ) noexcept
{
  return Lhs << Rhs.x << ", " << Rhs.y << ", " << Rhs.z;
}

template <typename VecType>
bool Vector<VecType, 3>::operator==( const Vector<VecType, 3>& Rhs ) const noexcept
{
  return x == Rhs.x && y == Rhs.y && z == Rhs.z;
}

template <typename VecType>
Vector<VecType, 3>::IndexTooLarge::IndexTooLarge( size_t Index ) noexcept
{
  std::cerr << "Vector Index: " << Index << ", Vector Size: 3" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

template <typename VecType>
constexpr Vector<VecType, 2>::Vector( const VecType &Initializer ) noexcept : x( Initializer ), y( Initializer )
{
}

template< typename VecType >
constexpr Vector< VecType, 2 >::Vector( const VecType *Items ) noexcept : x( std::move( *Items ) ), y( std::move( *( Items + 1 ) ) )
{
}

template< typename VecType >
constexpr Vector< VecType, 2 >::Vector( std::initializer_list< VecType > &&Items ) noexcept : x( std::move( *( Items.begin() ) ) ),
                                                                                              y( std::move( *( Items.begin() + 1 ) ) )
{
}

template< typename VecType >
constexpr Vector< VecType, 2 >::Vector( const Vector< VecType, 2 > &Copy ) noexcept : x( Copy.x ), y( Copy.y )
{
}

template< typename VecType >
constexpr Vector< VecType, 2 >::Vector( Vector< VecType, 2 > &&Move ) noexcept : x( std::move( Move.x ) ), y( std::move( Move.y ) )
{
}

template <typename VecType>
template <typename OtherType>
constexpr Vector<VecType, 2>::Vector( const Vector<OtherType, 2>& Copy ) noexcept : x( static_cast< VecType >( Copy.x ) ),
                                                                                    y( static_cast< VecType >( Copy.y ) )
{
}

template <typename VecType>
template <typename OtherType>
constexpr Vector<VecType, 2>::Vector( Vector<OtherType, 2>&& Move ) noexcept : x( std::move( static_cast< VecType >( Move.x ) ) ),
                                                                               y( std::move( static_cast< VecType >( Move.y ) ) )
{
}

template< typename VecType >
VecType & Vector< VecType, 2 >::operator[]( size_t Index ) ExceptDebug
{
  AssertDebug( Index >= 2, IndexTooLarge( Index ) );

  return Index ? y : x;
}

template< typename VecType >
constexpr const VecType & Vector< VecType, 2 >::operator[]( size_t Index ) const ExceptDebug
{
  AssertDebug( Index >= 2, IndexTooLarge( Index ) );

  return Index ? y : x;
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator+=( const Vector<VecType, 2>& Rhs ) noexcept
{
  x += Rhs.x;
  y += Rhs.y;

  return *this;
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator+=(const VecType& Rhs) noexcept
{
  x += Rhs;
  y += Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator+( const Vector<VecType, 2>& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return += Rhs;

  return Return; // Named RVO
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator+( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return + Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator-=( const Vector<VecType, 2>& Rhs ) noexcept
{
  x -= Rhs.x;
  y -= Rhs.y;

  return *this;
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator-=( const VecType& Rhs ) noexcept
{
  x -= Rhs;
  y -= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator-( const Vector<VecType, 2>& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return -= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator-( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return - Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator*=( const Vector<VecType, 2>& Rhs ) noexcept
{
  x *= Rhs.x;
  y *= Rhs.y;

  return *this;
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator*=( const VecType& Rhs ) noexcept
{
  x *= Rhs;
  y *= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator*( const Vector<VecType, 2>& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return *= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator*( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return *= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator/=( const Vector<VecType, 2>& Rhs ) noexcept
{
  x /= Rhs.x;
  y /= Rhs.y;

  return *this;
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator/=( const VecType& Rhs ) noexcept
{
  x /= Rhs;
  y /= Rhs;

  return *this;
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator/( const Vector<VecType, 2>& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return /= Rhs;

  return Return; // Named RVO
}

template <typename VecType>
constexpr const Vector<VecType, 2> Vector<VecType, 2>::operator/( const VecType& Rhs ) const noexcept
{
  Vector<VecType, 2> Return{ *this };

  Return / Rhs;

  return Return; // Named RVO
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator=( const Vector<VecType, 2>& Copy ) noexcept
{
  x = Copy.x;
  y = Copy.y;

  return *this;
}

template <typename VecType>
Vector<VecType, 2>& Vector<VecType, 2>::operator=( Vector<VecType, 2>&& Move ) noexcept
{
  x = std::move( Move.x );
  y = std::move( Move.y );

  return *this;
}

template <typename VecType>
template <typename NewType>
constexpr Vector<VecType, 2>::operator Vector<NewType, 2>() const noexcept
{
  return Vector<NewType, 2>{ *this };
}

template <typename VecType>
bool Vector<VecType, 2>::operator==( const Vector<VecType, 2>& Rhs ) const noexcept
{
  return x == Rhs.x && y == Rhs.y;
}

template< typename VecType >
std::ostream & operator<<( std::ostream &Lhs, const Vector< VecType, 2 > &Rhs ) noexcept
{
  return Lhs << Rhs.x << ", " << Rhs.y;
}

template < typename VecType >
Vector< VecType, 2 >::IndexTooLarge::IndexTooLarge( size_t Index ) noexcept
{
  std::cerr << "Vector Index: " << Index << ", Vector Size: 2" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

#include <type_traits>

template< typename Type >
using UnrefedType = typename std::_Unrefwrap< Type >::type;

template< typename Type, size_t VecSize >
using UnrefedVector = Vector< UnrefedType< Type >, VecSize >;

template< typename Type, size_t Size >
constexpr auto MakeVector( const Type( &&Items )[ Size ] ) noexcept
{
  return UnrefedVector< Type, Size >{ Items };
}
