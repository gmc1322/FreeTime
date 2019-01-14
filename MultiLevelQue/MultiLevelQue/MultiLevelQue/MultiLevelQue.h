#pragma once

#include <deque>
#include <vector>

#include "Tracer.h"

template< typename KeyType, typename ... OtherKeyTypes >
class QueRange
{
  public:
    explicit QueRange( const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept;
    explicit QueRange( KeyType &&Key, const OtherKeyTypes &&... OtherKeys ) noexcept;

  public:
    bool operator!=( const QueRange &Rhs ) const noexcept;
    
    void operator++() noexcept;

  public:
    const KeyType & GetKey() const noexcept;
          KeyType && GetKey() noexcept;

    const QueRange< OtherKeyTypes... > & GetOthers() const noexcept;
          QueRange< OtherKeyTypes... > && GetOthers() noexcept;

  private:
    const KeyType Key;
    QueRange< OtherKeyTypes... > Others;
};

template< typename KeyType >
class QueRange< KeyType >
{
  public:
    explicit QueRange( const KeyType &Key ) noexcept;
    explicit QueRange(       KeyType &&Key ) noexcept;

  public:
    bool operator!=( const QueRange &Rhs ) const noexcept;

    void operator++() noexcept;

  public:
    const KeyType & GetKey() const noexcept;
          KeyType && GetKey() noexcept;

  private:
    KeyType Key;
};

template< typename KeyType, typename ... OtherKeyTypes >
constexpr QueRange< KeyType, OtherKeyTypes... > MakeQueRange( KeyType &&FirstKey, OtherKeyTypes &&... OtherKeys ) noexcept;

/**
 * \brief Creates a mutliple level que for sorting with multiple keys.
 *
 * \tparam KeyType   - The type for the key.
 * \tparam OtherKeyTypes - The recursive type for the keys and data.
 */
template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
class MultiLevelQue
{
  public:
    typedef MultiLevelQue< DataType, OtherKeyTypes... > InnerQueType;
    typedef typename InnerQueType::OuterKeyType InnerKeyType;

    typedef KeyType OuterKeyType;

  public:
    /**
     * \brief Make sure Key can be converted to a string!
     */
    struct CouldNotFind
    {
      explicit CouldNotFind( const KeyType &Key ) noexcept;
      explicit CouldNotFind( std::string &&String ) noexcept;

      const std::string What;
    };

  private:
    struct QueList
    {
      public:
        explicit QueList( const DataType &Data, const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept;
        explicit QueList(       DataType &&Data,      KeyType &&Key,      OtherKeyTypes &&... OtherKeys ) noexcept;
      
        explicit QueList( QueList *Parent, const DataType &Data, const QueRange< KeyType, OtherKeyTypes... > &Keys ) noexcept;
        explicit QueList( QueList *Parent,       DataType &&Data,      QueRange< KeyType, OtherKeyTypes... > &&Keys ) noexcept;

        QueList( const QueList &Copy ) noexcept;
        QueList( QueList *Parent, const QueList &Copy ) noexcept;

      public:
        KeyType Key;
        MultiLevelQue< DataType, OtherKeyTypes... > Data;

        QueList *Greater = nullptr;
        QueList *Less    = nullptr;
        QueList *Parent  = nullptr;
    };

    class Iter
    {
      public:
        explicit Iter( const MultiLevelQue< DataType, KeyType, OtherKeyTypes... > &Que ) noexcept;

      public:
        void operator++() const noexcept;

        bool operator!=( const QueList *Rhs ) const noexcept;

        const DataType & operator*() const noexcept;

      private:
        mutable MultiLevelQue< DataType, KeyType, OtherKeyTypes... > Que;
    };

  public:
    explicit MultiLevelQue( const DataType &Data, const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept;
    explicit MultiLevelQue(       DataType &&Data,      KeyType &&Key,      OtherKeyTypes &&... OtherKeys ) noexcept;
  
    explicit MultiLevelQue( const DataType &Data, const QueRange< KeyType, OtherKeyTypes... > &Keys ) noexcept;
    explicit MultiLevelQue(       DataType &&Data,       QueRange< KeyType, OtherKeyTypes... > &&Keys ) noexcept;

    MultiLevelQue( const MultiLevelQue &Copy ) noexcept;
    MultiLevelQue(       MultiLevelQue &&Move ) noexcept;

    ~MultiLevelQue() noexcept;

  public:
    bool Empty() const noexcept;

  public:
    DataType & Top() noexcept;

    const Iter begin() const noexcept; // Very very costly! Could make multiple copys of the Que!
    static constexpr QueList * end() noexcept;

    template< typename ... InnerKeyTypes, typename ... OuterKeyTypes  >
    MultiLevelQue< DataType, InnerKeyTypes... > GetQue( const KeyType &Key, const OuterKeyTypes &... OtherKeys ) const ExceptDebug;
  
    /**
     * \brief Creates a new MutliQue from an the inner Que with key: Key.
     *
     * \tparam InnerKeyTypes - The types for the new Que to contain.
     *
     * \param Key - The que containing this key will be copied.
     *
     * \return MultiLevelQue<InnerKeyTypes...> - The new que.
     */
    template< typename ... InnerKeyTypes  >
    MultiLevelQue< DataType, InnerKeyTypes... > GetQue( const KeyType &Key ) const ExceptDebug;

  public:
    DataType & Find( const KeyType &Key, const OtherKeyTypes &... OtherKeys ) ExceptDebug;
    DataType * Find_NoThrow( const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept; // Much less efficient than the throw version
  
    DataType & Find( const QueRange< KeyType, OtherKeyTypes... > &Range ) ExceptDebug;
    DataType * Find_NoThrow( const QueRange< KeyType, OtherKeyTypes... > &Range ) noexcept; // Much less efficient than the throw version

    /**
     * \brief Finds data from the last Begin key, to the last End key.
     *
     * \return std::vector< DataType > - A vector of Data.
     *
     * This is doubly inclusive range [b,e]
     */
    std::vector< DataType > FindMultiple( const KeyType &KeyBegin, const OtherKeyTypes &... OthersBegin,
                                          const QueRange< KeyType, OtherKeyTypes... > &End ) ExceptDebug;
    std::vector< DataType > FindMultiple_NoThrow( const KeyType &KeyBegin, const OtherKeyTypes &... OthersBegin,
                                                  const QueRange< KeyType, OtherKeyTypes... > &End ) noexcept; // Much less efficient than the throw version

    auto FindRange( const KeyType &Key, const OtherKeyTypes &... Others ) ExceptDebug;
    auto FindRange_NoThrow( const KeyType &Key, const OtherKeyTypes &... Others ) noexcept; // Much less efficient than the throw version
  
    auto FindRange( const QueRange< KeyType, OtherKeyTypes... > &Range ) ExceptDebug;
    auto FindRange_NoThrow( const QueRange< KeyType, OtherKeyTypes... > &Range ) noexcept; // Much less efficient than the throw version
  
    /**
     * \brief Finds a range of data from the last Begin key, to the last End key.
     *
     * \return std::vector< IterRanges > - A vector of IterRanges.
     * 
     * This is doubly inclusive range [b,e]
     */
    auto FindMultipleRanges( const KeyType &KeyBegin, const OtherKeyTypes &... OthersBegin,
                             const QueRange< KeyType, OtherKeyTypes... > &End ) ExceptDebug;
    auto FindMultipleRanges_NoThrow( const KeyType &KeyBegin, const OtherKeyTypes &... OthersBegin,
                                     const QueRange< KeyType, OtherKeyTypes... > &End ) noexcept; // Much less efficient than the throw version

  public:
    void Emplace( const DataType &Data, const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept;
    void Emplace(       DataType &&Data,      KeyType &&Key,      OtherKeyTypes &&... OtherKeys ) noexcept;
  
    void EmplaceAfter( const DataType &Data, const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept;
    void EmplaceAfter(       DataType &&Data,      KeyType &&Key,      OtherKeyTypes &&... OtherKeys ) noexcept;

  public:
    void Pop() ExceptDebug;

    void Erase( const KeyType &Key, const OtherKeyTypes &... OtherKeys ) ExceptDebug;
    void EraseRange( const KeyType &Key, const OtherKeyTypes &... OtherKeys ) ExceptDebug;

  private:
    struct QueIsEmpty
    {
      QueIsEmpty() noexcept;
    };

  private:
    void Emplacer( void ( MultiLevelQue< DataType, OtherKeyTypes... >::*EmplacePtr )( const DataType &, const OtherKeyTypes &... ), 
                   const DataType &Data, const KeyType &Key, const OtherKeyTypes &... OtherKeys ) noexcept;
    void Emplacer( void ( MultiLevelQue< DataType, OtherKeyTypes... >::*EmplacePtr )( DataType &&, OtherKeyTypes &&... ), DataType &&Data,
                   KeyType &&Key, OtherKeyTypes &&... OtherKeys ) noexcept;

    template< typename Return, bool IsThrowing >
    Return Finder( const KeyType &Key, Return( *FunctionPtr )( QueList * ) noexcept( IsThrowing ) ) noexcept( IsThrowing || ExceptDebug );

    template< typename Return >
    Return Finder_NoThrow( const KeyType &Key, Return( *FunctionPtr )( QueList * ) noexcept ) noexcept;

    template< bool IsThrowing1, bool IsThrowing2, bool IsThrowing3 >
    void Finder( const KeyType &Key, bool( *Lambda1 )( QueList * ) noexcept( IsThrowing1 ),
                 bool( *Lambda2 )( QueList * ) noexcept( IsThrowing2 ), void( *Lambda3 )( QueList * ) noexcept( IsThrowing3 ) ) 
                   noexcept( IsThrowing1 || IsThrowing2 || IsThrowing3 || ExceptDebug );
  
    void Finder_NoThrow( const KeyType &Key, bool( *Lambda1 )( QueList * ) noexcept, bool( *Lambda2 )( QueList * ) noexcept,
                         void( *Lambda3 )( QueList * ) noexcept ) noexcept;
  

  
  private:
    QueList *Que = nullptr;
    QueList *TopPtr = Que;
};

template< typename DataType, typename KeyType >
class MultiLevelQue< DataType, KeyType >
{
  public:
    typedef KeyType OuterKeyType;

  private:
    struct QueList
    {
      public:
        explicit QueList( const DataType &Data, const KeyType &Key ) noexcept;
        explicit QueList(       DataType &&Data,      KeyType &&Key ) noexcept;
      
        explicit QueList( QueList *Parent, const DataType &Data, const KeyType &Key ) noexcept;
        explicit QueList( QueList *Parent,       DataType &&Data,      KeyType &&Key ) noexcept;

        QueList( const QueList &Copy ) noexcept;
        QueList( QueList *Parent, const QueList &Copy ) noexcept;

      public:
        KeyType Key;
        std::deque< DataType > Data;

        QueList *Greater = nullptr;
        QueList *Less    = nullptr;
        QueList *Parent  = nullptr;
    };

    class Iter
    {
      public:
        Iter( MultiLevelQue< DataType, KeyType > *Que ) noexcept;

        Iter( const Iter &Copy ) noexcept;
        Iter( Iter &&Move ) noexcept;

      public:
        const DataType & operator*() const noexcept;
        void operator++() const noexcept;
        bool operator!=( const QueList *Rhs ) const noexcept;

      private:
        mutable MultiLevelQue< DataType, KeyType > Que;
    };

  public:
    struct CouldNotFind
    {
      explicit CouldNotFind( const KeyType &Key ) noexcept;

      const std::string What;
    };

    class IterRange
    {
      public:
        IterRange( QueRange< KeyType > &&Begin, QueRange< KeyType > &&End ) noexcept;

        IterRange( const IterRange &Copy ) noexcept;
        IterRange(       IterRange &&Move ) noexcept;

      public:
        QueRange< KeyType > & begin() noexcept;
        QueRange< KeyType > & end() noexcept;

      private:
        QueRange< KeyType > Begin, End;
    };

  public:
    MultiLevelQue( const DataType &Data, const KeyType &Key ) noexcept;
    MultiLevelQue(       DataType &&Data,      KeyType &&Key ) noexcept;

    MultiLevelQue( const DataType &Data, const QueRange< KeyType > &Keys ) noexcept;
    MultiLevelQue(       DataType &&Data,      QueRange< KeyType > &&Keys ) noexcept;

    MultiLevelQue( const MultiLevelQue &Copy ) noexcept;
    MultiLevelQue(       MultiLevelQue &&Move ) noexcept;

    ~MultiLevelQue() noexcept;

  public:
    bool Empty() const noexcept;

  public:
    DataType & Top() noexcept;

    const Iter begin() const noexcept;
    static constexpr QueList * end() noexcept;

  public:
    DataType & Find( const KeyType &Key ) ExceptDebug;
    DataType * Find_NoThrow( const KeyType &Key ) noexcept; // Much less efficient than the throw version
  
    DataType & Find( const QueRange< KeyType > &Range ) ExceptDebug;
    DataType * Find_NoThrow( const QueRange< KeyType > &Range ) noexcept; // Much less efficient than the throw version

    std::vector< DataType > & FindRange( const KeyType &Key ) ExceptDebug;
    std::vector< DataType > * FindRange_NoThrow( const KeyType &Key ) noexcept;
  
    std::vector< DataType > & FindRange( const QueRange< KeyType > &Range ) ExceptDebug;
    std::vector< DataType > * FindRange_NoThrow( const QueRange< KeyType > &Range ) noexcept;

  public:
    void Emplace( const DataType &Data, const KeyType &Key ) noexcept;
    void Emplace(       DataType &&Data,      KeyType &&Key ) noexcept;
  
    void EmplaceAfter( const DataType &Data, const KeyType &Key ) noexcept;
    void EmplaceAfter(       DataType &&Data,      KeyType &&Key ) noexcept;

  public:
    void Pop() ExceptDebug;

    void Erase( const KeyType &Key ) ExceptDebug;
    void EraseRange( const KeyType &Key ) ExceptDebug;

  private:
    struct QueIsEmpty
    {
      QueIsEmpty() noexcept;
    };

  private:
    template< typename Return, bool IsThrowing >
    Return Finder( const KeyType &Key, Return( *FunctionPtr )( QueList * ) noexcept( IsThrowing ) ) noexcept( IsThrowing || ExceptDebug );

    template< typename Return >
    Return * Finder_NoThrow( const KeyType &Key, Return* ( *FunctionPtr )( QueList * ) noexcept ) noexcept;

    void Finder_NoThrow( const KeyType &Key, bool( *Lambda1 )( QueList * ) noexcept, bool( *Lambda2 )( QueList * ) noexcept,
                         void( *Lambda3 )( QueList * ) noexcept ) noexcept;

    void Eraser( QueList *( &ErasePtr ) ) noexcept;

  private:
    QueList *Que = nullptr;
    QueList *TopPtr = Que;
};

template< typename DataType, typename KeyType, typename ... OtherKeyTypes >
constexpr MultiLevelQue< DataType, KeyType, OtherKeyTypes... > MakeMultiLevelQue( DataType &&Data, KeyType &&FirstKey,
                                                                                  OtherKeyTypes &&... OtherKeys ) noexcept;

#include "MultiLevelQue.cpp"
