#include "BehaviorTree_Stack.h"

using namespace BehaviorTreeSpace;

StackClass::StackClass() noexcept
{
}

StackClass::~StackClass()
{
  delete[] Stack;
}

void StackClass::Push( UnionClassPtr Ptr ) noexcept
{
  if( Size == Capacity )
  {
    Grow();
  }

  Stack[ Size ] = Ptr;

  ++Size;
}

void StackClass::Pop() ThrowingElse( noexcept( false ), noexcept )
{
  Throwing(
    if( Empty() )
    {
      throw( EmptyStackThrow( "Tried to remove an item from an empty stack!" ) );
    }
  )

  --Size;
}

UnionClassPtr StackClass::Top() const ThrowingElse( noexcept( false ), noexcept )
{
  Throwing(
    if( Empty() )
    {
      throw( EmptyStackThrow( "Tried to access the top of an empty stack!" ) );
    }
  )

  return Stack[ Size - 1 ];
}

bool StackClass::Empty() const noexcept
{
  return ( Size == 0 );
}

void StackClass::Clear() noexcept
{
  Size = 0;
}

void StackClass::Grow() noexcept
{
  Capacity = static_cast< size_t >( ceil( Capacity * 1.75 ) );

  auto Temp = new UnionClassPtr[ Capacity ];

  for( size_t i = 0; i < Size; ++i )
  {
    Temp[ i ] = Stack[ i ];
  }

  delete [] Stack;

  Stack = Temp;
}
