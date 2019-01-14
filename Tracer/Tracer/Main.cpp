
#include "Tracer.h"

int main()
{
  Tracer Test{ "TestFile" };

  constexpr Tracer::Importance ImportanceLevel = Tracer::Verbose;
  constexpr Tracer::Type MessageType = Tracer::Error;

  Test << ImportanceLevel << MessageType << "I am printing in: " << std::to_string( ImportanceLevel ) << " and my message type is: ";
  Test << std::to_string( MessageType ) << std::endl;

  Test << Tracer::Warning << "Testing Other Stuff " << std::endl;

  Test.SetImportance( Tracer::Release );

  Test << "Release Only" << std::endl;

  return 0;
}
