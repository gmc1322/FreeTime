
#include "Serializer.h"

class Test
{
  public:
    Test() noexcept {}

    Test( const Serializer::SerializerData &Data ) noexcept
    {
      const size_t FourOffset = Data.find( " " );

      Three = atoi( Data.substr( 0, FourOffset ).c_str() );
      FourOne = static_cast< float >( atof( Data.substr( FourOffset + 1 ).c_str() ) );
    }
  
    Test( const Serializer::DataInfo &Data ) noexcept : Test( Data.Data ) {}

  public:
    operator Serializer::SerializerData() const noexcept
    {
      Serializer::SerializerData Return{ std::to_string( Three ) + std::string{ " " } +std::to_string( FourOne ) };
      return Return; // Split for Named RVO
    }
  
    operator Serializer::DataInfo() const noexcept
    {
      Serializer::DataInfo Return{ std::to_string( Three ), std::to_string( FourOne ), *this };
      return Return; // Split for Named RVO
    }

  private:
    int Three = 3;
    float FourOne = 4.1f;
};

int main()
{
  Serializer TestSaver{ "TestFile" };

  Serializer::DataInfo TestData1{ "float", "one" };
  Serializer::DataInfo TestData2{ "int", "one" };
  Serializer::DataInfo TestDataClass{ "3", std::to_string( 4.1f ) };

  TestSaver << Serializer::CreateSaveData( "int", "one", 1 );
  TestSaver << Serializer::CreateSaveData( "float", "one", 1.0 );
  TestSaver << Serializer::CreateSaveData( "int", "two", 2 );

  TestSaver << Test{};

  TestSaver.Save();

  TestSaver >> TestData1;
  TestSaver >> TestData2;
  TestSaver >> TestDataClass;

  Test TestClass1{ TestDataClass.Data };
  Test TestClass2{ TestDataClass };

  return 0;
}