/*!------------------------------------------------------------------------------
\file   SharedDirtyWrapper.h

\author Garrett Conti

\par    Project: SharedDirtyWrapper
\par    Course:  
   ------------------------------------------------------------------------------ */

#pragma once

#include <vector>

template< typename DataType >
class SharedDirtyWrapper
{
  public:
    SharedDirtyWrapper() noexcept = default;

    explicit SharedDirtyWrapper( const DataType  &Data ) noexcept;
    explicit SharedDirtyWrapper(       DataType &&Data ) noexcept;

    explicit SharedDirtyWrapper( DataType        *Data_ ) noexcept = delete;
    explicit SharedDirtyWrapper( DataType *const *Data_ ) noexcept = delete;

    explicit SharedDirtyWrapper( DataType* *Data_ ) noexcept;

    SharedDirtyWrapper( SharedDirtyWrapper &Copy ) noexcept;
    SharedDirtyWrapper( SharedDirtyWrapper &&Move ) noexcept;

    ~SharedDirtyWrapper() noexcept;

  public:
    SharedDirtyWrapper & operator=( const DataType  &Data_ ) noexcept;
    SharedDirtyWrapper & operator=(       DataType &&Data_ ) noexcept;

    SharedDirtyWrapper & operator=( DataType        *Data_ ) noexcept = delete;
    SharedDirtyWrapper & operator=( DataType *const *Data_ ) noexcept = delete;

    SharedDirtyWrapper & operator=( DataType* *Data_ ) noexcept;

    const SharedDirtyWrapper & operator=( const SharedDirtyWrapper &Copy ) const noexcept = delete;

    SharedDirtyWrapper & operator=( const SharedDirtyWrapper &Copy ) noexcept;
    SharedDirtyWrapper & operator=( SharedDirtyWrapper &&Move ) noexcept;

  public:
    bool IsDataDirty() const noexcept;

    const DataType & GetData() noexcept;
    const DataType & GetData() const noexcept;

    size_t GetReferenceCount() const noexcept;

  public:
    void SetData( const DataType  &Data_ ) noexcept;
    void SetData(       DataType &&Data_ ) noexcept;

    void SetData( DataType        *Data_ ) noexcept = delete;
    void SetData( DataType *const *Data_ ) noexcept = delete;

    void SetData( DataType* *Data_ ) noexcept;

  private:
    void FindAndReplace( SharedDirtyWrapper *CheckPtr, SharedDirtyWrapper *ReplacePtr ) noexcept;
    void FindCleanAndReplace( SharedDirtyWrapper *CheckPtr, SharedDirtyWrapper *ReplacePtr ) noexcept;

    void SetReferencesDirty() noexcept;
    void SetReferencesDirty( DataType *NewDataPtr ) noexcept;

    void CleanReferences() const noexcept;

    void AddReferences() const noexcept;

  private:
    DataType *Data = nullptr;

    bool IsDirty = false;

    mutable std::vector< SharedDirtyWrapper * > References{};
};

#include "SharedDirtyWrapper.cpp"
