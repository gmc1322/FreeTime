/*!------------------------------------------------------------------------------
\file   UniqueDirtyWrapper.h

\author Garrett Conti

\par    Project: DirtyWrapper
\par    Course:  
   ------------------------------------------------------------------------------ */

#pragma once

#include <vector>

template< typename DataType >
class UniqueParentDirtyWrapper;

template< typename DataType >
class UniqueChildDirtyWrapper
{
  public:
    UniqueChildDirtyWrapper() noexcept = default;

    UniqueChildDirtyWrapper( const UniqueChildDirtyWrapper &Copy  ) noexcept = delete;
    UniqueChildDirtyWrapper(       UniqueChildDirtyWrapper &&Move ) noexcept;

    explicit UniqueChildDirtyWrapper( const UniqueParentDirtyWrapper< DataType > &Parent ) noexcept;

    ~UniqueChildDirtyWrapper() noexcept;

  public:
    UniqueChildDirtyWrapper & operator=( const UniqueChildDirtyWrapper &Copy ) noexcept = delete;
    UniqueChildDirtyWrapper & operator=(       UniqueChildDirtyWrapper &&Move ) noexcept;

    UniqueChildDirtyWrapper & operator=( const UniqueParentDirtyWrapper< DataType > &Parent ) noexcept;

  public:
    const DataType & GetData() const noexcept;

    bool GetIsDirty() const noexcept;
    bool GetIsValidData() const noexcept;

  private:
    const DataType *const *Data = nullptr;

    bool *IsDirty = nullptr;
};

template< typename DataType >
class UniqueParentDirtyWrapper
{
  public:
    UniqueParentDirtyWrapper() noexcept = default;

    explicit UniqueParentDirtyWrapper( const DataType  &Data ) noexcept;
    explicit UniqueParentDirtyWrapper( DataType &&Data ) noexcept;

    explicit UniqueParentDirtyWrapper( DataType        *Data_ ) noexcept = delete;
    explicit UniqueParentDirtyWrapper( DataType *const *Data_ ) noexcept = delete;

    explicit UniqueParentDirtyWrapper( DataType* *Data_ ) noexcept;

    UniqueParentDirtyWrapper( const UniqueParentDirtyWrapper &Copy ) noexcept = delete;
    UniqueParentDirtyWrapper( UniqueParentDirtyWrapper &&Move ) noexcept;

    ~UniqueParentDirtyWrapper() noexcept;

  public:
    UniqueParentDirtyWrapper & operator=( const DataType  &Data_ ) noexcept;
    UniqueParentDirtyWrapper & operator=( DataType &&Data_ ) noexcept;

    UniqueParentDirtyWrapper & operator=( DataType        *Data_ ) noexcept = delete;
    UniqueParentDirtyWrapper & operator=( DataType *const *Data_ ) noexcept = delete;

    UniqueParentDirtyWrapper & operator=( DataType* *Data_ ) noexcept;

    UniqueParentDirtyWrapper & operator=( const UniqueParentDirtyWrapper &Copy ) noexcept = delete;
    UniqueParentDirtyWrapper & operator=( UniqueParentDirtyWrapper &&Move ) noexcept;

  public:
    const DataType & GetData() noexcept;
    const DataType & GetData() const noexcept;

    bool IsDataDirty() const noexcept;

  public:
    void SetData( const DataType  &Data_ ) noexcept;
    void SetData( DataType &&Data_ ) noexcept;

    void SetData( DataType        *Data_ ) noexcept = delete;
    void SetData( DataType *const *Data_ ) noexcept = delete;

    void SetData( DataType* *Data_ ) noexcept;

  private:
    void SetDirtyBools() noexcept;

  private:
    DataType * Data = nullptr;

    bool IsDirty = false;

    mutable std::vector< bool * * > DirtyBools{};

  private:
    template< typename ParentDataType >
    friend class UniqueChildDirtyWrapper;
};

#include "UniqueDirtyWrapper.cpp"
