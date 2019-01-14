#pragma once

#include <type_traits>

#define NoExcept noexcept

template< size_t ArraySize, size_t ... Initializers >
struct FastSwitch;

template< size_t ArraySize >
struct FastSwitch< ArraySize >{ constexpr FastSwitch() NoExcept = default; };

template< size_t ArraySize, size_t FirstInitializerValue, size_t FirstInitializerIndex >
struct FastSwitch< ArraySize, FirstInitializerValue, FirstInitializerIndex > :
  private std::conditional_t< ArraySize - 1 == FirstInitializerIndex,
                              FastSwitch< 0 >,
                              FastSwitch< ArraySize - 1, FirstInitializerValue, FirstInitializerIndex > >
{
  constexpr FastSwitch() NoExcept = default;

  const size_t Element = ( ArraySize - 1 == FirstInitializerIndex ? FirstInitializerValue : -1 );
};

template< size_t ArraySize, size_t LastInitializerValue, size_t LastInitializerIndex, size_t ... Initializers >
struct FastSwitch< ArraySize, LastInitializerValue, LastInitializerIndex, Initializers... > final :
  private std::conditional_t< ArraySize - 1 == LastInitializerIndex,
                              FastSwitch< ArraySize - 1, Initializers... >,
                              FastSwitch< ArraySize - 1, LastInitializerValue, LastInitializerIndex, Initializers... > >
{
  constexpr FastSwitch() NoExcept = default;

  constexpr size_t operator[]( const size_t Index ) const NoExcept
  {
    //DebugAssert( Index >= ArraySize, "Went out of bounds!", return 0 );
    
    return *( &Element - ( ArraySize - Index - 1 ) );
  }

  const size_t Element = ( ArraySize - 1 == LastInitializerIndex ? LastInitializerValue : -1 );
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template< typename ... SwitchValues >
struct TestSwitch;

template<>
struct TestSwitch<> { constexpr TestSwitch() NoExcept = default; };

template< size_t ArraySize, size_t FirstInitializerValue, size_t FirstInitializerIndex >
struct TestSwitch< ArraySize, FirstInitializerValue, FirstInitializerIndex > :
  private std::conditional_t< ArraySize - 1 == FirstInitializerIndex,
  TestSwitch< 0 >,
  TestSwitch< ArraySize - 1, FirstInitializerValue, FirstInitializerIndex > >
{
  constexpr TestSwitch() NoExcept = default;

  const size_t Element = ( ArraySize - 1 == FirstInitializerIndex ? FirstInitializerValue : -1 );
};

template< typename ... SwitchValues >
struct TestSwitch< ArraySize, LastInitializerValue, LastInitializerIndex, Initializers... > final :
  private std::conditional_t< ArraySize - 1 == LastInitializerIndex,
  TestSwitch< ArraySize - 1, Initializers... >,
  TestSwitch< ArraySize - 1, LastInitializerValue, LastInitializerIndex, Initializers... > >
{
  constexpr TestSwitch() NoExcept = default;

  constexpr size_t operator[]( const size_t Index ) const NoExcept
  {
    //DebugAssert( Index >= ArraySize, "Went out of bounds!", return 0 );

    return *( &Element - ( ArraySize - Index - 1 ) );
  }

  const size_t Element = ( ArraySize - 1 == LastInitializerIndex ? LastInitializerValue : -1 );
};
