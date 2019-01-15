/*!------------------------------------------------------------------------------
\file   CachedList.cpp

\author Garrett Conti

\par    Project: CachedContainers
\par    Course:  Fun
   ------------------------------------------------------------------------------ */


#include "CachedList.h"
#include <algorithm>

CachedList::CachedList( const size_t ReserveSize ) noexcept : NodeArray( new Node[ ReserveSize ] )
{
  FreePtr = NodeArray;

  const size_t End = ReserveSize - 1;

  for( size_t i = 0; i < End; ++i )
  {
    NodeArray[ i ].Next = NodeArray + ( i + 1 );
  }

  NodeArray[ End ].Next = nullptr;
}

void CachedList::Emplace( const int Data ) noexcept
{
  Node *const Temp = DataPtr;

  DataPtr = FreePtr;
  FreePtr = FreePtr->Next;

  DataPtr->Data = Data;
  DataPtr->Next = Temp;
}

void CachedList::Find( const int Data ) noexcept
{
  Node *Temp = DataPtr;
  Node *Prev = nullptr;

  while( Temp->Data != Data )
  {
    Prev = Temp;
    Temp = Temp->Next;
  }

  if( Prev )
  {
    Prev->Next = Temp->Next;

    Node *const TempData = DataPtr;

    DataPtr = Temp;
    DataPtr->Next = TempData;
  }
}

void CachedList::Remove( const int Data ) noexcept
{
  Node *Temp = DataPtr;
  Node *Prev = nullptr;

  while( Temp->Data != Data )
  {
    Prev = Temp;
    Temp = Temp->Next;
  }

  if( Prev )
  {
    Prev->Next = Temp->Next;
  }
  else
  {
    DataPtr = DataPtr->Next;
  }

  Node *const TempFree = FreePtr;

  FreePtr = Temp;
  FreePtr->Next = TempFree;
}
