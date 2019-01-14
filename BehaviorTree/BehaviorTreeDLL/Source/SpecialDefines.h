#pragma once

#ifndef SpecialDefines
#define SpecialDefines

#ifdef BehaviorTreeDLL_EXPORTS
#define BehaviorTreeDLL_API __declspec(dllexport)   
#else

#ifdef BehaviorTreeDLL_DEBUG
#define BehaviorTreeDLL_API
#else
#define BehaviorTreeDLL_API __declspec(dllimport)   
#endif

#endif

#ifdef NoThrow
#define Throwing( T )
#define ThrowingElse( T, ET ) ET
#else
#define Throwing( T ) T
#define ThrowingElse( T, ET ) T
#endif

#endif