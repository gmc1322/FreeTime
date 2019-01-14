
#include "BitField.h"

int main()
{
  BitField< 20 > b1, b2;

  std::cout << "b1.Sizeof      " << b1.Sizeof() << std::endl;

  b1.Set( 0 ); b1.Set( 19 );
  b2.Set( 9 ); b2.Set( 10 );

  std::cout << "b1             " << b1 << std::endl;
  std::cout << "b2             " << b2 << std::endl;
 
  auto b3{ b1 | b2 };

  std::cout << "b3 = b1 | b2   " << b3               << std::endl;
  std::cout << "~b3, b3        " << ~b3 << " " << b3 << std::endl;
  std::cout << "!b3, b3        " << !b3 << " " << b3 << std::endl;
  std::cout << "b3.Number    = " << b3.Number()      << std::endl;
  std::cout << "(!b3).Number = " << ( !b3 ).Number() << std::endl;

  BitField< 65 > b4;

  b4.Set( 63 );

  std::cout << "b4             " << b4          << std::endl;
  std::cout << "b4.Number      " << b4.Number() << std::endl;

  b4.Clear( 63 );
  b4.Set( 64 );

  std::cout << "b4             " << b4          << std::endl;
  std::cout << "b4.Number      " << b4.Number() << std::endl;

  std::cout << "b3.Get<8,11>   " << ( b3.Get< 8, 11 >() ) << std::endl;

  std::cout << "(!b3).Get<8,11>" << ( (!b3).Get< 8, 11 >() ) << std::endl;

  std::cout << "Bin output b3  " << BitOutput{ BitOutput::Binary } << b3 << std::endl;
  std::cout << "Oct output b3  " << BitOutput{ BitOutput::Octal }  << b3 << std::endl;
  std::cout << "Hex output b3  " << BitOutput{ BitOutput::Hex }    << b3 << std::endl;
  std::cout << "5   output b3  " << BitOutput{ 5 }                 << b3 << std::endl;

  auto b5{ ( BitField< 5 >{ 1,1,0,1,  1 } + BitField< 5 >{ 1,  1,0,1,1 } ) };

  std::cout << "b5=11011+11011 " << b5       << std::endl;
  std::cout << "b1 + b2        " << b1 + b2 << std::endl;

  BitField< 10 > b6{ 1,1,0,1,   1,1,0,0,   1,1 };
  std::cout << "b6             " << BitOutput{ BitOutput::Hex } << b6 << std::endl;
  std::cout << "b6 << 3        " << BitOutput{ BitOutput::Hex } << ( b6 << 3 ) << std::endl;

  std::cout << "b4 = b5        " << ( b4 = b5 ) << std::endl;

  BitField< 8 > b7{ 1,0,0,1,  1,0,0,1 }, b8{ 0,1,1,0,  0,1,1,0 };
  std::cout << "b7             " << b7 << std::endl;
  std::cout << "b8             " << b8 << std::endl;

  auto b9 = ( b7 + b8 );
  std::cout << "b7 + b8                                " << b9                                             << std::endl;
  std::cout << "b9.Number({{0,1},{4,5},{8,9},{12,13}}) " << b9.Number( { {0, 1}, {4,5}, {8,9}, {12,13} } ) << std::endl;

  auto b10 = ( b4 + b1 );
  std::cout << "b10                   " << b10 << std::endl;
  std::cout << "b10.Number({{0, 85}}) " << b10.Number( { {0, 85} } ) << std::endl;

  return 0;
}
