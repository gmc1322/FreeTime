/*!----------------------------------------------------------------------------
\file   DynamicMap.h

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

template< typename Type >
using CType = std::remove_pointer_t< std::remove_const_t< Type > >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename TemplateType >
struct GetBaseType;

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

template< typename TemplateType >
struct GetBaseType
{
  using Type = TemplateType;
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

template< typename Type >
using IsTypeSmaller = std::bool_constant < sizeof( Type ) < sizeof( void* ) > ; // Smaller then a ptr

template< typename Type >
using IsTypePtr = std::bool_constant< std::is_pointer< Type >::value >;

template< typename Type >
using IsTypeSmall = std::bool_constant< IsTypeSmaller< Type >::value || sizeof( Type ) == sizeof( void* ) >; // Smaller then or equal to a ptr

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type >
using IsTypeMemsetAble = std::bool_constant< std::is_arithmetic< Type >::value && sizeof( Type ) <= sizeof( char ) >;

template< typename Type >
using IsTypeTrivialCopyAble = std::bool_constant< std::is_trivially_copyable< Type >::value && IsTypeSmall< Type >::value >;

template< typename Type >
using IsTypeTrivial = std::is_trivial< Type >;

template< typename Type >
using IsTypeMemcpyAble = std::bool_constant< std::is_trivially_copyable< Type >::value && IsTypePtr< Type >::value >;

template< typename Type >
using IsTypeMoveAble = std::is_move_constructible< Type >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type >
using FuncArgType = TemplateIf< IsTypeSmaller< Type >::value, const CType< Type >,
                                TemplateIf< IsTypePtr< Type >::value, CType< Type > *const,
                                            TemplateIf< IsTypeMoveAble< Type >::value, CType< Type > &&, const CType< Type > & > > >;
template< typename Type >
using FuncConstArgType = TemplateIf< IsTypeSmaller< Type >::value, const CType< Type >,
                                     TemplateIf< IsTypePtr< Type >::value, const CType< Type > *const,
                                                 TemplateIf< IsTypeMoveAble< Type >::value, CType< Type >, const CType< Type > & > > >;
template< typename Type >
using FuncConstArgType_WithMove = TemplateIf< IsTypeSmaller< Type >::value, const CType< Type >,
                                              TemplateIf< IsTypePtr< Type >::value, const CType< Type > *const,
                                                          TemplateIf< IsTypeMoveAble< Type >::value, CType< Type > &&, const CType< Type > & > > >;
template< typename Type >
using FuncConstArgType_NoMove = TemplateIf< IsTypeSmaller< Type >::value, const CType< Type >,
                                            TemplateIf< IsTypePtr< Type >::value, const CType< Type > *const, const CType< Type > & > >;
template< typename Type >
using FuncReturnType = TemplateIf< IsTypeSmaller< Type >::value, CType< Type >, const Type & >;

template< typename Type, unsigned char Index >
using FuncReturnType_Explicit = TemplateIf< Index == 0, CType< Type >,
                                            TemplateIf< Index == 1, const Type &, 
                                                        TemplateIf< Index == 2, CType< Type > &&, CType< Type > * > > >;

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
  using MemberFuncType = TemplateIf< std::is_const< ClassType >::value, ReturnType( ClassType::* )()const, ReturnType( ClassType::* )() >;

  PtrToMemberFunction( const MemberFuncType MemberFunc ) NoExcept : MemberFunc( MemberFunc ) {}

  const MemberFuncType MemberFunc;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class SetFuncTypes : unsigned char
{
  Memset,

  TrivialCopy,

  Memcpy,

  Move,

  Copy,
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< SetFuncTypes FuncType, typename MemsetFuncType, typename TrivialFuncType, typename MemcpyFuncType, typename MoveFuncType, typename CopyFuncType >
struct RunSetFuncFunction;

template< typename MemsetFuncType, typename TrivialFuncType, typename MemcpyFuncType, typename MoveFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::Memset, MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType >
{
  RunSetFuncFunction( MemsetFuncType MemsetFunc, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType ) NoExcept :
  RunFunc( MemsetFunc ) {}

  const MemsetFuncType RunFunc;
};

template< typename MemsetFuncType, typename TrivialFuncType, typename MemcpyFuncType, typename MoveFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::TrivialCopy, MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType >
{
  RunSetFuncFunction( MemsetFuncType, TrivialFuncType TrivialFunc, MemcpyFuncType, MoveFuncType, CopyFuncType ) NoExcept :
  RunFunc( TrivialFunc ) {}

  const TrivialFuncType RunFunc;
};

template< typename MemsetFuncType, typename TrivialFuncType, typename MemcpyFuncType, typename MoveFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::Memcpy, MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType >
{
  RunSetFuncFunction( MemsetFuncType, TrivialFuncType, MemcpyFuncType MemcpyFunc, MoveFuncType, CopyFuncType ) NoExcept :
  RunFunc( MemcpyFunc ) {}

  const MemcpyFuncType RunFunc;
};

template< typename MemsetFuncType, typename TrivialFuncType, typename MemcpyFuncType, typename MoveFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::Move, MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType >
{
  RunSetFuncFunction( MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType MoveFunc, CopyFuncType ) NoExcept :
  RunFunc( MoveFunc ) {}

  const MoveFuncType RunFunc;
};

template< typename MemsetFuncType, typename TrivialFuncType, typename MemcpyFuncType, typename MoveFuncType, typename CopyFuncType >
struct RunSetFuncFunction< SetFuncTypes::Copy, MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType >
{
  RunSetFuncFunction( MemsetFuncType, TrivialFuncType, MemcpyFuncType, MoveFuncType, CopyFuncType CopyFunc ) NoExcept :
  RunFunc( CopyFunc ) {}

  const CopyFuncType RunFunc;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type >
using SetFuncType = std::integral_constant< SetFuncTypes, IsTypeMemsetAble< Type >::value ? SetFuncTypes::Memset :
                                                          ( IsTypeTrivialCopyAble< Type >::value ? SetFuncTypes::TrivialCopy :
                                                            ( IsTypeMemcpyAble< Type >::value ? SetFuncTypes::Memcpy :
                                                              ( IsTypeMoveAble< Type >::value ? SetFuncTypes::Move : SetFuncTypes::Copy ) ) ) >;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum class GetFuncTypes : unsigned char
{
  Trivial,
  Ref,

  // Non-const Types

  Move, // Moves data out of container
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename Type >
using GetFuncType = std::integral_constant< GetFuncTypes, ( IsTypeMemsetAble< Type >::value || IsTypeTrivialCopyAble< Type >::value ) ?
                                                          GetFuncTypes::Trivial : GetFuncTypes::Ref >;
