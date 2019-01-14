
#include "Public/Utils/MemoryManager.h"

#include <iostream>
#include <vector>

/*!------------------------------------------------------------------------------
\file   BaseMutliDataContainer.h

\author Garrett Conti

\brief  Project: Golems_Magic
         Course: GAM300

\par    COPYRIGHT © 2018 BY DIGIPEN CORP, USA. ALL RIGHTS RESERVED.
---------------------------------------------------------------------------- */

#pragma once

#include "Public/Utils/Macros.h"

#include "Public/Utils/TemplateTypeHelpers.h"

#include <vector>

template< typename ... DataTypes >
class BaseMultiDataContainer;

template< typename OurDataType >
class BaseMultiDataContainer< OurDataType >
{
  public:
  using PrivateChildType = void;

  public:
  void EmplaceData_Trivial( const OurDataType Data ) NoExcept { DataContainer.emplace_back( Data ); }
  void EmplaceData_Trivial( const OurDataType *const Data ) NoExcept { DataContainer.emplace_back( *Data ); }
  void EmplaceData_Move( OurDataType &&Data ) NoExcept { DataContainer.emplace_back( std::move( Data ) ); }
  void EmplaceData_Copy( const OurDataType &Data ) NoExcept { DataContainer.emplace_back( Data ); }

  void EraseData( const size_t Index ) NoExcept
  {
    DebugAssert( Size() == 0, "There is nothing to Erase!", return );
    DebugAssert( Index >= Size(), "The Index %llu is out of bounds!", return, Index );

    EraseData_Forced( Index );
  }

  void EraseData_Forced( const size_t Index ) NoExcept
  {
    DebugAssert( Size() == 0, "There is nothing to Erase!", return );
    DebugAssert( Index >= Size(), "The Index %llu is out of bounds!", return, Index );

    DataContainer.erase( DataContainer.begin() + Index );
  }

  void PopBackData() NoExcept { DebugAssert( Size() == 0, "There is nothing to Pop off!", return ); DataContainer.pop_back(); }
  void PopFrontData() NoExcept
  {
    DebugAssert( Size() == 1, "Use PopBackData instead!", );

    PopFrontData_Forced();
  }

  void PopFrontData_Forced() NoExcept
  {
    DebugAssert( Size() == 0, "There is nothing to Pop off!", return );

    DataContainer.erase( DataContainer.begin() );
  }

  public:
  size_t Size() const NoExcept { return DataContainer.size(); }

  public:
  OurDataType GetData_Trivial() const NoExcept { return DataContainer.front(); }
  const OurDataType & GetData_Ref() const NoExcept { return DataContainer.front(); }

  OurDataType GetData_Trivial( const size_t Index ) const NoExcept
  {
    DebugAssert( Index >= Size(), "The Index %llu is out of bounds!", return OurDataType{}, Index );
    DebugAssert( !Index, "Use the no-Index GetData!", );

    return DataContainer[ Index ];
  }

  const OurDataType & GetData_Ref( const size_t Index ) const NoExcept
  {
    DebugAssert( Index >= Size(), "The Index %llu is out of bounds!", , Index );
    DebugAssert( !Index, "Use the no-Index GetData!", );

    return DataContainer[ Index ];
  }

  public:
  OurDataType && GetData_Move() NoExcept { return std::move( DataContainer.front() ); }
  OurDataType * GetData_Ptr() NoExcept { return &( DataContainer.front() ); }

  OurDataType && GetData_Move( const size_t Index ) NoExcept
  {
    DebugAssert( Index >= Size(), "The Index %llu is out of bounds!", return std::move( OurDataType{} ), Index );
    DebugAssert( !Index, "Use the no-Index GetData!", );

    return std::move( DataContainer[ Index ] );
  }

  OurDataType * GetData_Ptr( const size_t Index ) NoExcept
  {
    DebugAssert( Index >= Size(), "The Index %llu is out of bounds!", return nullptr, Index );
    DebugAssert( !Index, "Use the no-Index GetData!", );

    return &( DataContainer[ Index ] );
  }

  public:
  void Reserve( const size_t Size ) NoExcept { DataContainer.reserve( Size ); }
  void Clear() NoExcept { DataContainer.clear(); }

  private:
  std::vector< OurDataType > DataContainer;
};

template< typename OurDataType, typename ... ChildDataTypes >
class BaseMultiDataContainer< OurDataType, ChildDataTypes... > : public BaseMultiDataContainer< OurDataType >,
  private BaseMultiDataContainer< ChildDataTypes... >
{
  StaticAssert( !( AreTypesUnique< OurDataType, ChildDataTypes... >::IsUnique ), "MultiDataContainers cannot have duplicate types!" );

  using PrivateChildType = BaseMultiDataContainer< ChildDataTypes... >;
};

struct Test
{
  Test() = default;
  Test( size_t Val ) : One( Val ) {};

  Test( const Test &Copy ) NoExcept : One( Copy.One ) {};
  Test( Test &&Move ) = default;

  Test& operator=( Test &&Move ) = default;

  size_t One = 10;
  void *Two = nullptr;
  double Three = 0.0;
};

int main()
{
  BaseMultiDataContainer< int > tesert;

  tesert.EmplaceData_Trivial( 10 );
  tesert.EmplaceData_Trivial( 9 );

  auto &TEst1 = tesert.GetData_Ref();
  auto &Test2 = tesert.GetData_Ref( 1 );

  std::cout << TEst1 << " " << Test2 << std::endl;

  {
    MemoryManager MemManager{ 15, sizeof( int ) };

    Test Testing{};

    auto &FirstPtr = MemManager.Allocate_Init< Test >( std::move( Testing ) );

    std::cout << FirstPtr.GetData_Unsafe().One << std::endl;

    FirstPtr.SetData_Trivial( 256 );

    std::cout << FirstPtr.GetData_Unsafe().One << std::endl;

    auto &SecondPtr = MemManager.Allocate< size_t >( 2, 512 );

    std::cout << SecondPtr.GetData( 1 ) << std::endl;

    ///////////////////////////////////////////////////////////////////////////

    std::vector< MemoryManager::MemoryPtr* > Ptrs;

    Ptrs.reserve( 12 );

    for( Size_T i = 0; i < 12; ++i )
    {
      Ptrs.emplace_back( &( MemManager.Allocate() ) );
    }

    MemManager.Free( FirstPtr );

    for( Size_T i = 0; i < 6; ++i )
    {
      MemManager.Free( Ptrs[ i ] );

      Ptrs.erase( Ptrs.begin() + i );
    }

    ///////////////////////////////////////////////////////////////////////////

    MemManager.Free( Ptrs.front() ); // Should compress
    Ptrs.erase( Ptrs.begin() );

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    Ptrs.emplace_back( &( MemManager.Allocate( sizeof( int ) * 5 ) ) ); // Should grow

    std::cout << SecondPtr.GetData()    << std::endl;
    std::cout << SecondPtr.GetData( 1 ) << std::endl;

    Ptrs.emplace_back( &( MemManager.Allocate() ) );                    // Should replace ( 4) free slot
    Ptrs.emplace_back( &( MemManager.Allocate( sizeof( int ) * 3 ) ) ); // Should replace (12) free slot
    Ptrs.emplace_back( &( MemManager.Allocate( sizeof( short ) ) ) );   // Should replace ( 2) free slot

    {
      Ptrs.back()->SetData_Trivial( static_cast< unsigned char >( 0 ) );

      ( *( Ptrs.back() ) + 1u )->SetData_Trivial( static_cast< unsigned char >( 10 ) );

      std::cout << Ptrs.back()->GetData< short >() << std::endl;
    }

    MemManager.Free( SecondPtr );

    for( auto *Iter : Ptrs )
    {
      MemManager.Free( Iter );
    }
  }

  return 0;
}