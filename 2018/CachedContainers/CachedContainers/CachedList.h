/*!------------------------------------------------------------------------------
\file   CachedList.h

\author Garrett Conti

\par    Project: CachedContainers
\par    Course:  Fun
   ------------------------------------------------------------------------------ */

#pragma once

class CachedList
{
  public:
    explicit CachedList( size_t ReserveSize ) noexcept;

  public:
    void Emplace( int Data ) noexcept;
    void Find( int Data ) noexcept;
    void Remove( int Data ) noexcept;

  private:
    struct Node
    {
      int Data;

      Node *Next;
    };

  private:
    Node *NodeArray;

    Node *FreePtr = nullptr;
    Node *DataPtr = nullptr;
};
