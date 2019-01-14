/*!----------------------------------------------------------------------------
\file   TempalteTypeHelpers.h

\author Garrett Conti

\brief  Project: Golems_Magic
         Course: GAM300

\par    COPYRIGHT © 2018 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
---------------------------------------------------------------------------- */

#pragma once

#include <type_traits>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< bool TrueFalse, typename T1, typename T2 >
using TemplateIf = typename std::conditional< TrueFalse, T1, T2 >::type;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename TemplateType >
struct GetBaseType
{
  using Type = TemplateType;
};

template< typename TemplateType >
struct GetBaseType< TemplateType * >
{
  using Type = typename GetBaseType< typename std::remove_pointer< TemplateType >::type >::Type;
};

template< typename TemplateType >
struct GetBaseType< TemplateType & >
{
  using Type = typename GetBaseType< typename std::remove_reference< TemplateType >::type >::Type;
};

template< typename TemplateType >
struct GetBaseType< TemplateType && >
{
  using Type = typename GetBaseType< typename std::remove_reference< TemplateType >::type >::Type;
};

template< typename TemplateType >
struct GetBaseType< const TemplateType >
{
  using Type = typename GetBaseType< typename std::remove_cv< TemplateType >::type >::Type;
};

template< typename TemplateType >
struct GetBaseType< volatile TemplateType >
{
  using Type = typename GetBaseType< typename std::remove_cv< TemplateType >::type >::Type;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type1, typename ... Types >
struct GetLargestType
{
  using Type = TemplateIf < sizeof( Type1 ) < GetLargestType< Types... >::Size, typename GetLargestType< Types... >::Type, Type1 > ;

  static constexpr size_t Size = sizeof( Type );
};

template< typename Type1, typename Type2 >
struct GetLargestType< Type1, Type2 >
{
  using Type = TemplateIf < sizeof( Type1 ) < sizeof( Type2 ), Type2, Type1 > ;

  static constexpr size_t Size = sizeof( Type );
};

// Handle only 1 type
template< typename Type >
struct GetLargestType< Type >
{
  static constexpr size_t Size = 1;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type, typename Check, typename... OtherChecks >
struct IsContainedIn
{
  static constexpr bool IsContained = std::is_same< Type, Check >::value || IsContainedIn< Type, OtherChecks... >::IsContained;
};

template< typename Type, typename Check >
struct IsContainedIn< Type, Check >
{
  static constexpr bool IsContained = std::is_same< Type, Check >::value;
};

template< typename Type, typename... Checks >
struct AreTypesUnique
{
  static constexpr bool IsUnique = AreTypesUnique< Checks... >::IsUnique && !( IsContainedIn< Type, Checks... >::IsContained );
};

template< typename Type1, typename Type2 >
struct AreTypesUnique< Type1, Type2 >
{
  static constexpr bool IsUnique = !( IsContainedIn< Type1, Type2 >::IsContained );
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type >
using IsTypeSmall = std::bool_constant< sizeof( Type ) <= sizeof( void* ) >; // Smaller then or equal to a ptr

template< typename Type >
using IsTypeMemsetAble = std::bool_constant< sizeof( Type ) <= sizeof( char ) >;

template< typename Type >
using IsTypeMemcpyAble = std::is_trivial< Type >;

template< typename Type >
using IsTypeMoveAble = std::bool_constant< std::is_move_constructible< Type >::value && !( std::is_trivially_move_constructible< Type >::value ) >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type >
using FuncArgType = TemplateIf< IsTypeSmall< Type >::value, const Type, const Type & >;

template< typename Type >
using FuncReturnType = TemplateIf< IsTypeSmall< Type >::value, const Type, const Type & >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename ReturnType, typename ClassType, typename... ArgTypes >
struct PtrToMemberFunction
{
  using MemberFuncType = TemplateIf< std::is_const< ClassType >::value, ReturnType( ClassType::* )( ArgTypes... )const,
                                     ReturnType( ClassType::* )( ArgTypes... ) >;

  PtrToMemberFunction( const MemberFuncType MemberFunc ) NoExcept : MemberFunc( MemberFunc ) {}

  const MemberFuncType MemberFunc;
};

template< typename ReturnType, typename ClassType >
struct PtrToMemberFunction< ReturnType, ClassType, void >
{
  using MemberFuncType = TemplateIf< std::is_const< ClassType >::value, ReturnType( ClassType::* )( )const, ReturnType( ClassType::* )( ) >;

  PtrToMemberFunction( const MemberFuncType MemberFunc ) NoExcept : MemberFunc( MemberFunc ) {}

  const MemberFuncType MemberFunc;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class SetFuncTypes : unsigned char
{
  Trivial, // No Ref needed
  Copy,    // Ref needed

  // Array Types, usualy only need for arrays with multiple elements, otherwise it is slower then a single copy.

  Memset, // Used to say: Set  array elements x->y to z    (IE. A[1]->A[5] = 10         when array is size 8)
  Memcpy, // Used to say: Copy array elements x->y to z->w (IE. A[1]->A[5] = B[2]->B[6] when both are size 8)

  // Explicit Types, requires user to explicitly say they want it.

  Move, // Requires explicit std::move call or it decays to a ref
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< SetFuncTypes FuncType, typename TrivialFuncType, typename CopyFuncType >
struct RunSetFuncFunction;

template< typename TrivialFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::Trivial, TrivialFuncType, CopyFuncType >
{
  RunSetFuncFunction( TrivialFuncType TrivialFunc, CopyFuncType ) NoExcept : RunFunc( TrivialFunc ) {}

  const TrivialFuncType RunFunc;
};

template< typename TrivialFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::Copy, TrivialFuncType, CopyFuncType >
{
  RunSetFuncFunction( TrivialFuncType, CopyFuncType CopyFunc ) NoExcept : RunFunc( CopyFunc ) {}

  const CopyFuncType RunFunc;
};

template< typename Type >
using SetFuncType = std::integral_constant< SetFuncTypes, IsTypeSmall< Type >::value ? SetFuncTypes::Trivial : SetFuncTypes::Copy >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< SetFuncTypes FuncType, typename MemsetFuncType, typename TrivialFuncType, typename CopyFuncType >
struct RunMemsetFuncFunction;

template< typename MemsetFuncType, typename TrivialFuncType, typename CopyFuncType  >
struct RunMemsetFuncFunction< SetFuncTypes::Memset, MemsetFuncType, TrivialFuncType, CopyFuncType >
{
  RunMemsetFuncFunction( MemsetFuncType MemsetFunc, TrivialFuncType, CopyFuncType ) NoExcept : RunFunc( MemsetFunc ) {}

  const MemsetFuncType RunFunc;
};

template< typename MemsetFuncType, typename TrivialFuncType, typename CopyFuncType  >
struct RunMemsetFuncFunction< SetFuncTypes::Trivial, MemsetFuncType, TrivialFuncType, CopyFuncType >
{
  RunMemsetFuncFunction( MemsetFuncType , TrivialFuncType TrivialFunc, CopyFuncType ) NoExcept : RunFunc( TrivialFunc ) {}

  const TrivialFuncType RunFunc;
};

template< typename MemsetFuncType, typename TrivialFuncType, typename CopyFuncType  >
struct RunMemsetFuncFunction< SetFuncTypes::Copy, MemsetFuncType, TrivialFuncType, CopyFuncType >
{
  RunMemsetFuncFunction( MemsetFuncType, TrivialFuncType, CopyFuncType CopyFunc ) NoExcept : RunFunc( CopyFunc ) {}

  const CopyFuncType RunFunc;
};

template< typename Type >
using MemsetFuncType = std::integral_constant< SetFuncTypes, ( IsTypeMemsetAble< Type >::value ? SetFuncTypes::Memset :
                                                               ( IsTypeSmall< Type >::value ? SetFuncTypes::Trivial : SetFuncTypes::Copy ) ) >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< SetFuncTypes FuncType, typename MemcpyFuncType, typename CopyFuncType >
struct RunMemcpyFuncFunction;

template< typename MemcpyFuncType, typename CopyFuncType >
struct RunMemcpyFuncFunction< SetFuncTypes::Memcpy, MemcpyFuncType, CopyFuncType >
{
  RunMemcpyFuncFunction( MemcpyFuncType MemcpyFunc, CopyFuncType ) NoExcept : RunFunc( MemcpyFunc ) {}

  const MemcpyFuncType RunFunc;
};

template< typename MemcpyFuncType,  typename CopyFuncType  >
struct RunMemcpyFuncFunction< SetFuncTypes::Copy, MemcpyFuncType, CopyFuncType >
{
  RunMemcpyFuncFunction( MemcpyFuncType, CopyFuncType CopyFunc ) NoExcept : RunFunc( CopyFunc ) {}

  const CopyFuncType RunFunc;
};

template< typename Type >
using MemcpyFuncType = std::integral_constant< SetFuncTypes, IsTypeMemcpyAble< Type >::value ? SetFuncTypes::Memcpy : SetFuncTypes::Copy >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class GetFuncTypes : unsigned char
{
  Trivial,
  Ref,

  // Non-const Types
  Move, // Moves data out of the container
  Ptr   // Allows non-const access
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< GetFuncTypes FuncType, typename TrivialFuncType, typename RefFuncType >
struct RunGetFuncFunction;

template< typename TrivialFuncType, typename RefFuncType >
struct RunGetFuncFunction< GetFuncTypes::Trivial, TrivialFuncType, RefFuncType >
{
  RunGetFuncFunction( TrivialFuncType TrivialFunc, RefFuncType ) NoExcept : RunFunc( TrivialFunc ) {}

  const TrivialFuncType RunFunc;
};

template< typename TrivialFuncType, typename RefFuncType >
struct RunGetFuncFunction< GetFuncTypes::Ref, TrivialFuncType, RefFuncType >
{
  RunGetFuncFunction( TrivialFuncType, RefFuncType RefFunc ) NoExcept : RunFunc( RefFunc ) {}

  const RefFuncType RunFunc;
};

template< typename Type >
using GetFuncType = std::integral_constant< GetFuncTypes, IsTypeSmall< Type >::value ? GetFuncTypes::Trivial : GetFuncTypes::Ref >;
