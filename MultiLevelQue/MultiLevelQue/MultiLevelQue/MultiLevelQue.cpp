#include "MultiLevelQue.h"

#include <iostream>

namespace std
{
  std::string to_string( char Character ) noexcept
  {
    return std::string{ Character };
  }

  std::string to_string( const std::string &String ) noexcept
  {
    return String;
  }
}

template< typename KeyType, typename ... OtherKeyTypes >
QueRange< KeyType, OtherKeyTypes... >::QueRange( const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept :
Key{ Key }, Others{ OtherKeys... }
{
}

template< typename KeyType, typename ... OtherKeyTypes >
QueRange< KeyType, OtherKeyTypes... >::QueRange( KeyType &&Key, const OtherKeyTypes &&... OtherKeys ) noexcept :
Key{ std::move( Key ) }, Others{ std::move( OtherKeys )... }
{
}

template< typename KeyType, typename ... OtherKeyTypes >
KeyType && QueRange< KeyType, OtherKeyTypes... >::GetKey() noexcept
{
  return std::move( Key );
}

template< typename KeyType, typename ... OtherKeyTypes >
QueRange< OtherKeyTypes... > && QueRange< KeyType, OtherKeyTypes... >::GetOthers() noexcept
{
  return std::move( Others );
}

template< typename KeyType, typename ... OtherKeyTypes >
bool QueRange< KeyType, OtherKeyTypes... >::operator!=( const QueRange &Rhs ) const noexcept
{
  return Key != Rhs.Key || Others != Rhs.Others;
}

template< typename KeyType, typename ... OtherKeyTypes >
void QueRange< KeyType, OtherKeyTypes... >::operator++() noexcept
{
  ++Others;
}

template< typename KeyType, typename ... OtherKeyTypes >
const KeyType & QueRange< KeyType, OtherKeyTypes... >::GetKey() const noexcept
{
  return Key;
}

template<typename KeyType, typename ... OtherKeyTypes >
const QueRange< OtherKeyTypes... > & QueRange< KeyType, OtherKeyTypes... >::GetOthers() const noexcept
{
  return Others;
}

template< typename KeyType >
QueRange< KeyType >::QueRange( const KeyType &Key ) noexcept : Key{ Key }
{
}

template< typename KeyType >
QueRange< KeyType >::QueRange( KeyType &&Key ) noexcept : Key{ std::move( Key ) }
{
}

template< typename KeyType >
KeyType && QueRange< KeyType >::GetKey() noexcept
{
  return std::move( Key );
}

template< typename KeyType >
bool QueRange< KeyType >::operator!=( const QueRange &Rhs ) const noexcept
{
  return Key != Rhs.Key;
}

template< typename KeyType >
void QueRange< KeyType >::operator++() noexcept
{
  ++Key;
}

template< typename KeyType >
const KeyType & QueRange< KeyType >::GetKey() const noexcept
{
  return Key;
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::CouldNotFind::CouldNotFind( const KeyType &Key ) noexcept :
What{ "Could not find the key: " + std::to_string( Key ) + "!" }
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::CouldNotFind::CouldNotFind( std::string &&String ) noexcept :
What{ std::move( String ) }
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueIsEmpty::QueIsEmpty() noexcept
{
  std::cerr << "Cant remove from the que, its empty!" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::MultiLevelQue( const DataType &Data, const KeyType &Key, 
                                                                     const OtherKeyTypes &... OtherKeys ) noexcept :
Que( new QueList{ Data, Key, OtherKeys... } )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::MultiLevelQue( DataType &&Data, KeyType &&Key, 
                                                                     OtherKeyTypes &&... OtherKeys ) noexcept :
Que( new QueList{ std::move( Data ), std::move( Key ), std::move( OtherKeys )... } )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::MultiLevelQue( const DataType &Data, 
                                                                     const QueRange< KeyType, OtherKeyTypes... > &Keys ) noexcept :
Que( new QueList{ Data, Keys.GetKey(), Keys.GetOthers() } )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::MultiLevelQue( DataType &&Data, QueRange< KeyType, OtherKeyTypes... > &&Keys ) noexcept :
Que( new QueList{ std::move( Data ), std::move( Keys.GetKey() ), std::move( Keys.GetOthers ) } )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::MultiLevelQue( const MultiLevelQue &Copy ) noexcept :
Que( new QueList{ *( Copy.Que ) } )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::MultiLevelQue( MultiLevelQue &&Move ) noexcept : Que( Move.Que )
{
  QueList *IterPtr = Que;

  Move.Que = Move.Que->Next;

  while( Move.Que )
  {
    IterPtr->Next = Move.Que;

    IterPtr = IterPtr->Next;
    Move.Que = Move.Que->Next;
  }
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::~MultiLevelQue() noexcept
{
  while( Que )
  {
    QueList *Temp = Que->Next;

    delete Que;

    Que = Temp;
  }
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
DataType & MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Top() noexcept
{
  return Que->Data.Top();
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
const typename MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Iter
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::begin() const noexcept
{
  return Iter{ *this };
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
constexpr typename MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList *
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::end() noexcept
{
  return nullptr;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
template< typename ... InnerKeyTypes, typename ... OuterKeyTypes >
MultiLevelQue< DataType, InnerKeyTypes... >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::GetQue( const KeyType &Key, const OuterKeyTypes &... OtherKeys ) const ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->MultiLevelQue< DataType, InnerKeyTypes... >
  {
    CatchAssertDebug( return IterPtr->Data.template GetQue< InnerKeyTypes... >( OtherKeys... ), CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
template< typename ... InnerKeyTypes >
MultiLevelQue< DataType, InnerKeyTypes... >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::GetQue( const KeyType &Key ) const ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->MultiLevelQue< DataType, InnerKeyTypes... >
  {
    MultiLevelQue< DataType, InnerKeyTypes... > &Return = MultiLevelQue< DataType, InnerKeyTypes... >{ IterPtr->Data }; // NamedRVO

    CatchAssertDebug( return Return, decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
bool MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Empty() const noexcept
{
  return Que ? false : true;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
DataType & MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Find( const KeyType &Key, const OtherKeyTypes &... Others ) ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->DataType &
  {
    CatchAssertDebug( return IterPtr->Data.Find( Others... ), decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
DataType * MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Find_NoThrow( const KeyType &Key, const OtherKeyTypes &... Others ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->DataType *
  {
    return IterPtr->Data.Find_NoThrow( Others... );
  };

  CatchAssert_NoThrow( return Finder( Key, Lambda ), CouldNotFind );

  return static_cast< DataType * >( nullptr );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
DataType & MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Find( const QueRange< KeyType, OtherKeyTypes... > &Range ) ExceptDebug
{
  const auto &Key = Range.GetKey();

  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->DataType &
  {
    CatchAssertDebug( return IterPtr->Data.Find( Range.GetOthers() ), decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
DataType * MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Find_NoThrow( const QueRange< KeyType, OtherKeyTypes... > &Range ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->DataType *
  {
    return IterPtr->Data.Find_NoThrow( Range.GetOthers() );
  };

  CatchAssert_NoThrow( return Finder( Range.GetKey(), Lambda ), CouldNotFind );

  return static_cast< DataType * >( nullptr );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
std::vector< DataType > MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindMultiple( const KeyType &KeyBegin,
                                                                                            const OtherKeyTypes &... OthersBegin,
                                                                                            const QueRange< KeyType, 
                                                                                                     OtherKeyTypes... > &End ) ExceptDebug
{
  std::vector< DataType > Ranges{ std::move( Find( KeyBegin, OthersBegin... ) ) };

  QueRange< KeyType, OtherKeyTypes... > Begin{ KeyBegin, OthersBegin... };

  while( Begin != End )
  {
    ++Begin;

    Ranges.push_back( Find( Begin ) );
  }

  return Ranges;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
std::vector< DataType > MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindMultiple_NoThrow( const KeyType &KeyBegin,
                                                                                                    const OtherKeyTypes &... OthersBegin,
                                                                                                    const QueRange< KeyType,
                                                                                                        OtherKeyTypes... > &End ) noexcept
{
  std::vector< DataType > Ranges{ std::move( Find_NoThrow( KeyBegin, OthersBegin... ) ) };

  QueRange< KeyType, OtherKeyTypes... > Begin{ KeyBegin, OthersBegin... };

  while( Begin != End )
  {
    ++Begin;

    Ranges.push_back( Find_NoThrow( Begin ) );
  }

  return Ranges;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
auto MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindRange( const KeyType &Key, const OtherKeyTypes &... Others ) ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->auto
  {
    CatchAssertDebug( return IterPtr->Data.FindRange( Others... ), decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
auto MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindRange_NoThrow( const KeyType &Key, const OtherKeyTypes &... Others ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->auto
  {
    return IterPtr->Data.FindRange_NoThrow( Others... );
  };

  CatchAssert_NoThrow( return Finder( Key, Lambda ), CouldNotFind );

  return decltype( FindRange_NoThrow( Others... ) ){ nullptr, nullptr };
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
auto MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindRange( const QueRange< KeyType, OtherKeyTypes... > &Range ) ExceptDebug
{
  const auto &Key = Range.GetKey();

  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->auto
  {
    CatchAssertDebug( return IterPtr->Data.FindRange( Range.GetOthers() ), decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
auto MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindRange_NoThrow( const QueRange< KeyType, OtherKeyTypes... > &Range ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->auto
  {
    return IterPtr->Data.FindRange_NoThrow( Range.GetOthers() );
  };

  CatchAssert_NoThrow( return Finder( Range.GetKey(), Lambda ), CouldNotFind );

  return decltype( FindRange_NoThrow( Range.GetOthers() ) ){ nullptr, nullptr };
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
auto MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindMultipleRanges( const KeyType &KeyBegin, 
                                                                               const OtherKeyTypes &... OthersBegin,
                                                                               const QueRange< KeyType,
                                                                                               OtherKeyTypes... > &End ) ExceptDebug
{
  auto Temp = FindRange( KeyBegin, OthersBegin... );

  std::vector< decltype( Temp ) > Ranges{ std::move( Temp ) };

  QueRange< KeyType, OtherKeyTypes... > Begin{ KeyBegin, OthersBegin... };

  while( Begin != End )
  {
    ++Begin;

    Ranges.push_back( FindRange( Begin ) );
  }

  return Ranges;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
auto MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::FindMultipleRanges_NoThrow( const KeyType &KeyBegin,
                                                                                       const OtherKeyTypes &... OthersBegin,
                                                                                       const QueRange< KeyType,
                                                                                                       OtherKeyTypes... > &End ) noexcept
{
  auto Temp = FindRange_NoThrow( KeyBegin, OthersBegin... );

  std::vector< decltype( Temp ) > Ranges{ std::move( Temp ) };

  QueRange< KeyType, OtherKeyTypes... > Begin{ KeyBegin, OthersBegin... };

  while( Begin != End )
  {
    ++Begin;

    Ranges.push_back( FindRange_NoThrow( Begin ) );
  }

  return Ranges;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Emplace( const DataType &Data, const KeyType &Key,
                                                                    const OtherKeyTypes &... Others ) noexcept
{
  Emplacer( &MultiLevelQue< DataType, OtherKeyTypes... >::Emplace, Data, Key, Others... );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Emplace( DataType &&Data, KeyType && Key, OtherKeyTypes &&... Others ) noexcept
{
  Emplacer( &MultiLevelQue<  DataType, OtherKeyTypes... >::Emplace, std::move( Data ), std::move( Key ), std::move( Others )... );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::EmplaceAfter( const DataType &Data, const KeyType &Key, 
                                                                         const OtherKeyTypes &... Others ) noexcept
{
  Emplacer( &MultiLevelQue< DataType, OtherKeyTypes... >::EmplaceAfter, Data, Key, Others... );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::EmplaceAfter( DataType &&Data, KeyType &&Key,
                                                                         OtherKeyTypes &&... Others ) noexcept
{
  Emplacer( &MultiLevelQue<  DataType, OtherKeyTypes... >::Emplace, std::move( Data ), std::move( Key ), std::move( Others )... );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Pop() ExceptDebug
{
  AssertDebug( Empty(), QueIsEmpty{} );

  TopPtr->Data.Pop();

  if( TopPtr->Data.Empty() )
  {
    QueList *Temp;

    if( TopPtr->Parent )
    {
      TopPtr->Parent->Less = TopPtr->Greater;

      Temp = TopPtr->Parent->Less;
    }
    else
    {
      Que = TopPtr->Greater;

      Temp = Que;
    }

    delete TopPtr;

    while( Temp->Less )
    {
      Temp = Temp->Less;
    }

    TopPtr = Temp;
  }
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Erase( const KeyType &Key, const OtherKeyTypes &... Others ) ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->void
  {
    CatchAssertDebug( IterPtr->Data.Erase( Others... ), decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::EraseRange( const KeyType &Key, const OtherKeyTypes &... Others ) ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->void
  {
    CatchAssertDebug( IterPtr->Data.EraseRange( Others... ), decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  Finder( Key, Lambda );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Emplacer( void ( MultiLevelQue< DataType, OtherKeyTypes... >::*EmplacePtr )
                                                                          ( const DataType &, const OtherKeyTypes &... ),
                                                                     const DataType &Data, const KeyType &Key, 
                                                                     const OtherKeyTypes &... Others ) noexcept
{
  auto &Lambda1 = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Less )
    {
      IterPtr = IterPtr->Less;

      return false;
    }

    IterPtr->Less = new QueList{ IterPtr, Data, MakeQueRange( Key, Others... ) };

    if( IterPtr == TopPtr )
    {
      TopPtr = IterPtr->Less;
    }

    return true;
  };
  
  auto &Lambda2 = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Greater )
    {
      IterPtr = IterPtr->Greater;

      return false;
    }

    IterPtr->Greater = new QueList{ IterPtr, Data, MakeQueRange( Key, Others... ) };

    return true;
  };
  
  auto &Lambda3 = [ & ]( QueList *IterPtr )noexcept->void
  {
    ( IterPtr->Data.*EmplacePtr )( Data, Others... );
  };

  Finder_NoThrow( Key, Lambda1, Lambda2, Lambda3 );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Emplacer( void ( MultiLevelQue< DataType, OtherKeyTypes... >::*EmplacePtr )
                                                                          ( DataType &&, OtherKeyTypes &&... ),
                                                                     DataType &&Data, KeyType &&Key, OtherKeyTypes &&... Others ) noexcept
{
  auto &Lambda1 = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Less )
    {
      IterPtr = IterPtr->Less;

      return false;
    }

    IterPtr->Less = new QueList{ IterPtr,  std::move( Data ), MakeQueRange( std::move( Key ), std::move( Others )... ) };

    if( IterPtr == TopPtr )
    {
      TopPtr = IterPtr->Less;
    }

    return true;
  };

  auto &Lambda2 = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Greater )
    {
      IterPtr = IterPtr->Greater;

      return false;
    }

    IterPtr->Greater = new QueList{ IterPtr,  std::move( Data ), MakeQueRange( std::move( Key ), std::move( Others )... ) };

    return true;
  };

  auto &Lambda3 = [ & ]( QueList *IterPtr )noexcept->void
  {
    ( IterPtr->Data.*EmplacePtr )( std::move( Data ), std::move( Others )... );
  };

  Finder_NoThrow( Key, Lambda1, Lambda2, Lambda3 );
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
template< typename Return, bool IsThrowing >
Return MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Finder( const KeyType &Key,
                                                                     Return( *FunctionPtr )( QueList * ) noexcept( IsThrowing ) )
                                                                       noexcept( IsThrowing || ExceptDebug )
{
  QueList *IterPtr = Que;

  for( ; ; )
  {
    AssertDebug( !IterPtr, CouldNotFind{ Key } );

    if( IterPtr->Key < Key )
    {
      IterPtr = IterPtr->Less;
    }
    else if( IterPtr->Key > Key )
    {
      IterPtr = IterPtr->Greater;
    }
    else
    {
      return FunctionPtr( IterPtr );
    }
  }
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
template< typename Return >
Return MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Finder_NoThrow( const KeyType &Key,
                                                                             Return( *FunctionPtr )( QueList * ) noexcept ) noexcept
{
  QueList *IterPtr = Que;

  for( ; ; )
  {
    if( IterPtr->Key < Key )
    {
      IterPtr = IterPtr->Less;
    }
    else if( IterPtr->Key > Key )
    {
      IterPtr = IterPtr->Greater;
    }
    else
    {
      return FunctionPtr( IterPtr );
    }
  }
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
template< bool IsThrowing1, bool IsThrowing2, bool IsThrowing3 >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Finder( const KeyType &Key,
                                                                   bool( *Function1 )( QueList * ) noexcept( IsThrowing1 ),
                                                                   bool( *Function2 )( QueList * ) noexcept( IsThrowing2 ),
                                                                   void( *Function3 )( QueList * ) noexcept( IsThrowing3 ) )
                                                                     noexcept( IsThrowing1 || IsThrowing2 || IsThrowing3 || ExceptDebug )
{
  QueList *IterPtr = Que;

  for( ; ; )
  {
    AssertDebug( IterPtr, CouldNotFind{ Key } );

    if( IterPtr->Key < Key )
    {
      if( Function1( IterPtr ) )
      {
        return;
      }
    }
    else if( IterPtr->Key > Key )
    {
      if( Function2( IterPtr ) )
      {
        return;
      }
    }
    else
    {
      Function3( IterPtr );

      return;
    }
  }
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Finder_NoThrow( const KeyType &Key, bool( *Function1 )( QueList * ) noexcept,
                                                                           bool( *Function2 )( QueList * ) noexcept,
                                                                           void( *Function3 )( QueList * ) noexcept ) noexcept
{
  QueList *IterPtr = Que;

  for( ; ; )
  {
    if( IterPtr->Key < Key )
    {
      if( Function1( IterPtr ) )
      {
        return;
      }
    }
    else if( IterPtr->Key > Key )
    {
      if( Function2( IterPtr ) )
      {
        return;
      }
    }
    else
    {
      Function3( IterPtr );

      return;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList::QueList( const DataType &Data, const KeyType &Key,
                                                                        const OtherKeyTypes &... Others ) noexcept :
Key{ Key }, Data{ Data, Others... }
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList::QueList( DataType &&Data, KeyType &&Key, 
                                                                        OtherKeyTypes &&... Others ) noexcept :
Key{ std::move( Key ) }, Data{ std::move( Data ), std::move( Others )... }
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList::QueList( QueList *Parent, const DataType &Data, 
                                                                        const QueRange< KeyType, OtherKeyTypes... > &Keys ) noexcept :
Key{ Keys.GetKey() }, Data{ Data, Keys.GetOthers() }, Greater( nullptr ), Less( nullptr ), Parent( Parent )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList::QueList( QueList *Parent, DataType &&Data,
                                                                        QueRange< KeyType, OtherKeyTypes... > &&Keys ) noexcept :
Key{ std::move( Keys.GetKey() ) }, Data{ std::move( Data ), std::move( Keys.GetOthers() )... }, Greater( nullptr ), Less( nullptr ),
Parent( Parent )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList::QueList( const QueList &Copy ) noexcept :
Key{ Copy.Key }, Data{ Copy.Data }, Greater( Copy.Greater ? new QueList{ this, *( Copy.Greater ) } : nullptr ),
Less( Copy.Less ? new QueList{ this, *( Copy.Less ) } : nullptr ), Parent( nullptr )
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::QueList::QueList( QueList *Parent, const QueList &Copy ) noexcept :
Key{ Copy.Key }, Data{ Copy.Data }, Greater( Copy.Greater ? new QueList{ this, *( Copy.Greater ) } : nullptr ),
Less( Copy.Less ? new QueList{ this, *( Copy.Less ) } : nullptr ), Parent( Parent )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Iter::Iter( const MultiLevelQue< DataType, KeyType, OtherKeyTypes... > &Que ) noexcept : Que{ Que }
{
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
void MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Iter::operator++() const noexcept
{
  Que.Pop();
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
bool MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Iter::operator!=( const QueList *Rhs ) const noexcept
{
  return Que.Que != Rhs;
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
const DataType & MultiLevelQue< DataType, KeyType, OtherKeyTypes... >::Iter::operator*() const noexcept
{
  return Que.Top();
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType  >::CouldNotFind::CouldNotFind( const KeyType &Key ) noexcept :
What{ "Could not find the key: " + std::to_string( Key ) + "!" }
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::IterRange::IterRange( QueRange< KeyType > &&Begin, QueRange< KeyType > &&End ) noexcept :
Begin{ std::move( Begin ) }, End{ std::move( End ) }
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::IterRange::IterRange( const IterRange &Copy ) noexcept : Begin{ Copy.Begin }, End{ Copy.End }
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::IterRange::IterRange( IterRange &&Move ) noexcept : 
Begin{ std::move( Move.Begin ) }, End{ std::move( Move.End ) }
{
}

template< typename DataType, typename KeyType >
QueRange< KeyType > & MultiLevelQue< DataType, KeyType >::IterRange::begin() noexcept
{
  return Begin;
}

template< typename DataType, typename KeyType >
QueRange< KeyType > & MultiLevelQue< DataType, KeyType >::IterRange::end() noexcept
{
  return End;
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::MultiLevelQue( const DataType &Data, const KeyType &Key ) noexcept :
Que( new QueList{ Data, Key } )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::MultiLevelQue( DataType &&Data, KeyType &&Key ) noexcept :
Que( new QueList{ std::move( Data ), std::move( Key ) } )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::MultiLevelQue( const DataType &Data, const QueRange< KeyType > &Keys ) noexcept : 
Que( new QueList{ Data, Keys.GetKey() } )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::MultiLevelQue( DataType &&Data, QueRange< KeyType > &&Keys ) noexcept :
Que( new QueList{ std::move( Data ), std::move( Keys.GetKey() ) } )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::MultiLevelQue( const MultiLevelQue &Copy ) noexcept : Que( new QueList{ *( Copy.Que ) } )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::MultiLevelQue( MultiLevelQue &&Move ) noexcept : Que( Move.Que )
{
  const QueList *IterPtr = Que;

  Move.Que = Move.Que->Next;

  while( Move.Que )
  {
    IterPtr->Next = Move.Que;

    IterPtr = IterPtr->Next;
    Move.Que = Move.Que->Next;
  }
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::~MultiLevelQue() noexcept
{
  while( Que )
  {
    QueList *Temp = Que->Next;

    delete Que;

    Que = Temp;
  }
}

template< typename DataType, typename KeyType >
DataType & MultiLevelQue< DataType, KeyType >::Top() noexcept
{
  return TopPtr->Data.front();
}

template< typename DataType, typename KeyType >
const typename MultiLevelQue< DataType, KeyType >::Iter MultiLevelQue< DataType, KeyType >::begin() const noexcept
{
  return Iter{ TopPtr };
}

template< typename DataType, typename KeyType >
constexpr typename MultiLevelQue< DataType, KeyType >::QueList * MultiLevelQue< DataType, KeyType >::end() noexcept
{
  return nullptr;
}

template< typename DataType, typename KeyType >
bool MultiLevelQue< DataType, KeyType >::Empty() const noexcept
{
  return Que ? false : true;
}

template< typename DataType, typename KeyType >
DataType & MultiLevelQue< DataType, KeyType >::Find( const KeyType &Key ) ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->DataType &
  {
    CatchAssertDebug( return IterPtr->Data, decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType >
DataType * MultiLevelQue< DataType, KeyType >::Find_NoThrow( const KeyType &Key) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->DataType *
  {
    return &( IterPtr->Data );
  };

  CatchAssert_NoThrow( return Finder( Key, Lambda ), CouldNotFind );

  return nullptr;
}

template< typename DataType, typename KeyType >
DataType & MultiLevelQue< DataType, KeyType >::Find( const QueRange< KeyType > & Range ) ExceptDebug
{
  const auto &Key = Range.GetKey();

  auto &Lambda = [ & ]( QueList *IterPtr )noexcept( false )->DataType &
  {
    CatchAssertDebug( return IterPtr->Data, decltype( IterPtr->Data )::CouldNotFind,
                      CouldNotFind{ "The key: " + std::to_string( Key ) + " was found. " + Error.What } );
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType >
DataType * MultiLevelQue< DataType, KeyType >::Find_NoThrow( const QueRange< KeyType > &Range ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->DataType *
  {
    return &( IterPtr->Data );
  };

  CatchAssert_NoThrow( return Finder( Range.GetKey(), Lambda ), CouldNotFind );

  return nullptr;
}

template< typename DataType, typename KeyType >
std::vector< DataType > & MultiLevelQue< DataType, KeyType >::FindRange( const KeyType &Key ) ExceptDebug
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->std::vector< DataType > &
  {
    return IterPtr->Data;
  };

  return Finder( Key, Lambda );
}

template< typename DataType, typename KeyType >
std::vector< DataType > * MultiLevelQue< DataType, KeyType >::FindRange_NoThrow( const KeyType &Key ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->std::vector< DataType > *
  {
    return &( IterPtr->Data );
  };

  return Finder_NoThrow( Key, Lambda );
}

template< typename DataType, typename KeyType >
std::vector< DataType > & MultiLevelQue< DataType, KeyType >::FindRange( const QueRange< KeyType > & Range ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->std::vector< DataType > &
  {
    return IterPtr->Data;
  };

  return Finder( Range.GetKey(), Lambda );
}

template< typename DataType, typename KeyType >
std::vector< DataType > * MultiLevelQue< DataType, KeyType >::FindRange_NoThrow( const QueRange< KeyType > & Range ) noexcept
{
  auto &Lambda = [ & ]( QueList *IterPtr )noexcept->std::vector< DataType > *
  {
    return &( IterPtr->Data );
  };

  return Finder_NoThrow( Range.GetKey(), Lambda );
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Emplace( const DataType &Data, const KeyType &Key ) noexcept
{
  auto &LambdaLess = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Less )
    {
      IterPtr = IterPtr->Less;

      return false;
    }

    IterPtr->Less = new QueList{ IterPtr, Data, Key };

    return true;
  };
  
  auto &LambdaGreater = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Greater )
    {
      IterPtr = IterPtr->Greater;

      return false;
    }

    IterPtr->Greater = new QueList{ IterPtr, Data, Key };

    return true;
  };
  
  auto &LambdaEqual = [ & ]( QueList *IterPtr )noexcept->void
  {
    IterPtr->Data.push_front( Data );
  };

  Finder_NoThrow( Key, LambdaLess, LambdaGreater, LambdaEqual );
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Emplace( DataType &&Data, KeyType &&Key ) noexcept
{
  auto &LambdaLess = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Less )
    {
      IterPtr = IterPtr->Less;

      return false;
    }

    IterPtr->Less = new QueList{ IterPtr, std::move( Data ), std::move( Key ) };

    return true;
  };

  auto &LambdaGreater = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Greater )
    {
      IterPtr = IterPtr->Greater;

      return false;
    }

    IterPtr->Greater = new QueList{ IterPtr,std::move( Data ), std::move( Key ) };

    return true;
  };

  auto &LambdaEqual = [ & ]( QueList *IterPtr )noexcept->void
  {
    IterPtr->Data.push_front( std::move( Data ) );
  };

  Finder_NoThrow( Key, LambdaLess, LambdaGreater, LambdaEqual );
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::EmplaceAfter( const DataType &Data, const KeyType &Key ) noexcept
{
  auto &LambdaLess = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Less )
    {
      IterPtr = IterPtr->Less;

      return false;
    }

    IterPtr->Less = new QueList{ IterPtr, Data, Key };

    return true;
  };

  auto &LambdaGreater = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Greater )
    {
      IterPtr = IterPtr->Greater;

      return false;
    }

    IterPtr->Greater = new QueList{ IterPtr, Data, Key };

    return true;
  };

  auto &LambdaEqual = [ & ]( QueList *IterPtr )noexcept->void
  {
    IterPtr->Data.push_back( Data );
  };

  Finder_NoThrow( Key, LambdaLess, LambdaGreater, LambdaEqual );
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::EmplaceAfter( DataType &&Data, KeyType &&Key ) noexcept
{
  auto &LambdaLess = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Less )
    {
      IterPtr = IterPtr->Less;

      return false;
    }

    IterPtr->Less = new QueList{ IterPtr, std::move( Data ), std::move( Key ) };

    return true;
  };

  auto &LambdaGreater = [ & ]( QueList *IterPtr )noexcept->bool
  {
    if( IterPtr->Greater )
    {
      IterPtr = IterPtr->Greater;

      return false;
    }

    IterPtr->Greater = new QueList{ IterPtr,std::move( Data ), std::move( Key ) };

    return true;
  };

  auto &LambdaEqual = [ & ]( QueList *IterPtr )noexcept->void
  {
    IterPtr->Data.push_back( std::move( Data ) );
  };

  Finder_NoThrow( Key, LambdaLess, LambdaGreater, LambdaEqual );
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Pop() ExceptDebug
{
  AssertDebug( Empty(), QueIsEmpty{} );

  TopPtr->Data.pop_front();

  if( TopPtr->Data.Empty() )
  {
    Eraser( TopPtr );
  }
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Erase( const KeyType &Key ) ExceptDebug
{
  auto &LambdaEqual = [ & ]( QueList *IterPtr )noexcept->void
  {
    Eraser( IterPtr );
  };

  Finder( Key, LambdaEqual );
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::EraseRange( const KeyType &Key ) ExceptDebug
{
  if( Que->Key != Key )
  {
    QueList *IterPtr = Que;

    while( IterPtr->Next && IterPtr->Next->Key != Key )
    {
      IterPtr = IterPtr->Next;
    }

    AssertDebug( !( IterPtr->Next ), CouldNotFind{ Key } );

    do
    {
      QueList *Temp = IterPtr->Next->Next;

      delete IterPtr->Next;

      IterPtr->Next = Temp;
    }
    while( IterPtr->Next && IterPtr->Next->Key == Key );
  }
  else
  {
    do
    {
      QueList *Temp = Que->Next;

      delete Que;

      Que = Temp;
    }
    while( Que && Que->Key == Key );
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueIsEmpty::QueIsEmpty() noexcept
{
  std::cerr << "Cant remove from the que, its empty!" << std::endl;
}

template< typename DataType, typename KeyType >
template< typename Return, bool IsThrowing >
Return MultiLevelQue< DataType, KeyType >::Finder( const KeyType &Key, Return( *FunctionPtr )( QueList* ) noexcept( IsThrowing ) )
                                                     noexcept( IsThrowing || ExceptDebug )
{
  QueList *IterPtr = Que;

  for( ; ; )
  {
    AssertDebug( !IterPtr, CouldNotFind{ Key } );

    if( IterPtr->Key < Key )
    {
      IterPtr = IterPtr->Less;
    }
    else if( IterPtr->Key > Key )
    {
      IterPtr = IterPtr->Greater;
    }
    else
    {
      return FunctionPtr( IterPtr );
    }
  }
}

template< typename DataType, typename KeyType >
template< typename Return >
Return * MultiLevelQue< DataType, KeyType >::Finder_NoThrow( const KeyType &Key, Return* ( *FunctionPtr )( QueList* ) noexcept ) noexcept
{
  QueList *IterPtr = Que;

  for( ; IterPtr; )
  {
    if( IterPtr->Key < Key )
    {
      IterPtr = IterPtr->Less;
    }
    else if( IterPtr->Key > Key )
    {
      IterPtr = IterPtr->Greater;
    }
    else
    {
      return FunctionPtr( IterPtr );
    }
  }

  return nullptr;
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Finder_NoThrow( const KeyType &Key, bool( *Lambda1 )( QueList* ) noexcept,
                                                         bool( *Lambda2 )( QueList* ) noexcept, 
                                                         void( *Lambda3 )( QueList* ) noexcept ) noexcept
{
  QueList *IterPtr = Que;

  for( ; ; )
  {
    if( IterPtr->Key < Key )
    {
      if( *Lambda1( IterPtr ) )
      {
        return;
      }
    }
    else if( IterPtr->Key > Key )
    {
      if( Lambda2( IterPtr ) )
      {
        return;
      }
    }
    else
    {
      Lambda3( IterPtr );

      return;
    }
  }

}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Eraser( QueList *( &ErasePtr ) ) noexcept
{
  if( ErasePtr->Parent )
  {
    ErasePtr->Parent->Less = ErasePtr->Greater;

    QueList *Temp = ErasePtr->Parent;

    if( ErasePtr->Greater )
    {
      delete ErasePtr;

      ErasePtr = Temp->Less;
    }
    else
    {
      delete ErasePtr;

      ErasePtr = Temp;
    }
  }
  else
  {
    Que = Que->Greater;

    delete ErasePtr;

    ErasePtr = Que;
  }
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueList::QueList( const DataType &Data, const KeyType &Key ) noexcept : Key{ Key }, Data{ Data }
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueList::QueList( DataType &&Data, KeyType &&Key ) noexcept :
Key{ std::move( Key ) }, Data{ std::move( Data ) }
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueList::QueList( QueList *Parent, const DataType &Data, const KeyType &Key ) noexcept :
Key{ Key }, Data{ Data }, Greater( nullptr ), Less( nullptr ), Parent( Parent )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueList::QueList( QueList *Parent, DataType &&Data, KeyType &&Key ) noexcept : 
Key{ std::move( Key ) }, Data{ std::move( Data ) }, Greater( nullptr ), Less( nullptr ), Parent( Parent )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueList::QueList( const QueList &Copy ) noexcept : 
Key{ Copy.Key }, Data{ Copy.Data }, Greater( Copy.Greater ? new QueList{ this, *( Copy.Greater ) } : nullptr ),
Less( Copy.Less ? new QueList{ this, *( Copy.Less ) } : nullptr ), Parent( nullptr )
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::QueList::QueList( QueList *Parent, const QueList &Copy ) noexcept : 
Key{ Copy.Key }, Data{ Copy.Data }, Greater( Copy.Greater ? new QueList{ this, *( Copy.Greater ) } : nullptr ),
Less( Copy.Less ? new QueList{ this, *( Copy.Less ) } : nullptr ), Parent( Parent )
{
}

///////////////////////////////////////////////////////////////////////////////

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::Iter::Iter( MultiLevelQue< DataType, KeyType > *Que ) noexcept : Que{ *Que }
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::Iter::Iter( const Iter &Copy ) noexcept : Que{ Copy.Que }
{
}

template< typename DataType, typename KeyType >
MultiLevelQue< DataType, KeyType >::Iter::Iter( Iter &&Move ) noexcept : Que{ std::move( Move.Que ) }
{
}

template< typename DataType, typename KeyType >
const DataType & MultiLevelQue< DataType, KeyType >::Iter::operator*() const noexcept
{
  return Que->Top();
}

template< typename DataType, typename KeyType >
void MultiLevelQue< DataType, KeyType >::Iter::operator++() const noexcept
{
  Que->Pop();
}

template< typename DataType, typename KeyType >
bool MultiLevelQue< DataType, KeyType >::Iter::operator!=( const QueList * ) const noexcept
{
  return Que->Empty();
}

///////////////////////////////////////////////////////////////////////////////

template< typename KeyType, typename ... OtherKeyTypes >
constexpr QueRange< KeyType, OtherKeyTypes... > MakeQueRange( KeyType &&FirstKey, OtherKeyTypes &&... OtherKeys ) noexcept
{
  return QueRange< KeyType, OtherKeyTypes... >{ std::move( FirstKey ), std::move( OtherKeys )... };
}

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
constexpr MultiLevelQue< DataType, KeyType, OtherKeyTypes... > MakeMultiLevelQue( DataType &&Data, KeyType &&FirstKey,
                                                                                  OtherKeyTypes &&... Others ) noexcept
{
  return MultiLevelQue< DataType, KeyType, OtherKeyTypes... >{ std::move( Data ), std::move( FirstKey ), std::move( Others )... };
}
