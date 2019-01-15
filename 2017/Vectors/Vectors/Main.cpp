#include "Vector.h"

int main()
{
  const auto IntVec = MakeVector( { 1, 2, 3, 4, 5, 6, 7, 8 } );
  Vector< int, 4 > ConstIntVec{ 1, 2, 3, 4 };
  Vector< int, 4 > ConstIntVec1{};
  Vector< float, 4 > ConstIntVec2(1.2);
  //auto ConstIntVec2 = MakeVector( { 1, 2, 3 } );

  //return ConstIntVec1.w;
  return ( ConstIntVec1 + ConstIntVec2 ).w;
  //return IntVec[ 1 ] + IntVec[ 3 ] + IntVec[ 5 ] + IntVec[ 7 ];
  //return ConstIntVec.x + ConstIntVec.y + ConstIntVec.z + ConstIntVec.w;
}