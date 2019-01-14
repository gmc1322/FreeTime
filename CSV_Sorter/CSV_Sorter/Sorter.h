#pragma once

#include <vector>
#include <array>
#include <fstream>
#include <thread>
#include <atomic>
#include <execution>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct SortingParameter;

/**
 * \brief CSV Privates
 */
namespace CSV
{
  struct Row
  {
    friend std::ifstream& operator>>( std::ifstream &IFS, CSV::Row &Row ) noexcept;

    friend std::ofstream& operator<<( std::ofstream &OFS, const CSV::Row &Rhs ) noexcept;

    void Reserve( const size_t ElementCount ) noexcept;

    bool IsInvalid() const noexcept;

    std::vector< std::string > Elements;
  };

  void Sorter_Threaded( const CSV::Row* *RowIter, const CSV::Row* *RowEnd, const SortingParameter *ParameterIter, const SortingParameter *ParameterEnd,
                        int ThreadCount ) noexcept;
  void Sorter( const CSV::Row* *RowIter, const CSV::Row* *RowEnd, const SortingParameter *ParameterIter, const SortingParameter *ParameterEnd ) noexcept;

  void Output( const char *OutputName, const CSV::Row &Header, const CSV::Row* *SortableRows, size_t SortableSize, const std::vector< CSV::Row > &InvalidRows ) noexcept;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct InvalidFileName : std::runtime_error
{
  explicit InvalidFileName( const std::string &FileName ) noexcept;

  explicit InvalidFileName( const char *FileName ) noexcept;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Must contain a vector to allow the ParameterArray to contain different size Parameters
struct SortingParameter
{
  public:
    SortingParameter() noexcept = default;

    SortingParameter( size_t Index, const std::initializer_list< const char * > &Priorities ) noexcept;

    SortingParameter( const SortingParameter  &Copy ) noexcept = delete;
    SortingParameter(       SortingParameter &&Move ) noexcept;

    ~SortingParameter() noexcept = default;

  public:
    void operator=( const SortingParameter  &Copy ) noexcept = delete;
    void operator=(       SortingParameter &&Move ) noexcept;

  public:
    size_t Index = 0;
    std::vector< std::string > Priorities{};
};

template< size_t Size >
struct ParameterArray
{
  static constexpr size_t ParameterCount = Size;

  public:
    ParameterArray() noexcept = default;

    explicit ParameterArray( std::array< SortingParameter, Size > &&MovedParameters ) noexcept
    {
      for( size_t i = 0; i < Size; ++i )
      {
        Parameters[ i ] = std::move( MovedParameters[ i ] );
      }
    }

    explicit ParameterArray( SortingParameter &&Parameter ) noexcept { Parameters.front() = std::move( Parameter ); }

    ParameterArray( const ParameterArray &Copy ) noexcept = delete;

    ParameterArray( ParameterArray &&Move ) noexcept
    {
      for( size_t i = 0; i < Size; ++i )
      {
        Parameters[ i ] = std::move( Move.Parameters[ i ] );
      }
    }

    ~ParameterArray() noexcept = default;

  public:
    void operator=( const ParameterArray  &Copy ) noexcept = delete;
    void operator=(       ParameterArray &&Move ) noexcept
    {
      for( size_t i = 0; i < Size; ++i )
      {
        Parameters[ i ] = std::move( Move.Parameters[ i ] );
      }
    }

    const SortingParameter & operator[]( const size_t Index ) const noexcept { return Parameters[ Index ]; }

  public:
    template< typename Parameter, typename ... OtherParameters >
    void Fill( Parameter &&LeftParameter, OtherParameters&&... RightParameters ) noexcept
    {
      Parameters.front() = std::move( LeftParameter );

      Fill< 1 >( RightParameters... );
    }

    template< typename LeftParameter, typename RightParameter >
    void Fill( LeftParameter &&FrontParameter, RightParameter &&BackParameter ) noexcept
    {
      Parameters.front() = std::move( FrontParameter );

      Fill( std::move( BackParameter ) );
    }

    template< typename Parameter >
    void Fill( Parameter &&LastParameter ) noexcept { Parameters.back() = std::move( LastParameter ); }

  public:
    std::array< SortingParameter, Size  > Parameters;

  private:
    template< size_t Index, typename Parameter, typename ... OtherParameters >
    void Fill( Parameter &&LeftParameter, OtherParameters&&... RightParameters ) noexcept
    {
      Parameters[ Index ] = std::move( LeftParameter );

      Fill< Index + 1 >( RightParameters... );
    }

    template< size_t Index, typename Parameter, typename BackParameter >
    void Fill( Parameter &&LeftParameter, BackParameter &&LastParameter ) noexcept
    {
      Parameters[ Index ] = std::move( LeftParameter );

      Fill( std::move( LastParameter ) );
    }
};

template< typename ... ParameterTypes >
auto MakeParameterArray( ParameterTypes&&... Parameters ) noexcept
{
  ParameterArray< sizeof...( Parameters ) > Return{};

  Return.Fill( Parameters... );

  return Return;
}

ParameterArray< 1 > MakeParameterArray( SortingParameter &&Parameter ) noexcept;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::vector< CSV::Row > *GRows;
inline std::vector< CSV::Row > *GInvalidRows;

// Assumes the first row in the file is the header for each column
template< size_t ParameterCount >
void CSV_Sort( const char *FileName, const char *OutputName, const ParameterArray< ParameterCount > &Parameters ) noexcept( false )
{
  std::vector< CSV::Row > Rows;
  std::vector< CSV::Row > InvalidRows;

  {
    std::ifstream InputFile{ FileName };

    if( InputFile.is_open() )
    {
      CSV::Row Temp;
      size_t ElementCount;

      {
        // Get the character count
        InputFile.seekg( 0, std::ios_base::end );
        long long CharacterCount = InputFile.tellg();
        InputFile.seekg( 0, std::ios_base::beg );

        InputFile >> Temp;

        ElementCount = Temp.Elements.size(); // There could be more elements then sorting parameters, get count from header row

        const auto &Front = Parameters.Parameters.front();

        // Get over-estimate line count, allows for a max of 2 allocations
        const size_t LineCount = static_cast< size_t >( CharacterCount / // Get longest string for approximation
          ( ElementCount * std::max_element( std::execution::par_unseq, Front.Priorities.cbegin(), Front.Priorities.cend(), 
            []( const std::string &Lhs, const std::string &Rhs )noexcept
            {
              return Lhs.size() < Rhs.size();
            } )->size() ) );

        Rows.reserve( LineCount );
        InvalidRows.reserve( LineCount );

        Rows.emplace_back( std::move( Temp ) );
      }

      for( ; !( InputFile.eof() ); )
      {
        Temp.Reserve( ElementCount );

        InputFile >> Temp;

        if( Temp.IsInvalid() )
        {
          InvalidRows.emplace_back( std::move( Temp ) ); // Don't sort invalids, push them to the bottom of the spreadsheet
        }
        else Rows.emplace_back( std::move( Temp ) );
      }
    }
    else throw InvalidFileName{ FileName };
  }

  // Get back any unused space
  Rows.shrink_to_fit();
  InvalidRows.shrink_to_fit();

  GRows = &Rows;
  GInvalidRows = &InvalidRows;

  const size_t SortableSize = Rows.size() - 1; // Sub 1, we don't need the first row, it's just headers

  // Get a ptr to each row allowing for quicker swapping
  const auto * *const SortableRows = reinterpret_cast< const CSV::Row* * >( alloca( SortableSize * sizeof( void* ) ) ); // Allocate on the stack, faster accessing

  for( size_t i = 0; i < SortableSize; ++i )
  {
    SortableRows[ i ] = &( Rows[ i + 1 ] );
  }

  const auto ThreadCount = std::thread::hardware_concurrency();

  if( ThreadCount > 1 && SortableSize / ThreadCount > 1'000 ) // Make sure we have more then one thread and there are a lot of elements to sort per thread
  {
    Sorter_Threaded( SortableRows, SortableRows + SortableSize, Parameters.Parameters.data(),
                     Parameters.Parameters.data() + ParameterCount, static_cast< int >( ThreadCount ) );
  }
  else // Run non-threaded
    Sorter( SortableRows, SortableRows + SortableSize, Parameters.Parameters.data(), Parameters.Parameters.data() + ParameterCount );

  CSV::Output( OutputName, Rows.front(), SortableRows, SortableSize, InvalidRows );
}
