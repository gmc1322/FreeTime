#include "MultiDirectionalList.h"

#include <iostream>
#include <algorithm>

using namespace MDLSpace;

Direction::Direction( const std::vector< float > &DirectionValues ) noexcept : Angles{ DirectionValues }
{
}

Direction::Direction( std::vector< float > &&DirectionValues ) noexcept : Angles{ std::move( DirectionValues ) }
{
}

bool Direction::operator<( const Direction &Rhs ) const noexcept
{
  size_t i = 0;

  for( auto Iter : Angles )
  {
    const float &Compair = Rhs.Angles[ i ];

    if( Iter < Compair )
    {
      return true;
    }
    
    if( Compair < Iter )
    {
      return false;
    }

    ++i;
  }

  return false;
}

int Direction::Calc( const std::vector< size_t > &DimSizes ) const ExceptDebug
{
  int Return = 0;
  size_t j = 0;
  size_t Mult = 1;

  for( auto AngleIter = Angles.cbegin(), AngleEnd = Angles.cend(); AngleIter != AngleEnd; ++AngleIter, ++j )
  {
    Mult *= DimSizes[ j ];

    Return += static_cast< int >( *AngleIter / 90 * Mult );
  }

  return Return; // NamedRVO
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr::DirectionPtr( const Direction &Dir,
                                                                                          Directions *NextPtr ) noexcept :
Dir{ Dir }, NextPtr( NextPtr )
{
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr::DirectionPtr( const DirectionPtr &Copy ) noexcept :
Dir{ Copy.Dir }, NextPtr( nullptr )
{
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr::DirectionPtr( DirectionPtr &&Move ) noexcept :
Dir{ Move.Dir }, NextPtr( Move.NextPtr )
{
  Move.NextPtr = nullptr;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr &
MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr::operator=( const DirectionPtr &Copy ) noexcept
{
  Dir = Copy.Dir;

  return *this;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr &
MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr::operator=( DirectionPtr &&Move ) noexcept
{
  Dir = std::move( Move.Dir );

  return *this;
}

template< typename ListType, size_t DirectionCount >
bool MultiDirectionalList< ListType, DirectionCount >::Directions::DirectionPtr::operator<( const DirectionPtr &Rhs ) const noexcept
{
  return Dir < Rhs.Dir;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::
  Directions::Directions( const ListType &Data, const std::initializer_list< DirectionPtr > &Directions ) ExceptDebug :
Data{ Data }, DirectionArray{ Directions }
{
  AssertDebug( Directions.size() != DirectionCount, NotEnoughDirectionInitializers{ Directions.size() } );

  std::sort( DirectionArray.begin(), DirectionArray.end() );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::Directions( const ListType &Data, 
                                                                          const std::vector< DirectionPtr > &Directions ) ExceptDebug :
Data{ Data }, DirectionArray{ Directions }
{
  AssertDebug( Directions.size() != DirectionCount, NotEnoughDirectionInitializers{ Directions.size() } );

  std::sort( DirectionArray.begin(), DirectionArray.end() );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::Directions( ListType &&Data, 
                                                                          std::vector< DirectionPtr > &&Directions ) ExceptDebug :
Data{ std::move( Data ) }, DirectionArray{ std::move( Directions ) }
{
  AssertDebug( Directions.size() != DirectionCount, NotEnoughDirectionInitializers{ Directions.size() } );

  std::sort( DirectionArray.begin(), DirectionArray.end() );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::Directions( const ListType &Data, const DirInitType &Directions ) noexcept :
Data{ Data }
{
  for( size_t i = 0; i < DirectionCount; ++i )
  {
    DirectionArray[ i ] = DirectionPtr{ Directions[ i ].Dir };
  }

  std::sort( DirectionArray.begin(), DirectionArray.end() );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::Directions( ListType &&Data, const DirInitType &Directions ) noexcept :
Data{ std::move( Data ) }
{
  for( size_t i = 0; i < DirectionCount; ++i )
  {
    DirectionArray[ i ] = DirectionPtr{ Directions[ i ].Dir };
  }

  std::sort( DirectionArray.begin(), DirectionArray.end() );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Directions::Directions( ListType &&Data, DirInitType &&Directions ) noexcept :
Data{ std::move( Data ) }, DirectionArray( std::move( Directions ) )
{
  std::sort( DirectionArray.begin(), DirectionArray.end() );
}

template< typename ListType, size_t DirectionCount>
MultiDirectionalList< ListType, DirectionCount >::Directions::Directions( Directions &&Dir ) noexcept : 
Data{ std::move( Dir.Data ) }, DirectionArray{ std::move( Dir.DirectionArray ) }
{
}

template< typename ListType, size_t DirectionCount >
void MultiDirectionalList< ListType, DirectionCount >::Directions::operator=( Directions &&Dir ) noexcept
{
  Data = std::move( Dir.Data );

  DirectionArray = std::move( Dir.DirectionArray );
}

template< typename ListType, size_t DirectionCount >
size_t MultiDirectionalList< ListType, DirectionCount >::Directions::Find( const std::vector< float > &Direction_ ) noexcept
{
  const DirectionPtr Compair{ Direction{ Direction_ } };

  const DirectionPtr *Start = DirectionArray.begin();

  return static_cast< DirectionPtr * >( std::bsearch( &Compair, Start, DirectionCount, sizeof( DirectionArray[ 0 ] ),
                                                      []( const void *Lhs, const void *Rhs )->int
                                                      {
                                                        const DirectionPtr *LhsPtr = static_cast< const DirectionPtr * >( Lhs );
                                                        const DirectionPtr *RhsPtr = static_cast< const DirectionPtr * >( Rhs );
                                                        return *LhsPtr < *RhsPtr ? -1 : ( *RhsPtr < *LhsPtr ? 1 : 0 );
                                                      } ) ) - Start;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::
  Directions::CouldNotFind::CouldNotFind( const std::vector< float > &Angles ) noexcept
{
  std::cerr << "Could not find the angles: ";

  auto End = Angles.end();

  --End;

  for( auto Iter = Angles.begin(); Iter != End; ++Iter )
  {
    std::cerr << *Iter << ", ";
  }

  std::cerr << " and " << *End << std::endl;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Iterator::Iterator( const std::vector< size_t > &DimensionSizes,
                                                                      Directions *Begin ) noexcept :
DimensionSizes( DimensionSizes ), Iter( Begin )
{
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Iterator::Iterator( const Iterator &Rhs ) noexcept : 
DimensionSizes{ Rhs.DimensionSizes }, Iter( Rhs.Iter )
{
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Iterator::Iterator( Iterator &&Rhs ) noexcept :
DimensionSizes{ std::move( Rhs.DimensionSizes ) }, Iter( Rhs.Iter )
{
}

template< typename ListType, size_t DirectionCount >
ListType & MultiDirectionalList< ListType, DirectionCount >::Iterator::operator*() const noexcept
{
  return Iter->Data;
}

template< typename ListType, size_t DirectionCount >
ListType * MultiDirectionalList< ListType, DirectionCount >::Iterator::operator->() const noexcept
{
  return &( Iter->Data );
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator & 
MultiDirectionalList< ListType, DirectionCount >::Iterator::operator++() noexcept
{
  Iter = Iter->Next.NextPtr;

  return *this;
}

template< typename ListType, size_t DirectionCount >
bool MultiDirectionalList< ListType, DirectionCount >::Iterator::operator!=( const Iterator &Rhs ) const noexcept
{
  return Iter != Rhs.Iter;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator &
MultiDirectionalList< ListType, DirectionCount >::Iterator::operator=( const Iterator &Rhs ) noexcept
{
  Iter = Rhs.Iter;

  return *this;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator & 
MultiDirectionalList< ListType, DirectionCount >::Iterator::operator=( Iterator &&Rhs ) noexcept
{
  Iter = Rhs.Iter;

  return *this;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator &
MultiDirectionalList< ListType, DirectionCount >::Iterator::Move( size_t Index ) ExceptDebug
{
  size_t DirIndex = DirectionCount - 1;

  const StaticArray< typename Directions::DirectionPtr, DirectionCount > &Array = Iter->DirectionArray;

  Direction Dir = Array[ DirIndex ].Dir;

  const Direction Zero{ std::vector< float >( DimensionSizes.size() ) };

  while( Zero < Dir )
  {
    const size_t End = static_cast< size_t >( std::abs( Dir.Calc( DimensionSizes ) ) );

    while( End <= Index )
    {
      Iter = Iter->DirectionArray[ DirIndex ].NextPtr;

      if( !Iter )
      {
        break;
      }

      Index -= End;
    }

    if( Index )
    {
      Dir = Array[ --DirIndex ].Dir;
    }
    else
    {
      return *this;
    }
  }

  for( size_t i = 0; i < Index; ++i )
  {
    Iter = Iter->Next.NextPtr;

    AssertDebug( !Iter, ( MovedTooFar{ Index, false } ) );
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator &
MultiDirectionalList< ListType, DirectionCount >::Iterator::MoveBack( size_t Index ) ExceptDebug
{
  size_t DirIndex = 0;

  const StaticArray< typename Directions::DirectionPtr, DirectionCount > &Array = Iter->DirectionArray;

  Direction Dir = Array[ DirIndex ].Dir;

  DebugElse( const Direction Zero{ std::vector< float >( DimensionSizes.size() ) }; while( Dir < Zero ), for( ; ; ) )
  {
    const size_t End = static_cast< size_t >( std::abs( Dir.Calc( DimensionSizes ) ) );

    while( End <= Index )
    {
      Iter = Iter->DirectionArray[ DirIndex ].NextPtr;

     if( !Iter )
     {
       break;
     }

      Index -= End;
    }

    if( Index )
    {
      Dir = Array[ ++DirIndex ].Dir;
    }
    else
    {
      return *this;
    }
  }

  DebugOnly( throw( UnableToMove( Index, DimensionSizes[ 0 ] ) ) );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Iterator::MovedTooFar::MovedTooFar( size_t Index, bool Back ) noexcept
{
  if( Back )
  {
    std::cerr << "You tried to move the Iterator back " << Index << " nodes, but there is no node there!" << std::endl;
  }
  else
  {
    std::cerr << "You tried to move the Iterator forward " << Index << " nodes, but there is no node there!" << std::endl;
  }
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Iterator::UnableToMove::UnableToMove( size_t Index, size_t DimSize ) noexcept
{
  std::cerr << "The Iterator is unable to move back " << Index << " spaces with the available directions!" << std::endl;
  std::cerr << "This can be fixed by including the direction: { -" << 1.0f / DimSize * 90 << "f, 0 }" << std::endl;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator &
MultiDirectionalList< ListType, DirectionCount >::Iterator::Move( const Direction &Dir, size_t Count ) ExceptDebug
{
  const size_t Index = Iter->Find( Dir.Angles );

  for( size_t i = 0; i < Count; ++i )
  {
    Iter = Iter->DirectionArray[ Index ].NextPtr;

    AssertDebug( !Iter, ( MultiDirectionalList< ListType, DirectionCount >::MovedTooFar{ Dir, Count } ) );
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::ConstIterator::ConstIterator( const std::vector< size_t > &DimensionSizes,
                                                                                Directions *Begin ) noexcept : Iterator( DimensionSizes,
                                                                                                                         Begin )
{
}

template< typename ListType, size_t DirectionCount >
const ListType & MultiDirectionalList< ListType, DirectionCount >::ConstIterator::operator*() const noexcept
{
  return static_cast< Iterator *const >( this )->operator*();
}

template< typename ListType, size_t DirectionCount >
const ListType * MultiDirectionalList< ListType, DirectionCount >::ConstIterator::operator->() const noexcept
{
  return static_cast< Iterator *const >( this )->operator->();
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::ConstIterator & 
MultiDirectionalList< ListType, DirectionCount >::ConstIterator::Move( const Direction &Dir, size_t Count ) ExceptDebug
{
  static_cast< Iterator *const >( this )->Move( Dir, Count );

  return *this;
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::ConstIterator & 
MultiDirectionalList< ListType, DirectionCount >::ConstIterator::Move( size_t Index ) ExceptDebug
{
  static_cast< Iterator *const >( this )->Move( Index );

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::MultiDirectionalList( const ListType &Data, const DirInitType &Directions_,
                                                                        const std::vector< size_t > &DimensionCounts ) ExceptDebug :
DimensionSizes{ DimensionCounts }, DirectionsMemory( DimensionSizes, Data, Directions_ ), DirectionalHead( DirectionsMemory.GetTop() ),
Size( 1 )
{
  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionSizes.size(),( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                                                 DimensionSizes.size() } ) );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::MultiDirectionalList( ListType &&Data, DirInitType &&Directions_, 
                                                                        std::vector< size_t > &&DimensionCounts ) ExceptDebug :
DimensionSizes{ std::move( DimensionCounts ) }, DirectionsMemory( DimensionSizes, std::move( Data ), std::move( Directions_ ) ), 
DirectionalHead( DirectionsMemory.GetTop() ), Size( 1 )
{
  AssertDebug( DirectionsMemory.GetTop()->DirectionArray[ 0 ].Dir.Angles.size() != DimensionSizes.size(),
    ( DirectionSizeError{ DirectionsMemory.GetTop()->DirectionArray[ 0 ].Dir.Angles.size(), DimensionSizes.size() } ) );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::MultiDirectionalList( const DataInitType &Data,
                                                                        const DirInitType &Directions_,
                                                                        const std::vector< size_t > &DimensionCounts ) ExceptDebug :
DimensionSizes{ DimensionCounts }, DirectionsMemory( Data.size(), *( Data.begin() ), Directions_ ), 
DirectionalHead( DirectionsMemory.GetTop() ), Size( 1 )
{
  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionSizes.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                                                  DimensionSizes.size() } ) );
  Directions *Iter = DirectionalHead;

  for( auto DataIter = Data.begin(), EndIter = Data.end(); ++DataIter != EndIter; )
  {
    Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ *DataIter, DirectionalHead->DirectionArray } );

    Iter = Iter->Next.NextPtr;

    ++Size;
  }

  ValidateIterators();
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::MultiDirectionalList( DataInitType &&Data,
                                                                        DirInitType &&Directions_, 
                                                                        std::vector< size_t > &&DimensionCounts ) ExceptDebug :
DimensionSizes{ std::move( DimensionCounts ) },
DirectionsMemory( Data.size(), std::move( *( Data.begin() ) ), std::move( Directions_ ) ),
DirectionalHead( DirectionsMemory.GetTop() ), Size( 1 )
{
  AssertDebug( DirectionsMemory.GetTop()->DirectionArray[ 0 ].Dir.Angles.size() != DimensionSizes.size(),
    ( DirectionSizeError{ DirectionsMemory.GetTop()->DirectionArray[ 0 ].Dir.Angles.size(), DimensionSizes.size() } ) );

  Directions *Iter = DirectionalHead;

  for( auto DataIter = Data.begin(), EndIter = Data.end(); ++DataIter != EndIter; )
  {
    Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *DataIter ), DirectionalHead->DirectionArray } );

    Iter = Iter->Next.NextPtr;

    ++Size;
  }

  ValidateIterators();
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator 
MultiDirectionalList< ListType, DirectionCount >::Move( const Direction &Dir, size_t Count ) ExceptDebug
{
  Directions *ReturnPtr = DirectionalHead;

  const size_t Index = ReturnPtr->Find( Dir.Angles );

  for( size_t i = 0; i < Count; ++i )
  {
    ReturnPtr = ReturnPtr->DirectionArray[ Index ].NextPtr;

    AssertDebug( !ReturnPtr, ( MovedTooFar{ Dir, Count } ) );
  }

  return Iterator{ DimensionSizes, ReturnPtr };
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator
MultiDirectionalList< ListType, DirectionCount >::Move( size_t Index ) ExceptDebug
{
  Iterator Return{ DimensionSizes, MoveTo( DirectionalHead, Index ) };
  return Return; // NamedRVO
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::ConstIterator
MultiDirectionalList< ListType, DirectionCount >::Move( const Direction &Dir, size_t Count ) const ExceptDebug
{
  Directions *ReturnPtr = DirectionalHead;

  const size_t Index = ReturnPtr->Find( Dir.Angles );

  for( size_t i = 0; i < Count; ++i )
  {
    ReturnPtr = ReturnPtr->DirectionArray[ Index ].NextPtr;

    AssertDebug( !ReturnPtr, ( MovedTooFar{ Dir, Count } ) );
  }

  return ConstIterator{ DimensionSizes, ReturnPtr };
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::ConstIterator
MultiDirectionalList< ListType, DirectionCount >::Move( size_t Index ) const ExceptDebug
{
  ConstIterator Return{ DimensionSizes, MoveTo( DirectionalHead, Index ) };
  return Return; // NamedRVO
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > & 
MultiDirectionalList< ListType, DirectionCount >::PushBack( const ListType &Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ Size - 1, Size } ) );

  MoveTo( DirectionalHead, Size - 1 )->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ Data, DirectionalHead->DirectionArray } );

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > & 
MultiDirectionalList< ListType, DirectionCount >::PushBack( ListType &&Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ Size - 1, Size } ) );

  MoveTo( DirectionalHead, Size - 1 )->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( Data ),
                                                                                             DirectionalHead->DirectionArray } );

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushBack( const DataInitType &Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ Size - 1, Size } ) );

  Directions *Iter = MoveTo( DirectionalHead, Size - 1 );

  for( auto VecIter : Data )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ VecIter, DirectionalHead->DirectionArray } );

    ++Size;
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushBack( DataInitType &&Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ Size - 1, Size } ) );

  Directions *Iter = MoveTo( DirectionalHead, Size - 1 );

  for( auto Begin = Data.begin(), End = Data.end(); Begin != End; ++Begin, ++Size )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), DirectionalHead->DirectionArray } );
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushBack( const DataInitType &Data, const DirInitType &Directions_,
                                                            const std::vector< size_t > &DimensionCounts ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ Size - 1, Size } ) );

  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                              DimensionCounts.size() } ) );

  DimensionSizes = DimensionCounts;

  Directions *Iter = MoveTo( DirectionalHead, Size - 1 );

  for( auto VecIter : Data )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ VecIter, Directions_ } );

    ++Size;
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushBack( DataInitType &&Data, DirInitType &&Directions_,
                                                            std::vector< size_t > &&DimensionCounts ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ Size - 1, Size } ) );

  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                              DimensionCounts.size() } ) );

  DimensionSizes = std::move( DimensionCounts );

  Directions *Iter = MoveTo( DirectionalHead, Size - 1 );

  auto End = Data.end();

  --End;

  for( auto Begin = Data.begin() ; Begin != End; ++Begin, ++Size )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), Directions_ } );
  }

  Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *End ), std::move( Directions_ ) } );

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( const ListType &Data, const DirInitType &Directions_,
                                                             const std::vector< size_t > &DimensionCounts ) ExceptDebug
{
  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                              DimensionCounts.size() } ) );

  DimensionSizes = DimensionCounts;

  if( !DirectionalHead )
  {
    DirectionalHead = DirectionsMemory = std::make_pair( DimensionSizes, Directions{ Data, Directions_ } );
  }
  else
  {
    Directions *Temp = DirectionalHead;

    DirectionalHead = DirectionsMemory.SetBlock( Directions{ Data, Directions_ } );

    DirectionalHead->Next.NextPtr = Temp;
  }

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( ListType &&Data, DirInitType &&Directions_,
                                                             std::vector< size_t > &&DimensionCounts ) ExceptDebug
{
  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                              DimensionCounts.size() } ) );

  DimensionSizes = std::move( DimensionCounts );

  if( !DirectionalHead )
  {
    DirectionalHead = DirectionsMemory = std::make_pair( DimensionSizes, Directions{ std::move( Data ), std::move( Directions_ ) } );
  }
  else
  {
    Directions *Temp = DirectionalHead;

    DirectionalHead = DirectionsMemory.SetBlock( Directions{ std::move( Data ), std::move( Directions_ ) } );

    DirectionalHead->Next.NextPtr = Temp;
  }

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( const DataInitType &Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, Size } ) );

  Directions *Temp = DirectionalHead;

  Directions* *Iter = &DirectionalHead;

  for( auto Begin = Data.rbegin(), End = Data.rend(); Begin != End; ++Begin )
  {
    ( *Iter ) = DirectionsMemory.SetBlock( Directions{ *Begin, Temp->DirectionArray } );

    Iter = &( ( *Iter )->Next.NextPtr );

    ++Size;
  }

  ( *Iter )->Next.NextPtr = Temp;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( DataInitType &&Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, Size } ) );

  Directions *Temp = DirectionalHead;

  Directions* *Iter = &DirectionalHead;

  for( auto Begin = Data.rbegin(), End = Data.rend(); Begin != End; ++Begin )
  {
    ( *Iter ) = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), Temp->DirectionArray } );

    Iter = &( ( *Iter )->Next.NextPtr );

    ++Size;
  }

  ( *Iter )->Next.NextPtr = Temp;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( const DataInitType &Data, const DirInitType &Directions_,
                                                             const std::vector< size_t > &DimensionCounts ) ExceptDebug
{
  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                               DimensionCounts.size() } ) );

  DimensionSizes = DimensionCounts;

  if( !DirectionalHead )
  {
    DirectionalHead = DirectionsMemory = std::make_pair( Data.size(), Directions{ std::move( *( Data.begin() ) ),
                                                                                      std::move( Directions_ ) } );

    Directions *Iter = DirectionalHead;

    for( auto Begin = Data.rbegin(), End = Data.rend(); ++Begin != End; )
    {
      Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ *Begin, Directions_ } );

      Iter = Iter->Next.NextPtr;

      ++Size;
    }
  }
  else
  {
    Directions *Temp = DirectionalHead;

    Directions* *Iter = &DirectionalHead;

    for( auto Begin = Data.rbegin(), End = Data.rend(); Begin != End; ++Begin )
    {
      ( *Iter ) = DirectionsMemory.SetBlock( Directions{ *Begin, Directions_ } );

      Iter = &( ( *Iter )->Next.NextPtr );

      ++Size;
    }

    ( *Iter )->Next.NextPtr = Temp;
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( DataInitType &&Data, DirInitType &&Directions_,
                                                             std::vector< size_t > &&DimensionCounts ) ExceptDebug
{
  AssertDebug( Directions_[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions_[ 0 ].Dir.Angles.size(),
                                                                               DimensionCounts.size() } ) );

  DimensionSizes = DimensionCounts;

  if( !DirectionalHead )
  {
    DirectionalHead = DirectionsMemory = std::make_pair( Data.size(), Directions{ std::move( *( Data.begin() ) ),
                                                                                  Directions_ } );

    Directions *Iter = DirectionalHead;

    auto End = Data.rend();

    --End;

    for( auto Begin = Data.rbegin(); ++Begin != End; )
    {
      Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), Directions_ } );

      Iter = Iter->Next.NextPtr;

      ++Size;
    }

    Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *End ), std::move( Directions_ ) } );

    ++Size;
  }
  else
  {
    Directions *Temp = DirectionalHead;

    Directions* *Iter = &DirectionalHead;

    auto End = Data.rend();

    --End;

    for( auto Begin = Data.rbegin(); Begin != End; ++Begin )
    {
      ( *Iter ) = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), Directions_ } );

      Iter = &( ( *Iter )->Next.NextPtr );

      ++Size;
    }

    ( *Iter ) = DirectionsMemory.SetBlock( Directions{ std::move( *End ), std::move( Directions_ ) } );

    ( *Iter )->Next.NextPtr = Temp;

    ++Size;
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( const ListType &Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, Size } ) );

  Directions *Temp = DirectionalHead;

  DirectionalHead = DirectionsMemory.SetBlock( Directions{ Data, DirectionalHead->DirectionArray } );

  DirectionalHead->Next.NextPtr = Temp;

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PushFront( ListType &&Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, Size } ) );

  Directions *Temp = DirectionalHead;

  DirectionalHead = DirectionsMemory.SetBlock( Directions{ std::move( Data ), DirectionalHead->DirectionArray } );

  DirectionalHead->Next.NextPtr = Temp;

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertAfter( const ListType &Data, size_t Index ) ExceptDebug
{
  Directions *Temp = MoveTo( DirectionalHead, Index );

  Directions *NextTemp = Temp->Next.NextPtr;

  Temp->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ Data, DirectionalHead->DirectionArray } );

  Temp->Next.NextPtr = NextTemp;

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertAfter( ListType &&Data, size_t Index ) ExceptDebug
{
  Directions *Temp = MoveTo( DirectionalHead, Index );

  Directions *NextTemp = Temp->Next.NextPtr;

  Temp->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( Data ), DirectionalHead->DirectionArray } );

  Temp->Next.NextPtr->Next.NextPtr = NextTemp;

  ++Size;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertAfter( const DataInitType &Data, size_t Index ) ExceptDebug
{
  Directions *Iter = MoveTo( DirectionalHead, Index );

  Directions *Temp = Iter->Next.NextPtr;

  for( auto VecIter : Data )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ VecIter, DirectionalHead->DirectionArray } );

    ++Size;
  }

  Iter->Next.NextPtr = Temp;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertAfter( DataInitType &&Data, size_t Index ) ExceptDebug
{
  Directions *Iter = MoveTo( DirectionalHead, Index );

  Directions *Temp = Iter->Next.NextPtr;

  for( auto Begin = Data.begin(), End = Data.end(); Begin != End; ++Begin )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), DirectionalHead->DirectionArray } );

    ++Size;
  }

  Iter->Next.NextPtr = Temp;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertBefore( const ListType &Data, size_t Index ) ExceptDebug
{
  return InsertAfter( Data, Index - 1 );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertBefore( ListType &&Data, size_t Index ) ExceptDebug
{
  return InsertAfter( std::move( Data ), Index - 1 );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertBefore( const DataInitType &Data, size_t Index ) ExceptDebug
{
  Directions *Iter = MoveTo( DirectionalHead, Index );

  Directions *Temp = Iter->Next.NextPtr;

  for( auto Begin = Data.rbegin(), End = Data.rend(); Begin != End; ++Begin )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ *Begin, DirectionalHead->DirectionArray } );

    ++Size;
  }

  Iter->Next.NextPtr = Temp;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::InsertBefore( DataInitType &&Data, size_t Index ) ExceptDebug
{
  Directions *Iter = MoveTo( DirectionalHead, Index );

  Directions *Temp = Iter->Next.NextPtr;

  for( auto Begin = Data.rbegin(), End = Data.rend(); Begin != End; ++Begin )
  {
    Iter = Iter->Next.NextPtr = DirectionsMemory.SetBlock( Directions{ std::move( *Begin ), DirectionalHead->DirectionArray } );

    ++Size;
  }

  Iter->Next.NextPtr = Temp;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PopFront() ExceptDebug
{
  DirectionalHead = DirectionalHead->Next.NextPtr;

  DirectionsMemory.Erase( 0 );

  --Size;

  InvalidateIterators();

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::PopBack() ExceptDebug
{
  return Erase( Size - 1 );
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::Erase( size_t Index ) ExceptDebug
{
  
  Directions *PrevTemp = MoveTo( DirectionalHead, Index - 1 );

  PrevTemp->Next.NextPtr = PrevTemp->Next.NextPtr->Next.NextPtr;
  
  DirectionsMemory.Erase( Index );

  --Size;

  InvalidateIterators();

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Iterator
MultiDirectionalList< ListType, DirectionCount >::begin() noexcept
{
  return  Iterator{ DimensionSizes, DirectionalHead };
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::ConstIterator 
MultiDirectionalList< ListType, DirectionCount >::begin() const noexcept
{
  return ConstIterator{ DirectionalHead };
}

template< typename ListType, size_t DirectionCount >
const typename MultiDirectionalList< ListType, DirectionCount >::ConstIterator
MultiDirectionalList< ListType, DirectionCount >::end() const noexcept
{
  return ConstIterator{ DimensionSizes };
}

template< typename ListType, size_t DirectionCount >
size_t MultiDirectionalList< ListType, DirectionCount >::GetSize() const noexcept
{
  return Size;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > & 
MultiDirectionalList< ListType, DirectionCount >::ValidateIterators() noexcept
{
  for( size_t i = 0; i < DirectionCount; ++i )
  {
    const typename Directions::DirectionPtr &DirIter = DirectionalHead->DirectionArray[ i ];

    Directions *Iter = DirectionalHead;

    const int End = DirIter.Dir.Calc( DimensionSizes );

    Iter = MoveTo( Iter, std::abs( End ) );

    Directions *Start = DirectionalHead;

    if( End > 0 )
    {
      while( Iter )
      {
        Start->DirectionArray[ i ].NextPtr = Iter;

        Start = Start->Next.NextPtr;
        Iter = Iter->Next.NextPtr;
      }
    }
    else
    {
      while( Iter )
      {
        Iter->DirectionArray[ i ].NextPtr = Start;

        Start = Start->Next.NextPtr;
        Iter = Iter->Next.NextPtr;
      }
    }
  }

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::SetDimensions( const DirInitType &Directions,
                                                                 const std::vector< size_t > &DimensionCounts ) ExceptDebug
{
  AssertDebug( Directions[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions[ 0 ].Dir.Angles.size(),
                                                                              DimensionCounts.size() } ) );

  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, 0 } ) );

  DimensionSizes = DimensionCounts;

  DirectionalHead->DirectionArray = Directions;

  std::sort( DirectionalHead->DirectionArray.begin(), DirectionalHead->DirectionArray.end() );

  ValidateIterators();

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &
MultiDirectionalList< ListType, DirectionCount >::SetDimensions( DirInitType &&Directions,
                                                                 std::vector< size_t > &&DimensionCounts ) ExceptDebug
{
  AssertDebug( Directions[ 0 ].Dir.Angles.size() != DimensionCounts.size(), ( DirectionSizeError{ Directions[ 0 ].Dir.Angles.size(),
                                                                               DimensionCounts.size() } ) );

  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, 0 } ) );

  DimensionSizes = std::move( DimensionCounts );

  DirectionalHead->DirectionArray = std::move( Directions );

  std::sort( DirectionalHead->DirectionArray.begin(), DirectionalHead->DirectionArray.end() );

  ValidateIterators();

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > & 
MultiDirectionalList< ListType, DirectionCount >::Resize( size_t Count, const ListType &Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, Size } ) );

  if( Count )
  {
    DirectionsMemory = std::make_pair( Count, Directions{ Data, DirectionalHead->DirectionArray } );

    Directions* *Iter = &DirectionalHead;

    for( size_t i = 0; i < Count; ++i )
    {
      ( *Iter ) = DirectionsMemory.GetBlock( i );

      Iter = &( ( *Iter )->Next.NextPtr );
    }
  }
  else
  {
    DirectionsMemory.Reset();

    DirectionalHead = nullptr;
  }

  Size = Count;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > & 
MultiDirectionalList< ListType, DirectionCount >::Resize( size_t Count, ListType &&Data ) ExceptDebug
{
  AssertDebug( !DirectionalHead, ( MovedTooFar{ 0, Size } ) );

  if( Count )
  {
    DirectionsMemory = std::make_pair( Count, Directions{ std::move( Data ), DirectionalHead->DirectionArray } );

    Directions* *Iter = &DirectionalHead;

    for( size_t i = 0; i < Count; ++i )
    {
      ( *Iter ) = DirectionsMemory.GetBlock( i );

      Iter = &( ( *Iter )->Next.NextPtr );
    }
  }
  else
  {
    DirectionsMemory.Reset();

    DirectionalHead = nullptr;
  }

  Size = Count;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > & 
MultiDirectionalList< ListType, DirectionCount >::Resize( size_t Count, const ListType &Data, const DirInitType &Directions_ ) ExceptDebug
{
  if( Count )
  {
    DirectionsMemory = std::make_pair( Count, Directions{ Data, Directions_ } );

    Directions* *Iter = &DirectionalHead;

    for( size_t i = 0; i < Count; ++i )
    {
      ( *Iter ) = DirectionsMemory.GetBlock( i );

      Iter = &( ( *Iter )->Next.NextPtr );
    }
  }
  else
  {
    DirectionsMemory.Reset();

    DirectionalHead = nullptr;
  }

  Size = Count;

  return *this;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount > &  
MultiDirectionalList< ListType, DirectionCount >::Resize( size_t Count, ListType &&Data, DirInitType &&Directions_ ) ExceptDebug
{
  if( Count )
  {
    DirectionsMemory = std::make_pair( Count, Directions{ std::move( Data ), std::move( Directions_ ) } );

    Directions* *Iter = &DirectionalHead;

    for( size_t i = 0; i < Count; ++i )
    {
      ( *Iter ) = DirectionsMemory.GetBlock( i );

      Iter = &( ( *Iter )->Next.NextPtr );
    }
  }
  else
  {
    DirectionsMemory.Reset();

    DirectionalHead = nullptr;
  }

  Size = Count;

  return *this;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::
  NotEnoughDirectionInitializers::NotEnoughDirectionInitializers( size_t Count ) noexcept
{
  std::cerr << "You tried to initialize the list with " << Count << " directions, but it requires " << DirectionCount << std::endl;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::MovedTooFar::MovedTooFar( const Direction &Dir, size_t Count ) noexcept
{
  std::cerr << "You tried to move at the angles: ";

  for( auto Iter : Dir.Angles )
  {
    std::cerr << Iter << ", ";
  }

  std::cerr << Count << " times, but the list does not have a node there!" << std::endl;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::MovedTooFar::MovedTooFar( size_t Index, size_t Size ) noexcept
{
  std::cerr << "You tried to move to index: " << Index << ", but the list only has " << Size << " nodes!" << std::endl;
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::DirectionSizeError::DirectionSizeError( size_t DirSize, size_t ListSize ) noexcept
{
  std::cerr << "The amount of dimensions in the direction struct is diffrent from the amount in the list: ";
  std::cerr << DirSize << " for the direction struct, vs: " << ListSize << " for the list!" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Memory::Memory( size_t BlockSize, const ListType &Data, 
                                                                  const DirInitType &Directions_ ) noexcept :
BlockSize( BlockSize ), CurrentIndex( 1 ), MemoryBlocks{ new Directions[ BlockSize ]() }
{
  *( MemoryBlocks[ 0 ] ) = Directions{ Data, Directions_ };
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Memory::Memory( size_t BlockSize, ListType &&Data, DirInitType &&Directions_ ) noexcept :
BlockSize( BlockSize ), CurrentIndex( 1 ), MemoryBlocks{ new Directions[ BlockSize ]() }
{
  *( MemoryBlocks[ 0 ] ) = Directions{ std::move( Data ), std::move( Directions_ ) };
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Memory::Memory( const std::vector< size_t > &BlockSizes, const ListType &Data,
                                                                  const DirInitType &Directions_ ) noexcept :
BlockSize( 1 ), CurrentIndex( 1 )
{
  for( auto Iter : BlockSizes )
  {
    BlockSize *= Iter;
  }

  MemoryBlocks.push_back( new Directions[ BlockSize ]() );

  *( MemoryBlocks[ 0 ] ) = Directions{ Data, Directions_ };
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Memory::Memory( const std::vector< size_t > &BlockSizes, ListType &&Data,
                                                                  DirInitType &&Directions_ ) noexcept : 
BlockSize( 1 ), CurrentIndex( 1 )
{
  for( auto Iter : BlockSizes )
  {
    BlockSize *= Iter;
  }

  MemoryBlocks.push_back( new Directions[ BlockSize ]() );

  *( MemoryBlocks[ 0 ] ) = Directions{ std::move( Data ), std::move( Directions_ ) };
}

template< typename ListType, size_t DirectionCount >
MultiDirectionalList< ListType, DirectionCount >::Memory::~Memory() noexcept
{
  EmptyBlocks.clear();

  for( auto Iter : MemoryBlocks )
  {
    delete [] Iter;
  }

  MemoryBlocks.clear();
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions *
MultiDirectionalList< ListType, DirectionCount >::Memory::operator=( std::pair< size_t, Directions > &&DataCount ) noexcept
{
  BlockSize = DataCount.first;

  CurrentIndex = 1;

  EmptyBlocks.clear();

  for( auto Iter : MemoryBlocks )
  {
    delete[] Iter;
  }

  MemoryBlocks.clear();

  MemoryBlocks.push_back( new Directions[ BlockSize ]() );


  MemoryBlocks[ 0 ][ 0 ] = Directions{ std::move( DataCount.second ) };

  return MemoryBlocks[ 0 ];
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions *
MultiDirectionalList< ListType, DirectionCount >::Memory::operator=( std::pair< std::vector< size_t >, Directions > &&DataCount ) noexcept
{
  BlockSize = 1;

  for( auto Iter : DataCount.first )
  {
    BlockSize *= Iter;
  }

  CurrentIndex = 1;

  EmptyBlocks.clear();

  for( auto Iter : MemoryBlocks )
  {
    delete[] Iter;
  }

  MemoryBlocks.clear(); 

  MemoryBlocks.push_back( new Directions[ BlockSize ]() );

  MemoryBlocks[ 0 ][ 0 ] = Directions{ std::move( DataCount.second ) };

  return MemoryBlocks[ 0 ];
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions * 
MultiDirectionalList< ListType, DirectionCount >::Memory::GetTop() noexcept
{
  return MemoryBlocks[ 0 ];
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions * 
MultiDirectionalList< ListType, DirectionCount >::Memory::GetBlock( size_t Index ) noexcept
{
  return MemoryBlocks[ Index / BlockSize ] + ( Index % BlockSize );
}

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions *
MultiDirectionalList< ListType, DirectionCount >::Memory::SetBlock( Directions &&Dir ) noexcept
{
  if( EmptyBlocks.empty() )
  {
    const size_t Index = CurrentIndex / BlockSize;

    const size_t MemSize = MemoryBlocks.size();

    if( Index < MemSize )
    {
      Directions *const Return = MemoryBlocks[ Index ] + ( CurrentIndex % BlockSize );

      *Return = std::move( Dir );

      ++CurrentIndex;

      return Return;
    }

    ++CurrentIndex;

    Directions *const Return = new Directions[ BlockSize ]() ;

    MemoryBlocks.push_back( Return );

    Return[ 0 ] = std::move( Dir );

    return Return;
  }

  const auto Iter = EmptyBlocks.cbegin();

  Directions *const Return = GetBlock( *Iter );

  EmptyBlocks.erase( Iter );

  *Return = std::move( Dir );

  return Return;
}

template< typename ListType, size_t DirectionCount >
void MultiDirectionalList< ListType, DirectionCount >::Memory::Erase( size_t Index ) noexcept
{
  if( Index == CurrentIndex )
  {
    --CurrentIndex;

    return;
  }

  const auto End = EmptyBlocks.cend();

  while( EmptyBlocks.find( Index ) != End )
  {
    ++Index;
  }

  EmptyBlocks.emplace( Index );
}

template <typename ListType, size_t DirectionCount>
void MultiDirectionalList<ListType, DirectionCount>::Memory::Reset() noexcept
{
  BlockSize = 0;

  CurrentIndex = 0;

  EmptyBlocks.clear();

  for( auto Iter : MemoryBlocks )
  {
    delete[] Iter;
  }

  MemoryBlocks.clear();
}

///////////////////////////////////////////////////////////////////////////////

template< typename ListType, size_t DirectionCount >
typename MultiDirectionalList< ListType, DirectionCount >::Directions *
MultiDirectionalList< ListType, DirectionCount >::MoveTo( Directions *Start, size_t EndIndex ) const ExceptDebug
{
  AssertDebug( EndIndex > Size, ( MovedTooFar{ EndIndex, Size } ) );

  size_t DirIndex = DirectionCount - 1;

  Direction Dir = DirectionalHead->DirectionArray[ DirIndex ].Dir;

  const Direction Zero{ std::vector< float >( DimensionSizes.size() ) };

  while( Zero < Dir )
  {
    const size_t End = static_cast< size_t >( std::abs( Dir.Calc( DimensionSizes ) ) );

    while( End <= EndIndex )
    {
      Directions *Temp = Start->DirectionArray[ DirIndex ].NextPtr;

      if( !Temp )
      {
        break;
      }

      Start = Temp;

      EndIndex -= End;
    }

    if( EndIndex )
    {
      Dir = DirectionalHead->DirectionArray[ --DirIndex ].Dir;
    }
    else
    {
      return Start;
    }
  }

  for( size_t i = 0; i < EndIndex; ++i )
  {
    Start = Start->Next.NextPtr;
  }

  return Start;
}

template< typename ListType, size_t DirectionCount >
void MultiDirectionalList< ListType, DirectionCount >::InvalidateIterators() noexcept
{
  Directions *Iter = DirectionalHead;

  for( size_t i = 0; i < Size; ++i )
  {
    for( size_t j = 0; j < DirectionCount; ++j )
    {
      Iter->DirectionArray[ j ].NextPtr = nullptr;
    }

    Iter = Iter->Next.NextPtr;
  }
}
