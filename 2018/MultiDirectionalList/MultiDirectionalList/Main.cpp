#include "MultiDirectionalList.h"

int main()
{
  for( size_t j = 0; j < 100; ++j )
  {
    {
      MultiDirectionalList< const size_t, 3 > Test{ { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15, 16,17,18,19 },
                                                    { { { { 180 } } }, { { { 90 } } }, { { { -90 } } } }, { 4 } };

      std::cout << *( Test.Move( 10 ) ) << std::endl;

      std::cout << *( Test.begin().Move( 14 ) ) << std::endl;

      auto Iter = Test.Move( { { 90 } }, 3 ).Move( { { -90 } }, 1 ).Move( { { 180 } }, 1 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.MoveBack( 4 ) ) << std::endl << std::endl;
    }
    
    {
      MultiDirectionalList< size_t, 3 > Test{ 0, { { { { 180 } } }, { { { 90 } } }, { { { -90 } } } }, { 4 } };

      for( size_t i = 1; i < 20; ++i )
      {
        Test.PushBack( i );
      }

      Test.ValidateIterators();

      std::cout << *( Test.Move( 10 ) ) << std::endl;

      std::cout << *( Test.begin().Move( 14 ) ) << std::endl;

      auto Iter = Test.Move( { { 90 } }, 3 ).Move( { { -90 } }, 1 ).Move( { { 180 } }, 1 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.MoveBack( 4 ) ) << std::endl << std::endl;
    }
    
    {
      MultiDirectionalList< size_t, 3 > Test{};

      Test.PushFront( 0, { { { { 180 } } }, { { { 90 } } }, { { { -90 } } } }, { 4 } );

      for( size_t i = 1; i < 20; ++i )
      {
        Test.PushBack( i );
      }

      Test.ValidateIterators();

      std::cout << *( Test.Move( 10 ) ) << std::endl;

      std::cout << *( Test.begin().Move( 14 ) ) << std::endl;

      auto Iter = Test.Move( { { 90 } }, 3 ).Move( { { -90 } }, 1 ).Move( { { 180 } }, 1 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.MoveBack( 4 ) ) << std::endl << std::endl;
    }

    {
      MultiDirectionalList< size_t, 3 > Test{};

      Test.PushFront( 0, { { { { 180 } } }, { { { 90 } } }, { { { -90 } } } }, { 4 } );

      for( size_t i = 1; i < 11; ++i )
      {
        Test.PushBack( i );
      }

      Test.ValidateIterators();

      std::cout << *( Test.Move( 10 ) ) << std::endl;

      for( size_t i = 11; i < 20; ++i )
      {
        Test.PushBack( i );
      }

      Test.ValidateIterators();

      std::cout << *( Test.begin().Move( 14 ) ) << std::endl;

      auto Iter = Test.Move( { { 90 } }, 3 ).Move( { { -90 } }, 1 ).Move( { { 180 } }, 1 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.MoveBack( 4 ) ) << std::endl << std::endl;
    }

    {
      MultiDirectionalList< size_t, 3 > Test{ { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15, 16,17,18,19 },
                                              { { { { 180 } } }, { { { 90 } } }, { { { -90 } } } }, { 4 } };

      std::cout << *( Test.Erase( 9 ).ValidateIterators().Move( 9 ) ) << std::endl;

      std::cout << *( Test.InsertBefore( 9, 9 ).ValidateIterators().begin().Move( 14 ) ) << std::endl;

      Test.PopBack().ValidateIterators();

      auto Iter = Test.Move( { { 90 } }, 3 ).Move( { { -90 } }, 1 ).Move( { { 180 }  }, 1 );

      std::cout << *( Iter ) << std::endl;

      Test.PushFront( 0 );

      std::cout << *( Iter.MoveBack( 4 ) ) << std::endl << std::endl;

      Test.Resize( 0 );

      Test.PushFront( { 0,1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15, 16,17,18,19 }, { { { { 180 } } },{ { { 90 } } }, { { { -90 } } } }, 
                      { 4 } );

      Test.ValidateIterators();

      std::cout << *( Test.Move( 9 ) ) << std::endl;

      std::cout << *( Test.begin().Move( 5 ) ) << std::endl;

      Iter = Test.Move( { { 180 } }, 1 ).Move( { { 90 } }, 1 ).Move( 3 ).Move( { { -90 } }, 3 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.Move( 4 ) ) << std::endl << std::endl;
    }

    {
      MultiDirectionalList< size_t, 3 > Test{};

      Test.PushFront( 0, { { { { 180 } } }, { { { 90 } } }, { { { -90 } } } }, { 4 } );

      Test.InsertBefore( { 1,2,3, 4,5,6,7, 8,9,10,11, 12,13,14,15, 16,17,18,19 }, 0 );

      Test.ValidateIterators();

      std::cout << *( Test.Move( 10 ) ) << std::endl;

      std::cout << *( Test.begin().Move( 6 ) ) << std::endl;

      auto Iter = Test.Move( { { 180 } }, 1 ).Move( { { 90 } }, 2 ).Move( { { -90 } }, 3 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.Move( 4 ) ) << std::endl << std::endl;

      Test.SetDimensions( { { { { 180 } } },{ { { 90 } } },{ { { -90 } } } }, { 5 } );

      std::cout << *( Test.Move( 10 ) ) << std::endl;

      std::cout << *( Test.begin().Move( 6 ) ) << std::endl;

      Iter = Test.Move( { { 180 } }, 1 ).Move( { { 90 } }, 2 ).Move( { { -90 } }, 3 );

      std::cout << *Iter << std::endl;

      std::cout << *( Iter.Move( 4 ) ) << std::endl << std::endl;
    }
  }

  return 0;
}
