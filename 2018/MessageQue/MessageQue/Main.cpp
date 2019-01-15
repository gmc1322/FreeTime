
#include "MessageQue.h"

int main()
{
  {
    MessageQue Temp{ "C:\\Users\\garre\\Desktop\\garrett.conti\\CS180\\MessageQue\\Debug\\ExampleProcess.exe", "TestPipe", 1, 5 };

    Temp.SendMsg( "3" );

    Sleep( 500 );

    Temp.SendMsg( "4" );

    Sleep( 10500 ); // Minus 1000 (1s)

    Temp.SendMsg( "5" );

    Temp.SendMsg( "6" );

    Temp.SendMsg( "7" );
  }

  return 0;
}