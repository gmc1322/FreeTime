#pragma once

#include <type_traits>
#include <vector>
#include <set>

#include "Tracer.h"
#include "StaticArray.h"

namespace MDLSpace
{
  struct Direction
  {
    public:
      Direction() = default;

      Direction( const std::vector< float > &DirectionValues ) noexcept;
      Direction(       std::vector< float > &&DirectionValues ) noexcept;

    public:
      bool operator<( const Direction &Rhs ) const noexcept;

    public:
      int Calc( const std::vector< size_t > &DimSizes ) const ExceptDebug;

    public:
      /**
       * \par This is the x/y dimension.
       * Every (+/-) 90 will go (forword/backword) one in the y dimension.
       *
       * Any number inbetween 0 and (+/-) 90 will jump an amount proportional to the x diminsions size.
       *
       * (IE. +45 will go forward half the size of the x diminsion)
       */
      std::vector< float > Angles = { 0.0f };
  };
}

/**
 * Do not count the NextPtr (Angles 0,0,0...) in the dimension count!
 * 
 * Do not include the (Angles 0,0,0...) direction when initializing!
 */
template< typename ListType, size_t DirectionCount >
class MultiDirectionalList
{
  private:
    struct Directions
    {
      public:
        struct DirectionPtr
        {
          public:
            DirectionPtr() noexcept = default;

            DirectionPtr( const MDLSpace::Direction &Dir, Directions *NextPtr = nullptr ) noexcept;

            DirectionPtr( const DirectionPtr &Copy ) noexcept;
            DirectionPtr(       DirectionPtr &&Move ) noexcept;

          public:
            DirectionPtr & operator=( const DirectionPtr &Copy ) noexcept;
            DirectionPtr & operator=(       DirectionPtr &&Move ) noexcept;

          public:
            bool operator<( const DirectionPtr &Rhs ) const noexcept;

          public:
            MDLSpace::Direction Dir{};

            Directions *NextPtr = nullptr;
        };

      public:
        typedef StaticArray< DirectionPtr, DirectionCount > DirInitType;

      public:
        Directions() noexcept = default;

        Directions( const ListType &Data,  const std::initializer_list< DirectionPtr > &Directions ) ExceptDebug;
        Directions( const ListType &Data,  const std::vector< DirectionPtr >           &Directions ) ExceptDebug;
        Directions(       ListType &&Data,       std::vector< DirectionPtr >           &&Directions ) ExceptDebug;

        Directions( const ListType &Data,  const DirInitType &Directions ) noexcept;
        Directions(       ListType &&Data, const DirInitType &Directions ) noexcept;
        Directions(       ListType &&Data,       DirInitType &&Directions ) noexcept;

        Directions( Directions &&Dir ) noexcept;

      public:
         void operator=( Directions &&Dir ) noexcept;

      public:
        size_t Find( const std::vector< float > &Direction ) noexcept;

      public:
        typename std::remove_const< ListType >::type Data = ListType();

        DirInitType DirectionArray{};

        DirectionPtr Next{ { { 0 } } }; //!< Default ptr for 0,0; Always needed.

      private:
        struct CouldNotFind
        {
          explicit CouldNotFind( const std::vector< float > &Angles ) noexcept;
        };
    };

  public:
    typedef typename Directions::DirectionPtr DirPtr;

    typedef typename Directions::DirInitType DirInitType;

    typedef std::vector< typename std::remove_const< ListType >::type > DataInitType;

  public:
    class Iterator
    {
      public:
        explicit Iterator( const std::vector< size_t > &DimensionSizes, Directions *Begin = nullptr  ) noexcept;

        Iterator( const Iterator &Rhs ) noexcept;
        Iterator(       Iterator &&Rhs ) noexcept;

      public:
        ListType & operator*() const noexcept;
        ListType * operator->() const noexcept;

        Iterator & operator++() noexcept;

        bool operator!=( const Iterator &Rhs ) const noexcept;

        Iterator & operator=( const Iterator &Rhs ) noexcept;
        Iterator & operator=(       Iterator &&Rhs ) noexcept;

      public:
        Iterator & Move( const MDLSpace::Direction &Dir, size_t Count ) ExceptDebug;
        Iterator & Move( size_t Index ) ExceptDebug;

        Iterator & MoveBack( size_t Index ) ExceptDebug;

      private:
        struct MovedTooFar
        {
          explicit MovedTooFar( size_t Index, bool Back ) noexcept;
        };

        struct UnableToMove
        {
          explicit UnableToMove( size_t Index, size_t DimSize ) noexcept;
        };

      private:
        const std::vector< size_t > &DimensionSizes;

        Directions *Iter = nullptr;
    };
  
    class ConstIterator : public Iterator
    {
      public:
        explicit ConstIterator( const std::vector< size_t > &DimensionSizes, Directions *Begin = nullptr ) noexcept;

      public:
        const ListType & operator*() const noexcept;
        const ListType * operator->() const noexcept;

      public:
        ConstIterator & Move( const MDLSpace::Direction &Dir, size_t Count ) ExceptDebug;
        ConstIterator & Move( size_t Index ) ExceptDebug;
    };

  public:
    MultiDirectionalList() noexcept = default;

    MultiDirectionalList( const ListType &Data, const DirInitType &Directions, const std::vector< size_t > &DimensionCounts ) ExceptDebug;
    MultiDirectionalList(       ListType &&Data,      DirInitType &&Directions,      std::vector< size_t > &&DimensionCounts ) ExceptDebug;
  
    MultiDirectionalList( const DataInitType &Data, const DirInitType &Directions, 
                          const std::vector< size_t > &DimensionCounts ) ExceptDebug;
    MultiDirectionalList(       DataInitType &&Data,      DirInitType &&Directions,
                                std::vector< size_t > &&DimensionCounts ) ExceptDebug;

    ~MultiDirectionalList() noexcept = default;

  public:
    Iterator Move( const MDLSpace::Direction &Dir, size_t Count ) ExceptDebug;
    Iterator Move( size_t Index ) ExceptDebug;
  
    ConstIterator Move( const MDLSpace::Direction &Dir, size_t Count ) const ExceptDebug;
    ConstIterator Move( size_t Index ) const ExceptDebug;

  public:
    MultiDirectionalList & PushBack( const ListType &Data ) ExceptDebug;
    MultiDirectionalList & PushBack(       ListType &&Data ) ExceptDebug;
    MultiDirectionalList & PushBack( const DataInitType &Data ) ExceptDebug;
    MultiDirectionalList & PushBack(       DataInitType &&Data ) ExceptDebug;
    MultiDirectionalList & PushBack( const DataInitType &Data, const DirInitType &Directions,
                                     const std::vector< size_t > &DimensionCounts ) ExceptDebug;
    MultiDirectionalList & PushBack(       DataInitType &&Data,      DirInitType &&Directions, 
                                           std::vector< size_t > &&DimensionCounts ) ExceptDebug;

    MultiDirectionalList & PushFront( const ListType &Data ) ExceptDebug;
    MultiDirectionalList & PushFront(       ListType &&Data ) ExceptDebug;
    MultiDirectionalList & PushFront( const ListType &Data, const DirInitType &Directions,
                                      const std::vector< size_t > &DimensionCounts ) ExceptDebug;
    MultiDirectionalList & PushFront(       ListType &&Data,      DirInitType &&Directions,
                                            std::vector< size_t > &&DimensionCounts ) ExceptDebug;
    MultiDirectionalList & PushFront( const DataInitType &Data ) ExceptDebug;
    MultiDirectionalList & PushFront(       DataInitType &&Data ) ExceptDebug;
    MultiDirectionalList & PushFront( const DataInitType &Data, const DirInitType &Directions,
                                      const std::vector< size_t > &DimensionCounts ) ExceptDebug;
    MultiDirectionalList & PushFront(       DataInitType &&Data,      DirInitType &&Directions,
                                            std::vector< size_t > &&DimensionCounts ) ExceptDebug;

    MultiDirectionalList & InsertAfter( const ListType &Data,              size_t Index ) ExceptDebug;
    MultiDirectionalList & InsertAfter(       ListType &&Data,             size_t Index ) ExceptDebug;
    MultiDirectionalList & InsertAfter( const DataInitType &Data,  size_t Index ) ExceptDebug;
    MultiDirectionalList & InsertAfter(       DataInitType &&Data, size_t Index ) ExceptDebug;

    MultiDirectionalList & InsertBefore( const ListType &Data,              size_t Index ) ExceptDebug;
    MultiDirectionalList & InsertBefore(       ListType &&Data,             size_t Index ) ExceptDebug;
    MultiDirectionalList & InsertBefore( const DataInitType &Data,  size_t Index ) ExceptDebug;
    MultiDirectionalList & InsertBefore(       DataInitType &&Data, size_t Index ) ExceptDebug;

    MultiDirectionalList & PopFront() ExceptDebug;
    MultiDirectionalList & PopBack() ExceptDebug;
    MultiDirectionalList & Erase( size_t Index ) ExceptDebug;

  public:
    Iterator begin() noexcept;
    ConstIterator begin() const noexcept;

    const ConstIterator end() const noexcept;

  public:
    size_t GetSize() const noexcept;

    MultiDirectionalList & Resize( size_t Count, const ListType &Data ) ExceptDebug;
    MultiDirectionalList & Resize( size_t Count,       ListType &&Data = ListType() ) ExceptDebug; 
    MultiDirectionalList & Resize( size_t Count, const ListType &Data, const DirInitType &Directions ) ExceptDebug;
    MultiDirectionalList & Resize( size_t Count,       ListType &&Data,      DirInitType &&Directions ) ExceptDebug;

  public: 
    MultiDirectionalList & ValidateIterators() noexcept;

    MultiDirectionalList & SetDimensions( const DirInitType &Directions, const std::vector< size_t > &DimensionCounts ) ExceptDebug;
    MultiDirectionalList & SetDimensions(       DirInitType &&Directions,      std::vector< size_t > &&DimensionCounts ) ExceptDebug;

  private:
    struct NotEnoughDirectionInitializers
    {
      explicit NotEnoughDirectionInitializers( size_t Count ) noexcept;
    };

    struct MovedTooFar
    {
      explicit MovedTooFar( const MDLSpace::Direction &Dir, size_t Count ) noexcept;
      explicit MovedTooFar( size_t Index, size_t Size ) noexcept;
    };

    struct DirectionSizeError
    {
      explicit DirectionSizeError( size_t DirSize, size_t ListSize ) noexcept;
    };

  private:
    class Memory
    {
      public:
        Memory() noexcept = default;

        explicit Memory( size_t BlockSize, const ListType &Data, const DirInitType &Directions ) noexcept;
        explicit Memory( size_t BlockSize,       ListType &&Data,      DirInitType &&Directions ) noexcept;

        explicit Memory( const std::vector< size_t > &BlockSize, const ListType &Data, const DirInitType &Directions ) noexcept;
        explicit Memory( const std::vector< size_t > &BlockSize,       ListType &&Data,      DirInitType &&Directions ) noexcept;

        ~Memory() noexcept;
      
      public:
        Directions * operator=( std::pair< size_t, Directions >                &&DataCount ) noexcept;
        Directions * operator=( std::pair< std::vector< size_t >, Directions > &&DataCount ) noexcept;

      public:
        Directions * GetTop() noexcept;
        Directions * GetBlock( size_t Index ) noexcept;

        Directions * SetBlock( Directions &&Dir ) noexcept;

        void Erase( size_t Index ) noexcept;

        void Reset() noexcept;

      private: 
        size_t BlockSize = 0;
        size_t CurrentIndex = 0;

        std::vector< Directions * > MemoryBlocks{};
        std::set< size_t > EmptyBlocks{};
    };

  private:
    Directions * MoveTo( Directions *Start, size_t End ) const ExceptDebug;

    void InvalidateIterators() noexcept;

  private:
    std::vector< size_t > DimensionSizes;
    
    Memory DirectionsMemory{};

    Directions *DirectionalHead = nullptr;

    size_t Size = 0;
};

#include "MultiDirectionalList.cpp"
