
#include "InitializerArray.h"

#include <iostream>
#include <chrono>

#define MarkupSubtract( Name ) ( static_cast< int >( ETextMarkup::##Name ) - static_cast< int >( ETextMarkup::Bold ) )
#define MarkupPair( Name ) Name, MarkupSubtract( Name )

enum class ETextMarkup : int
{
  Bold = 'b',
  Italic = 'i',
  StrikeThrough = 's',
  Underline = 'u',

  Last
};

static constexpr size_t Bold          = 1 << 0;
static constexpr size_t Italic        = 1 << 1;
static constexpr size_t StrikeThrough = 1 << 2;
static constexpr size_t Underline     = 1 << 3;

size_t Func( const ETextMarkup Character )
{
  switch( Character )
  {
    case ETextMarkup::Bold:
      return Bold;

    case ETextMarkup::Italic:
      return Italic;

    case ETextMarkup::StrikeThrough:
      return StrikeThrough;

    case ETextMarkup::Underline:
      return Underline;
  }
}

static constexpr ETextMarkup Array[] = { ETextMarkup::Bold, ETextMarkup::Italic, ETextMarkup::StrikeThrough, ETextMarkup::Underline };

int main()
{
  constexpr InitializedArray< MarkupSubtract( Last ), MarkupPair( Underline ), MarkupPair( StrikeThrough ), MarkupPair( Italic ), MarkupPair( Bold ) > MarkupMasks{};

  std::cout << MarkupMasks[ MarkupSubtract( Bold ) ] << std::endl;
  std::cout << MarkupMasks[ MarkupSubtract( Italic ) ] << std::endl;
  std::cout << MarkupMasks[ MarkupSubtract( StrikeThrough ) ] << std::endl;
  std::cout << MarkupMasks[ MarkupSubtract( Underline ) ] << std::endl;


  std::cout << Func( ETextMarkup::Bold ) << std::endl;
  std::cout << Func( ETextMarkup::Italic ) << std::endl;
  std::cout << Func( ETextMarkup::StrikeThrough ) << std::endl;
  std::cout << Func( ETextMarkup::Underline ) << std::endl;

  constexpr size_t LoopSize = 100'00'000;

  {
    const auto Start = std::chrono::steady_clock::now();
    for( size_t i = 0; i < LoopSize; ++i )
    {
      MarkupMasks[ static_cast< int >( Array[ ( std::rand() % 4 ) ] ) - static_cast< int >( ETextMarkup::Bold ) ];
    }
    const auto End = std::chrono::steady_clock::now();

    std::cout << static_cast< std::chrono::duration< double > >( End - Start ).count() << std::endl;
  }

  {
    const auto Start = std::chrono::steady_clock::now();
    for( size_t i = 0; i < LoopSize; ++i )
    {
      Func( Array[ ( std::rand() % 4 ) ] );
    }
    const auto End = std::chrono::steady_clock::now();

    std::cout << static_cast< std::chrono::duration< double > >( End - Start ).count() << std::endl;
  }

  return 0;
}