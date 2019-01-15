
#include "MapRange.h"

int main()
{
  std::cout << MapRange( 5, MakeVector( { 0, 10 } ), MakeVector( { 0.0f, 1.0f } ) ) << std::endl;
  std::cout << MapRange( .5f, MakeVector( { 0.0f, 1.0f } ), MakeVector( { 0, 10 } ) );

  return 0;
}
