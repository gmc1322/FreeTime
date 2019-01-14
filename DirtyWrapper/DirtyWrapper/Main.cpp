
#include <iostream>

#include "SharedDirtyWrapper.h"
#include "UniqueDirtyWrapper.h"

int main()
{
#if 0
  {
    SharedDirtyWrapper< int > Temp3{};

    {
      int *Test = new int( 5 );

      auto *Temp1 = new SharedDirtyWrapper< int >{ &Test };

      std::cout << "Test is: " << Test << " after passing to wrapper." << std::endl;

      SharedDirtyWrapper< int > Temp1Ref{ *Temp1 };

      std::cout << Temp1->GetData() << std::endl;

      Temp1->SetData( 2 );

      if( Temp1Ref.IsDataDirty() )
      {
        std::cout << "Data dirty, new data is: " << Temp1Ref.GetData() << std::endl;
      }

      delete Temp1;

      std::cout << "References Left: " << Temp1Ref.GetReferenceCount() << std::endl;
      std::cout << "Data is: " << Temp1Ref.GetData() << std::endl;

      std::cout << "Moving data" << std::endl;

      SharedDirtyWrapper< int > Temp2{ std::move( Temp1Ref ) };

      Temp2 = 10;

      SharedDirtyWrapper< int > Temp2Ref2{};

      {
        SharedDirtyWrapper< int > Temp2Ref1{ Temp2 };

        if( Temp2Ref1.IsDataDirty() )
        {
          std::cout << "Data dirty, new data is: " << Temp2Ref1.GetData() << std::endl;
        }

        Temp2Ref2 = Temp2Ref1;
      }

      std::cout << "Alocating new data" << std::endl;

      int *Test2 = new int( 20 );

      Temp2Ref2 = &Test2;

      std::cout << "References Left: " << Temp2Ref2.GetReferenceCount() << std::endl;

      if( Temp2.IsDataDirty() )
      {
        std::cout << "Data dirty, new data is: " << Temp2.GetData() << std::endl;
      }

      Temp3 = std::move( Temp2Ref2 );
    }

    std::cout << "References Left: " << Temp3.GetReferenceCount() << std::endl;
    std::cout << "Data is: " << Temp3.GetData() << std::endl;
  }
#endif

  {
    UniqueChildDirtyWrapper< int > Child1;

    {
      UniqueParentDirtyWrapper< int > Parent{ 5 };

      Child1 = Parent;

      {
        UniqueChildDirtyWrapper< int > Child2{ Parent };

        std::cout << "ChildValue: " << Child1.GetData() << std::endl;

        Parent.SetData( 2 );

        std::cout << "IsDirty?: " << Child2.GetIsDirty() << std::endl;
        std::cout << "ChildValue: " << Child2.GetData() << std::endl;
      }

      Parent.SetData( 7 );

      std::cout << "ChildValue: " << Child1.GetData() << std::endl;
    }

    std::cout << "IsDirty?: " << Child1.GetIsDirty() << std::endl;
    std::cout << "IsValid?: " << Child1.GetIsValidData() << std::endl;
  }

  return 0;
}
