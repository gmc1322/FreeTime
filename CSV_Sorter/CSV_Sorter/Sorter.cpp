#include "Sorter.h"

#define _ENABLE_ATOMIC_ALIGNMENT_FIX 1

#include <utility>
#include <string>
#include <sstream>
#include <future>
#include <stack>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::ifstream& CSV::operator>>( std::ifstream &IFS, CSV::Row &Row ) noexcept
{
  std::string Element;

  std::getline( IFS, Element ); // Get an entire line first

  std::stringstream ElementTokenizer{ Element };

  while( std::getline( ElementTokenizer, Element, ',' ) ) // Now get each element from the line
  {
    Row.Elements.emplace_back( std::move( Element ) );
  }

  return IFS;
}

std::ofstream& CSV::operator<<( std::ofstream &OFS, const CSV::Row &Rhs ) noexcept
{
  size_t Size = Rhs.Elements.size();

  if( Size )
  {
    {
      const size_t End = Size - 1;

      for( Size = 0; Size < End; ++Size )
      {
        OFS << Rhs.Elements[ Size ] << ",";
      }
    }

    OFS << Rhs.Elements.back() << std::endl;
  }

  return OFS;
}

void CSV::Row::Reserve( const size_t ElementCount ) noexcept
{
  Elements.reserve( ElementCount );
}

bool CSV::Row::IsInvalid() const noexcept    // Can be parallel, we just need to find one empty string for it to be invalid!
{ return Elements.empty() || std::find( std::execution::par_unseq, Elements.cbegin(), Elements.cend(), std::string{} ) != Elements.cend(); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

InvalidFileName::InvalidFileName( const std::string &FileName ) noexcept : runtime_error{ "Invalid file name: '" + FileName + "'!" } {}

InvalidFileName::InvalidFileName( const char *const FileName ) noexcept : InvalidFileName{ std::string{ FileName } } {}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SortingParameter::SortingParameter( const size_t Index, const std::initializer_list< const char* > &Priorities_ ) noexcept : Index( Index )
{
  Priorities.reserve( Priorities_.size() );

  for( const char *const Priority : Priorities_ )
  {
    Priorities.emplace_back( std::string{ Priority } );
  }
}

SortingParameter::SortingParameter( SortingParameter &&Move ) noexcept : Index( Move.Index ), Priorities{ std::move( Move.Priorities ) } {}

void SortingParameter::operator=( SortingParameter &&Move ) noexcept
{
  Index = Move.Index;
  Priorities = std::move( Move.Priorities );
}

ParameterArray< 1 > MakeParameterArray( SortingParameter &&Parameter ) noexcept { return ParameterArray< 1 >{ std::move( Parameter ) }; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct CSV_RowIterator
{
  public:
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = ptrdiff_t;
    using value_type = std::string;
    using pointer = std::string *;
    using reference = const std::string &;

  public:
    CSV_RowIterator() noexcept = default;

    explicit CSV_RowIterator( const CSV::Row* *const Iter ) noexcept : Iter( Iter ) {}
             CSV_RowIterator( const CSV::Row* *const Iter, const size_t Index ) noexcept : Iter( Iter ), Index( Index ) {}

    CSV_RowIterator( const CSV_RowIterator  &Copy ) noexcept = default;
    CSV_RowIterator(       CSV_RowIterator &&Move ) noexcept = default;

    ~CSV_RowIterator() noexcept = default;

  public:
    void operator++() noexcept { ++Iter; }

    CSV_RowIterator operator+( const size_t Amount ) const noexcept { return CSV_RowIterator{ Iter + Amount, Index }; }
    size_t operator-( const CSV_RowIterator &Rhs ) const noexcept { return Iter - Rhs.Iter; }

    const std::string & operator*() const noexcept { return ( *Iter )->Elements[ Index ]; }

    bool operator!=( const CSV_RowIterator &Rhs ) const noexcept { return Iter != Rhs.Iter; }
    bool operator==( const CSV_RowIterator &Rhs ) const noexcept { return Iter == Rhs.Iter; }
    bool operator<( const CSV_RowIterator &Rhs ) const noexcept { return Iter < Rhs.Iter; }
    bool operator>( const CSV_RowIterator &Rhs ) const noexcept { return Iter > Rhs.Iter; }

    void operator=( const CSV_RowIterator  &Copy ) noexcept { std::memcpy( this, &Copy, sizeof( CSV_RowIterator ) ); }
    void operator=(       CSV_RowIterator &&Move ) noexcept { std::memmove( this, &Move, sizeof( CSV_RowIterator ) ); }

  public:
    const CSV::Row* *Iter = nullptr;
    const size_t Index = 0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void Sort( const CSV::Row* *const RowIter, const CSV::Row* *const RowEnd, const SortingParameter *const ParameterIter ) noexcept
{
  const auto LessThan = [ ParameterIter ]( const CSV::Row *const Lhs, const CSV::Row *const Rhs )noexcept->bool
  {
    const auto &Priorities = ParameterIter->Priorities;

    const size_t Size = Priorities.size();

    size_t LhsIndex = 0;

    // Get the index into Priorities that each Element is relative to
    {
      const std::string &Element = Lhs->Elements[ ParameterIter->Index ];

      while( Element != Priorities[ LhsIndex ] )
      {
        if( ++LhsIndex >= Size ) return false; // Push empty cell to the bottom
      }
    }

    size_t RhsIndex = 0;

    {
      const std::string &Element = Rhs->Elements[ ParameterIter->Index ];

      while( Element != Priorities[ RhsIndex ] )
      {
        if( ++RhsIndex >= Size ) return true; // Keep empty cell at the bottom
      }
    }
    
    // Sort by each Element's Priority index
    return LhsIndex < RhsIndex;
  };

  std::sort( std::execution::par_unseq, RowIter, RowEnd, LessThan );
}

struct StackObject
{
  const CSV::Row* *BeginRow;
  const CSV::Row* *EndRow;

  const SortingParameter *ParameterIter;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSV::Sorter_Threaded( const CSV::Row* *RowIter, const CSV::Row* *const RowEnd, const SortingParameter *ParameterIter, const SortingParameter *const ParameterEnd,
                           const int ThreadCount ) noexcept
{
  Sort( RowIter, RowEnd, ParameterIter ); // First sorting, does a full sort of the CSV

  if( ParameterIter + 1 != ParameterEnd )
  {
    std::vector< std::future< void > > Threads;

    const size_t Size = ParameterIter->Priorities.size();

    int NewThreadCount = ThreadCount - Size;

    if( NewThreadCount > 1 ) // We need at least two threads to make a difference 
    {
      // Will make a few more threads then logical processors, but just a few is ok
      NewThreadCount = static_cast< int >( std::ceil( static_cast< double >( NewThreadCount ) / Size ) ); // Give each thread the same amount of child threads, round up

      const SortingParameter *const NextParameter = ParameterIter + 1;

      CSV_RowIterator Begin{ RowIter, ParameterIter->Index };

      {
        const CSV_RowIterator End{ RowEnd, ParameterIter->Index };

        for( auto PriorityIter = ParameterIter->Priorities.cbegin() + 1, PriorityEnd = ParameterIter->Priorities.cend();
             PriorityIter != PriorityEnd; ++PriorityIter )
        {
          // Can't be parallel, we need to find the very first one and not just any index
          const CSV::Row* *TempIter = std::find( std::execution::seq, Begin, End, *PriorityIter ).Iter;

          Threads.emplace_back( std::async( std::launch::async, CSV::Sorter_Threaded, Begin.Iter, TempIter, NextParameter, ParameterEnd, 
                                            NewThreadCount ) );
          Begin.Iter = TempIter;
        }
      }

      // Run our function
      CSV::Sorter_Threaded( Begin.Iter, RowEnd, NextParameter, ParameterEnd, NewThreadCount );
    }
    else // Don't run any more threaded
    {
      const SortingParameter *const NextParameter = ParameterIter + 1;

      CSV_RowIterator Begin{ RowIter, ParameterIter->Index };

      {
        const CSV_RowIterator End{ RowEnd, ParameterIter->Index };

        for( auto PriorityIter = ParameterIter->Priorities.cbegin() + 1, PriorityEnd = ParameterIter->Priorities.cend();
             PriorityIter != PriorityEnd; ++PriorityIter )
        {
          // Can't be parallel, we need to find the very first one and not just any index
          const CSV::Row* *TempIter = std::find( std::execution::seq, Begin, End, *PriorityIter ).Iter;

          Threads.emplace_back( std::async( std::launch::async, CSV::Sorter, Begin.Iter, TempIter, NextParameter, ParameterEnd ) );

          Begin.Iter = TempIter;
        }
      }

      // Run our function
      CSV::Sorter( Begin.Iter, RowEnd, NextParameter, ParameterEnd );
    }

    // Wait for the other functions
    for( std::future< void > &Thread : Threads )
    {
      Thread.wait();
    }
  }
}

#undef _ENABLE_ATOMIC_ALIGNMENT_FIX

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSV::Sorter( const CSV::Row* *RowIter, const CSV::Row* *const RowEnd, const SortingParameter *ParameterIter,
                  const SortingParameter *const ParameterEnd ) noexcept
{
  Sort( RowIter, RowEnd, ParameterIter ); // First sorting, does a full sort of the CSV

  if( ParameterIter + 1 != ParameterEnd )
  {
    std::vector< StackObject > RecursionStack;

    {
      size_t Size = ( ParameterIter + 1 )->Priorities.size();

      {
        const SortingParameter *const End = ParameterEnd - 1; // We will only ever have one row of the last priorities, they are taken off right after they are added

        for( const SortingParameter *Iter = ParameterIter + 2; Iter != End; ++Iter )
        {
          Size *= Iter->Priorities.size();
        }

        Size += End->Priorities.size() - 1; // Sub one, its popped off after this function
      }

      RecursionStack.reserve( Size );
    }

    const auto Runner = [ &RecursionStack ]( const SortingParameter *CurrentParameter, const CSV::Row* *const CurrentRowIter, const CSV::Row* *const CurrentRowEnd )noexcept
    {
      const SortingParameter *const NextParameter = CurrentParameter + 1;

      CSV_RowIterator Begin{ CurrentRowIter, CurrentParameter->Index };

      std::vector< StackObject > ReverseObjects;

      {
        const CSV_RowIterator End{ CurrentRowEnd, CurrentParameter->Index };

        for( auto PriorityIter = CurrentParameter->Priorities.cbegin() + 1, PriorityEnd = CurrentParameter->Priorities.cend();
          PriorityIter != PriorityEnd; ++PriorityIter )
        {
          // Can't be parallel, we need to find the very first one and not just any index
          const CSV::Row* *TempIter = std::find( std::execution::seq, Begin, End, *PriorityIter ).Iter;

          ReverseObjects.emplace_back( StackObject{ Begin.Iter, TempIter, NextParameter } );

          Begin.Iter = TempIter;
        }
      }

      ReverseObjects.emplace_back( StackObject{ Begin.Iter, CurrentRowEnd, NextParameter } );

      std::reverse( std::execution::par_unseq, ReverseObjects.begin(), ReverseObjects.end() );

      for( const StackObject &Element : ReverseObjects )
      {
        RecursionStack.emplace_back( Element );
      }
    };

    Runner( ParameterIter, RowIter, RowEnd );
    
    while( RecursionStack.size() )
    {
      const StackObject CurrentObject = RecursionStack.back();
      RecursionStack.pop_back();

      Sort( CurrentObject.BeginRow, CurrentObject.EndRow, CurrentObject.ParameterIter ); // First sorting, does a full sort of the CSV

      if( CurrentObject.ParameterIter + 1 != ParameterEnd )
      {
        Runner( CurrentObject.ParameterIter, CurrentObject.BeginRow, CurrentObject.EndRow );
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CSV::Output( const char *const OutputName, const CSV::Row &Header, const CSV::Row* *const SortableRows, const size_t SortableSize,
                  const std::vector< CSV::Row > &InvalidRows ) noexcept
{
  std::ofstream OutputFile{ OutputName };

  OutputFile << Header;

  for( size_t i = 0; i < SortableSize; ++i )
  {
    OutputFile << *( SortableRows[ i ] );
  }

  for( const CSV::Row &Element : InvalidRows )
  {
    OutputFile << Element;
  }
}
