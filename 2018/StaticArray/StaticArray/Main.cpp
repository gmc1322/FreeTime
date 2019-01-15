
#include <iostream>
#include <array>
#include "StaticArray.h"

template< typename ArrayType, size_t FirstSize, size_t ... SizePack >
void ArrayFunction( const StaticArray< ArrayType, FirstSize, SizePack... > &Array )
{
  size_t i = 0;

  for( ; i < FirstSize; ++i )
  {
    std::cout << Array[ i ] << ",";
  }

  DebugOnly(
    try
    {
      Array[ i ];
    }
    catch( ... ){}
  )

  std::cout << std::endl;

  /*if( sizeof...( SizePack ) ) // Only needed if you dont specialize the function
  {
    for( i = 0; i < FirstSize; ++i )
    {
      ArrayFunction( Array[ i ] );
    }
  }*/

  for( i = 0; i < FirstSize; ++i )
  {
    ArrayFunction( Array[ i ] );
  }
}

template< typename ArrayType, size_t LastSize >
void ArrayFunction( const StaticArray< ArrayType, LastSize > &Array )
{
  size_t i = 0;

  for( ; i < LastSize; ++i )
  {
    std::cout << Array[ i ] << ",";
  }

  DebugOnly(
    try
    {
      Array[ i ];
    }
    catch( ... ){}
  )

  std::cout << std::endl;
}

template< typename Type >
void ArrayPtrFunction( const Type &Ptr )
{
  std::cout << Ptr << "," << std::endl;
}

template< typename PointerType, size_t Size >
void ArrayPtrFunction( const StaticArrayPtr( PointerType, Size ) &Ptr )
{
  size_t i = 0;

  for( ; i < Size; ++i )
  {
    std::cout << Ptr[ i ] << ",";
  }

  DebugOnly(
    try
    {
      Ptr[ i ];
    }
    catch( ... ){}
  )

  std::cout << std::endl;

  //if( std::is_constructible< decltype( Ptr[ i ] ), PointerType, size_t >::value ) // Only needed if you dont specialize the function
  //{
  //  for( i = 0; i < Size; ++i )
  //  {
  //    ArrayPtrFunction( Ptr[ i ] );
  //  }
  //}

  for( i = 0; i < Size; ++i )
  {
    ArrayPtrFunction( Ptr[ i ] );
  }
}

int main()
{
        int Array0[ 5 ];
  const int Array1[ 5 ] = { 6, 5 };
        int Array2[ 5 ]{ 5, 6 };
  const int * Array3[ 5 ]{ &( Array1[ 0 ] ), &( Array1[ 1 ] ), &( Array1[ 2 ] ), &( Array1[ 3 ] ), &( Array1[ 4 ] ) };
  //const int & Array3_1[ 5 ]{ Array1[ 0 ], Array1[ 1 ], Array1[ 2 ], Array1[ 3 ], Array1[ 4 ] }; // Error
  //int Array4[ 5 ]{ Array2 }; // Error

        StaticArray< int, 5 > StaticArray0;
  const StaticArray< int, 5 > StaticArray1 = { 6, 5 };
        StaticArray< int, 5 > StaticArray2{ 6, 5 };
  const StaticArray< const int *, 5 > StaticArray3{ { &( StaticArray1[ 0 ] ), &( StaticArray1[ 1 ] ), &( StaticArray1[ 2 ] ),
                                                          &( StaticArray1[ 3 ] ), &( StaticArray1[ 4 ] ) } };

  std::cout << &StaticArray2;

  //StaticArray< int &, 5 > StaticArray3_1{ StaticArray1[ 0 ], StaticArray1[ 1 ], StaticArray1[ 2 ], StaticArray1[ 3 ], StaticArray1[ 4 ] }; // Error
  //StaticArray< int, 5 > StaticArray4{ StaticArray1 }; // Error

  for( auto Iter : Array1 )
  {
    std::cout << Iter << std::endl;
  }
  
  for( auto Iter : StaticArray1 )
  {
    std::cout << Iter << std::endl;
  }

  int Array01[ 5 ][ 5 ]{ {}, { 0, 1 } };
  int Array02[ 5 ][ 5 ]{ {}, { 2, 3 } };

  StaticArray< int, 5, 5 > StaticArray01{ {}, { 0, 1 } };
  StaticArray< int, 5, 5 > StaticArray02{ {}, { 2, 3 } };

  std::cout << &StaticArray02;

  for( auto Iter : Array02 )
  {
    std::cout << Iter << std::endl;
  }

  for( auto Iter : StaticArray02 )
  {
    std::cout << Iter << std::endl;
  }


  int *One = Array0;
  int *Two = Array2;
  One += 2;
  int Test = One - Two;
  std::cout << Test << std::endl << std::endl;
  std::cout << One[ -2 ] << std::endl << std::endl;


  StaticArrayPtr( ArrayType( StaticArray0 ), StaticArray0.Size() ) StaticOne = StaticArray0;
  StaticArrayPtr( ArrayType( StaticArray2 ), StaticArray2.Size() ) StaticTwo = StaticArray2;
  StaticOne += 2;
  int StaticTest = StaticOne - StaticTwo;
  std::cout << StaticTest << std::endl << std::endl;
  std::cout << StaticOne[ -2 ] << std::endl << std::endl;
  

  StaticArrayPtr( ArrayType( StaticArray01 ), StaticArray01.Size() ) StaticZeroOne = StaticArray01;
  StaticArrayPtr( ArrayType( StaticArray02 ), StaticArray02.Size() ) StaticZeroTwo = StaticArray02;
  StaticZeroOne += 2;
  int StaticTest2 = StaticZeroOne - StaticZeroTwo;
  std::cout << StaticTest2 << std::endl << std::endl;
  std::cout << StaticZeroOne[ -2 ] << std::endl << std::endl;
  

  StaticArrayPtr( ArrayType( StaticArray01 )::Type, StaticArray01.Size< 2 >() ) StaticZeroOneTwo = StaticArray01[ 0 ];
  StaticArrayPtr( ArrayType( StaticArray01 )::Type, StaticArray02.Size< 2 >() ) StaticZeroTwoTwo = StaticArray02[ 0 ];
  StaticZeroOneTwo += 2;
  int StaticTest3 = StaticZeroOneTwo - StaticZeroTwoTwo;
  std::cout << StaticTest3 << std::endl << std::endl;
  std::cout << StaticZeroOneTwo[ -2 ] << std::endl << std::endl;


  std::cout << *( ( Array2 + 3 ) - 2 ) << std::endl;
  std::cout << *( ( StaticArray2 + 3 ) - 2 ) << std::endl << std::endl;


  std::cout << Array0[ 1 ] << std::endl;
  std::cout << Array1[ 1 ] << std::endl;
  std::cout << Array2[ 1 ] << std::endl; 
  std::cout << Array3[ 1 ] << std::endl << std::endl;
  
  std::cout << StaticArray0[ 1 ] << std::endl;
  std::cout << StaticArray1[ 1 ] << std::endl;
  std::cout << StaticArray2[ 1 ] << std::endl;
  std::cout << StaticArray3[ 1 ] << std::endl << std::endl;


  std::cout << *( Array0 + 1 ) << std::endl;
  std::cout << *( Array1 + 1 ) << std::endl;
  std::cout << *( Array2 + 1 ) << std::endl;
  
  std::cout << *( StaticArray0 + 1 ) << std::endl;
  std::cout << *( StaticArray1 + 1 ) << std::endl;
  std::cout << *( StaticArray2 + 1 ) << std::endl << std::endl;


  std::cout << *( Array01 + 1 ) << std::endl;
  std::cout << *( *( Array01 + 1 ) + 1 ) << std::endl;
  std::cout << *( *( Array01 + 1 ) ) << std::endl;
  
  std::cout << *( StaticArray01 + 1 ) << std::endl;
  std::cout << *( *( StaticArray01 + 1 ) + 1 ) << std::endl;
  std::cout << *( *( StaticArray01 + 1 ) ) << std::endl << std::endl;
  

  std::cout << Array01[ 1 ] << std::endl;
  std::cout << Array01[ 1 ][ 1 ] << std::endl;
  std::cout << Array01[ 1 ][ 0 ] << std::endl;
  std::cout << *( Array01[ 1 ] ) << std::endl;
  
  std::cout << StaticArray01[ 1 ] << std::endl;
  std::cout << StaticArray01[ 1 ][ 1 ] << std::endl;
  std::cout << StaticArray01[ 1 ][ 0 ] << std::endl;
  std::cout << *( StaticArray01[ 1 ] ) << std::endl << std::endl;

  ArrayFunction( StaticArray3 );
  std::cout << std::endl;

  ArrayFunction( StaticArray02 );
  std::cout << std::endl;

  ArrayPtrFunction( StaticArray3 );
  std::cout << std::endl;

  ArrayPtrFunction( StaticArray02 );
  std::cout << std::endl << std::endl;


  StaticArray< int, 5 > StaticArray10{ std::vector< int >{ 0, 1, 2, 3, 4 } };

  for( auto Iter : StaticArray10 )
  {
    std::cout << Iter << std::endl;
  }

  std::cout << std::endl;

  for( int *Iter = StaticArray10.begin(), *End = StaticArray10.end(); Iter != End; ++Iter )
  {
    std::cout << *Iter << std::endl;
  }

  return 0;
}
