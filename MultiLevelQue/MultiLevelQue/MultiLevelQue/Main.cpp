#include "MultiLevelQue.h"

#include <iostream>

struct Test
{
  public:
    bool operator<( const Test &Rhs ) const noexcept
    {
      return value < Rhs.value;
    }
  
    bool operator!=( const Test &Rhs ) const noexcept
    {
      return value != Rhs.value;
    }
  
    bool operator==( const Test &Rhs ) const noexcept
    {
      return value == Rhs.value;
    }

    void operator++() noexcept
    {
      ++value;
    }

    operator std::string() const noexcept
    {
      return "Test" + std::to_string( value );
    }

  public:
    int value;
};

int main()
{
  auto MyQue{ MakeMultiLevelQue( 5.5f, 0, 0, Test{ 1 } ) };

  typedef decltype( MyQue ) MyQueType;

  std::cout << MyQue.Top() << std::endl;

  MyQue.Emplace( 2.2f, 0, -1, Test{ 1 } );

  std::cout << MyQue.Top() << std::endl;

  MyQue.Emplace( -2.2f , -1, -1, Test{ 1 } );

  std::cout << MyQue.Top() << std::endl;

  MyQue.Emplace( 3.2f, 1, -1, Test{ 1 } );

  std::cout << MyQue.Top() << std::endl;

  if( auto Ptr = MyQue.Find_NoThrow( 1, -1, Test{ 1 } ) )
  {
    std::cout << *Ptr << std::endl;
  }

  if( auto Ptr = MyQue.Find_NoThrow( 1, -2, Test{ 1 } ) )
  {
    std::cout << *Ptr << std::endl;
  }

  MyQue.Emplace( 5.2f, 1, -1, Test{ 1 } );
  MyQue.Emplace( 7.2f, 1, -1, Test{ 1 } );

  MyQue.EmplaceAfter( 5.2f, 1, -1, Test{ 1 } );
  MyQue.EmplaceAfter( 7.2f, 1, -1, Test{ 1 } );

  try
  {
    std::cout << MyQue.Find( 1, -1, Test{ 2 } ) << std::endl;
  }
  catch( MyQueType::CouldNotFind &Error )
  {
    std::cout << Error.What << std::endl;
  }

  for( auto Iter : MyQue.FindRange( 1, -1, Test{ 1 } ) )
  {
    std::cout << Iter << std::endl;
  }

  auto NewQue = MyQue;

  std::cout << std::endl;

  for( auto Iter : NewQue.FindRange( 1, -1, Test{ 1 } ) )
  {
    std::cout << Iter << std::endl;
  }

  auto NewMoveQue = std::move( MyQue );

  std::cout << std::endl;

  for( auto Iter : NewMoveQue.FindRange( 1, -1, Test{ 1 } ) )
  {
    std::cout << Iter << std::endl;
  }

  std::cout << "Empty ? " << MyQue.Empty() << std::endl;

  NewMoveQue.Pop();

  std::cout << NewMoveQue.Top() << std::endl;

  NewMoveQue.Erase( 0, -1, Test{ 1 } );
  NewMoveQue.Erase( 0, 0, Test{ 1 } );

  NewMoveQue.EraseRange( 1, -1, Test{ 1 } );

  std::cout << "Empty ? " << NewMoveQue.Empty() << std::endl;
  std::cout << "Empty ? " << NewQue.Empty() << std::endl;

  NewQue.Emplace( 10.f, 1, -1, Test{ 2 } );
  NewQue.Emplace( 9.0f, 1, -1, Test{ 2 });

  NewQue.Emplace( 12.f, 1, -1, Test{ 3 } );
  NewQue.Emplace( 11.f, 1, -1, Test{ 3 } );

  NewQue.Emplace( 14.f, 1, -1, Test{ 4 } );
  NewQue.Emplace( 13.f, 1, -1, Test{ 4 } );

  NewQue.Emplace( 16.f, 1, -1, Test{ 5 } );
  NewQue.Emplace( 15.f, 1, -1, Test{ 5 } );

  for( auto Iter : NewQue.FindMultipleRanges( 1, -1, Test{ 1 }, MakeQueRange( 1, -1, Test{ 5 } ) ) )
  {
    for( auto InnerIter : Iter )
    {
      std::cout << InnerIter << ", ";
    }

    std::cout << std::endl;
  }

  std::cout << std::endl;

  for( auto Iter : NewQue.FindMultiple( 1, -1, Test{ 1 }, QueRange< MyQueType::OuterKeyType, MyQueType::InnerKeyType,
                                                                    MyQueType::InnerQueType::InnerKeyType >{ 1, -1, Test{ 5 } } ) )
  {
    std::cout << Iter << std::endl;
  }

  std::cout << std::endl;

  for( auto Iter : NewQue )
  {
    std::cout << Iter << ", ";
  }

  std::cout << std::endl << std::endl;

  for( auto Iter : NewQue.GetQue< MyQueType::InnerKeyType, MyQueType::InnerQueType::InnerKeyType >( 1 ) )
  {
    std::cout << Iter << ", ";
  }
  
  std::cout << std::endl;

  for( auto Iter : NewQue.GetQue< MyQueType::InnerQueType::InnerKeyType >( 1, -1 ) )
  {
    std::cout << Iter << ", ";
  }

  return 0;
}