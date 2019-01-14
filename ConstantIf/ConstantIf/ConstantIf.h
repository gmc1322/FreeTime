#pragma once

#define ConstIf( TF ) if constexpr( TF )
#define ConstElseIf( TF ) else if constexpr( TF )

#define IfDebug   ConstIf( _DEBUG )
#define IfRelease ConstIf( NDEBUG )

#define ElseIfDebug   ConstElseIf( _DEBUG )
#define ElseIfRelease ConstElseIf( NDEBUG )
