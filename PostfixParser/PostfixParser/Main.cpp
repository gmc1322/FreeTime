
#include <iostream>
#include <array>

#include "InfixParser.h"

template< size_t ArraySize >
std::string FitData( const std::array< std::pair< float, float >, ArraySize > &DataArray )
{
  float MeanX = 0, MeanY = 0;

  for( const auto &Iter : DataArray )
  {
    MeanX += Iter.first;
    MeanY += Iter.second;
  }

  MeanX /= ArraySize;
  MeanY /= ArraySize;

  float Slope = 0;

  for( const auto &Iter : DataArray )
  {
    Slope += ( Iter.first - MeanX ) * ( Iter.second - MeanY );
  }

  float Diviser = 0;

  for( const auto &Iter : DataArray )
  {
    const float Temp = Iter.first - MeanX;

    Diviser += Temp * Temp;
  }

  Slope /= Diviser;

  std::string Formula = std::to_string( Slope ) + "*X+" + std::to_string( MeanY - Slope * MeanX );

  return Formula; // Return Value Optimization (RVO)
}


int main()
{
  const std::array< std::pair< float, float >, 5 > Data{ { {10.f,10.f},{11.f,11.f},{12.f,12.f},{13.f,5.f},{14.f,-2.f} } };

  const std::string Formula = FitData( Data );

  std::cout << Formula << std::endl;

  InfixParser Parser{ Formula };

  for( const auto &Iter : Data )
  {
    std::cout << Parser.Evaluate( { { "X", Iter.first } } ) << std::endl;
  }

  return 0;
}
