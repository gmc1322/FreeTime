#pragma once

#include "BaseWindow.h"

class MutliLevelQue
{
  public:
    void AddToQue( const ImVec2 &XY, BehaviorTreeWindow_BaseDoubleConstPtr Data ) noexcept;

    void FillMap( std::map< size_t, BehaviorTreeWindow_BaseDoubleConstPtr > &Map ) noexcept;

  private:
    template< typename DataType >
    struct QueList
    {
      QueList( float Key, DataType Data ) noexcept : Key_( Key ), Data_( Data ) {}

      explicit QueList( const QueList &Copy ) noexcept : Key_( Copy.Key_ ), Data_( Copy.Data_ ), NextQue( Copy.NextQue ) {}

      float Key_;

      DataType Data_;

      struct QueList< DataType > *NextQue = nullptr;
    };

    typedef QueList< BehaviorTreeWindow_BaseDoubleConstPtr > * QueData;

    QueList< QueData > *Que = nullptr;
};
