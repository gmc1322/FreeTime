
#include <Windows.h>
#include <thread>

static size_t ThreadCount = 0;
static size_t MaxThreads = _Thrd_hardware_concurrency() - 1;

/*
 * Thread Helpers
 */

template< typename ArrayType >
using NonThreadedFunction = void( * )( size_t ArraySize, va_list Vars );

template< typename ArrayType >
using ThreadedFunction = DWORD ( WINAPI* )( LPVOID Data );

#pragma warning( push )
#pragma warning( disable : 4244 ) // Dont care about float to int conversion loss
template< typename ArrayType, template < typename >typename DataType >
void RunThreadedFunctions( size_t ArraySize, size_t WorkCount, NonThreadedFunction< ArrayType > RegFunc,
                           ThreadedFunction< ArrayType > ThreadFunc, const char *Args, ... ) noexcept
{
  va_list Vars;
  va_start( Vars, Args );

  if( ArraySize >= ( MaxThreads + 1 ) * WorkCount )
  {
    const float DoAmount = ArraySize / ( ( MaxThreads + 1.0f ) * WorkCount );
    auto Threads = new HANDLE[ MaxThreads ];
    auto ThreadDatas = new DataType< ArrayType >[ MaxThreads ];

    for( size_t i = 0; i < MaxThreads; ++i )
    {
      ThreadDatas[ i ] = DataType< ArrayType >( DoAmount, i, Vars );

      Threads[ i ] = CreateThread( nullptr, 0, ThreadFunc, &ThreadDatas[ i ], 0, nullptr );
    }

    DataType< ArrayType > ThreadData( static_cast< size_t >( DoAmount ) * MaxThreads, ArraySize, true, Vars );

    ThreadFunc( &ThreadData );

    WaitForMultipleObjects( MaxThreads, Threads, true, INFINITE );

    for( size_t i = 0; i < MaxThreads; ++i )
    {
      CloseHandle( Threads[ i ] );
    }

    delete[] ThreadDatas;
    delete[] Threads;
  }
  else
  {
    RegFunc( ArraySize, Vars ); 
  }

  va_end( Vars );
}
#pragma warning( pop )

/*
 * Sorting Threads
 */

template< typename ArrayType >
struct SortData
{
  SortData( ArrayType *Array, size_t Low, size_t High ) noexcept : Array( Array ), Low( Low ), High( High ) {}

  ArrayType *Array;
  size_t Low;
  size_t High;
};

template< typename ArrayType >
DWORD WINAPI SortThreadFunction( LPVOID Data ) noexcept
{
  SortData< ArrayType > *DataStruct = static_cast< SortData< ArrayType > * >( Data );

  QuickSortThread( DataStruct->Array, DataStruct->Low, DataStruct->High );

  return 0;
}

template< typename ArrayType >
DWORD WINAPI SortSwapThreadFunction( LPVOID Data ) noexcept
{
  SortData< ArrayType > *DataStruct = static_cast< SortData< ArrayType > * >( Data );

  QuickSortThreadSwap( DataStruct->Array, DataStruct->Low, DataStruct->High );

  return 0;
}

template< typename ArrayType >
struct CopyData
{
  CopyData() noexcept {}

  CopyData( size_t DoAmount, size_t LoopIndex, va_list Vars ) noexcept :
    Array( va_arg( Vars, const ArrayType* ) ), ArrayCopy( va_arg( Vars, ArrayType* ) ), Start( DoAmount * LoopIndex ),
    End( Start + DoAmount ) {}
  
  CopyData( size_t Start, size_t ArraySize, bool, va_list Vars ) noexcept :
    Array( va_arg( Vars, const ArrayType* ) ), ArrayCopy( va_arg( Vars, ArrayType* ) ), Start( Start ),
    End( ArraySize ) {}

  CopyData( const CopyData &Copy ) noexcept :
    Array( Copy.Array ), ArrayCopy( Copy.ArrayCopy ), Start( Copy.Start ), End( Copy.End ) {}

  void operator=( const CopyData &Copy ) noexcept 
  { 
    Array = Copy.Array;
    ArrayCopy = Copy.ArrayCopy;
    Start = Copy.Start;
    End = Copy.End;
  }

  const ArrayType *Array = nullptr;
  ArrayType *ArrayCopy = nullptr;
  size_t Start = 0;
  size_t End = 0;
};

template< typename ArrayType >
DWORD WINAPI CopyThreadFunction( LPVOID Data ) noexcept
{
  CopyData< ArrayType > *CastedData = static_cast< CopyData< ArrayType > * >( Data );

  for( size_t i = CastedData->Start; i < CastedData->End; ++i )
  {
    CastedData->ArrayCopy[ i ] = CastedData->Array[ i ];
  }

  return 0;
}

template< typename ArrayType >
struct SwapData
{
  SwapData() noexcept {}

  SwapData( float DoAmount, size_t LoopIndex, va_list Vars ) noexcept :
    Array( va_arg( Vars, ArrayType* ) ), StartLeft( static_cast< size_t >( DoAmount * LoopIndex ) ),
    StartRight( static_cast< size_t >( DoAmount * ( ( MaxThreads + 1 ) * 2 ) - 1 - ( DoAmount * LoopIndex ) ) ),
    Count( static_cast< size_t >( DoAmount ) ) {}
  
  SwapData( size_t Start, size_t ArraySize, bool, va_list Vars ) noexcept :
    Array( va_arg( Vars, ArrayType* ) ), StartLeft( Start ), StartRight( ArraySize - Start ),
    Count( ( StartRight - StartLeft ) / 2 ) {}

  SwapData( const SwapData &Copy ) noexcept :
    Array( Copy.Array ), StartLeft( Copy.StartLeft ), StartRight( Copy.StartRight ), Count( Copy.Count ) {}

  void operator=( const SwapData &Copy ) noexcept
  {
    Array = Copy.Array;
    StartLeft = Copy.StartLeft;
    StartRight = Copy.StartRight;
    Count = Copy.Count;
  }

  ArrayType *Array = nullptr;
  size_t StartLeft = 0;
  size_t StartRight = 0;
  size_t Count = 0;
};

template< typename ArrayType >
DWORD WINAPI SwapThreadFunction( LPVOID Data ) noexcept
{
  SwapData< ArrayType > *CastedData = static_cast< SwapData< ArrayType > * >( Data );

  for( size_t Left = CastedData->StartLeft, Right = CastedData->StartRight;
       Left < CastedData->StartLeft + CastedData->Count; ++Left, --Right )
  {
    std::swap( CastedData->Array[ Left ], CastedData->Array[ Right ] );
  }

  return 0;
}

/*
 * Sorting Helpers
 */

template< typename ArrayType >
size_t Partition( ArrayType *Array, size_t Low, size_t High ) noexcept
{
  const ArrayType &Pivot = Array[ Low ];

  size_t i = Low - 1;
  size_t j = High + 1;

  for(;;)
  {
    do
    {
      ++i;
    }
    while( Array[ i ] < Pivot );

    do
    {
      --j;
    }
    while( Pivot < Array[ j ] );

    if( i >= j )
    {
      return j;
    }

    std::swap< ArrayType >( Array[ i ], Array[ j ] );
  }
}

template< typename ArrayType >
size_t PartitionSwap( ArrayType *Array, size_t Low, size_t High ) noexcept
{
  const ArrayType &Pivot = Array[ Low ];

  size_t i = Low - 1;
  size_t j = High + 1;

  for( ;; )
  {
    do
    {
      ++i;
    }
    while( Array[ i ] < Pivot );

    do
    {
      --j;
    }
    while( Pivot < Array[ j ] );

    if( i >= j )
    {
      return j;
    }

    ArrayType::swap( Array[ i ], Array[ j ] );
  }
}

template< typename ArrayType >
void QuickSortThread( ArrayType *Array, size_t Low, size_t High ) noexcept
{
  if( Low < High )
  {
    size_t Pivot = Partition( Array, Low, High );

    if( ThreadCount >= MaxThreads )
    {
      QuickSort( Array, Low, Pivot );

      QuickSort( Array, Pivot + 1, High );
    }
    else
    {
      SortData< ArrayType > ThreadData( Array, Low, Pivot );

      HANDLE Thread = CreateThread( nullptr, 0, &SortThreadFunction< ArrayType >, &ThreadData, 0, nullptr );

      ++ThreadCount;

      QuickSortThread( Array, Pivot + 1, High );

      WaitForSingleObject( Thread, INFINITE );

      CloseHandle( Thread );

      --ThreadCount;
    }
  }
}

template< typename ArrayType >
void QuickSortThreadSwap( ArrayType *Array, size_t Low, size_t High ) noexcept
{
  if( Low < High )
  {
    size_t Pivot = PartitionSwap( Array, Low, High );

    if( ThreadCount >= MaxThreads )
    {
      QuickSortSwap( Array, Low, Pivot );

      QuickSortSwap( Array, Pivot + 1, High );
    }
    else
    {
      SortData< ArrayType > ThreadData( Array, Low, Pivot );

      HANDLE Thread = CreateThread( nullptr, 0, &SortSwapThreadFunction< ArrayType >, &ThreadData, 0, nullptr );

      ++ThreadCount;

      QuickSortThreadSwap( Array, Pivot + 1, High );

      WaitForSingleObject( Thread, INFINITE );

      CloseHandle( Thread );

      --ThreadCount;
    }
  }
}

template< typename ArrayType >
void QuickSort( ArrayType *Array, size_t Low, size_t High ) noexcept
{
  if( Low < High )
  {
    if( High - Low <= 1 )
    {
      if( Array[ High ] < Array[ Low ] )
      {
        std::swap< ArrayType >( Array[ Low ], Array[ High ] );
      }
    }
    else
    {
      size_t Pivot = Partition( Array, Low, High );

      QuickSort( Array, Low, Pivot );

      QuickSort( Array, Pivot + 1, High );
    }
  }
}

template< typename ArrayType >
void QuickSortSwap( ArrayType *Array, size_t Low, size_t High ) noexcept
{
  if( Low < High )
  {
    if( High - Low <= 1 )
    {
      if( Array[ High ] < Array[ Low ] )
      {
        ArrayType::swap( Array[ Low ], Array[ High ] );
      }
    }
    else
    {
      size_t Pivot = Partition( Array, Low, High );

      QuickSortSwap( Array, Low, Pivot );

      QuickSortSwap( Array, Pivot + 1, High );
    }
  }
}

/*
 * Sorting Functions
 */

#include <future>

template< typename ArrayType >
void Sort( ArrayType *const Array, const size_t ArraySize ) noexcept
{
  QuickSortThread( Array, 0, ArraySize - 1 );
  
  /*for( size_t i = 0; i < MaxThreads; ++i )
  {
    std::async( std::launch::async, std::sort, )
  }*/
}

template< typename ArrayType >
void Sort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept
{
  CopyArray( InArray, OutArray, ArraySize );

  Sort( OutArray, ArraySize );
}


template< typename ArrayType >
void RSort( ArrayType *Array, size_t ArraySize ) noexcept
{
  Sort( Array, ArraySize );

  ReverseArray( Array, ArraySize );
}

template< typename ArrayType >
void RSort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept
{
  Sort( InArray, OutArray, ArraySize );

  ReverseArray( OutArray, ArraySize );
}


template< typename ArrayType >
void SwapSort( ArrayType *Array, size_t ArraySize ) noexcept
{
  QuickSortThreadSwap( Array, 0, ArraySize - 1 );
}

template< typename ArrayType >
void SwapSort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept
{
  CopyArray( InArray, OutArray, ArraySize );

  SwapSort( OutArray, ArraySize );
}


template< typename ArrayType >
void RSwapSort( ArrayType *Array, size_t ArraySize ) noexcept
{
  SwapSort( Array, ArraySize );

  ReverseArray( Array, ArraySize );
}

template< typename ArrayType >
void RSwapSort( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept
{
  SwapSort( InArray, OutArray, ArraySize );

  ReverseArray( OutArray, ArraySize );
}

/*
 * Other Good Functions Being Used
 */

template< typename ArrayType >
void ReverseArray( ArrayType *Array, size_t ArraySize ) noexcept
{
  auto RegFunc = []( size_t Size, va_list Vars ) noexcept 
  { 
    ArrayType *Swap = va_arg( Vars, ArrayType* );

    for( size_t i = 0; i < Size / 2; ++i )
    {
      std::swap( Swap[ i ], Swap[ ( Size - 1 ) - i ] );
    }
  };

  RunThreadedFunctions< ArrayType, SwapData >( ArraySize, 2, RegFunc, SwapThreadFunction< ArrayType >, "A", Array, Array );
}

template< typename ArrayType >
void CopyArray( const ArrayType *InArray, ArrayType *OutArray, size_t ArraySize ) noexcept
{
  if( std::is_fundamental< ArrayType >::value )
  {
    memcpy( OutArray, InArray, ArraySize * sizeof( ArrayType ) );
  }
  else
  {
    auto RegFunc = []( size_t Size, va_list Vars ) noexcept
    {
      const ArrayType *CopyFrom = va_arg( Vars, const ArrayType* );
      ArrayType *CopyInto = va_arg( Vars, ArrayType* );

      for( size_t i = 0; i < Size; ++i )
      {
        CopyInto[ i ] = CopyFrom[ i ];
      }
    };

    RunThreadedFunctions< ArrayType, CopyData >( ArraySize, 1, RegFunc, CopyThreadFunction< ArrayType >, "AA",
                                                 InArray, OutArray, InArray, OutArray );
  }
}
