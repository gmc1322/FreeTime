
#include "MyMultiLevelQue.h"

void MutliLevelQue::AddToQue( const ImVec2 &XY, BehaviorTreeWindow_BaseDoubleConstPtr Data ) noexcept
{
  auto NewInnerItem = new QueList< BehaviorTreeWindow_BaseDoubleConstPtr >( XY.x, Data );

  if( Que )
  {
    auto iterOuter = Que;
    QueList< QueData > *prevOuter = nullptr;

    while( true )
    {
      const auto WindowSizeY = 80;

      if( XY.y + WindowSizeY <= iterOuter->Key_ + WindowSizeY * 1.9f && XY.y >= iterOuter->Key_ - WindowSizeY * 0.9f )
      {
        auto iterInner = iterOuter->Data_;
        QueList< BehaviorTreeWindow_BaseDoubleConstPtr > *prevInner = nullptr;

        while( true )
        {
          if( XY.x < iterInner->Key_ )
          {
            if( iterInner == iterOuter->Data_ )
            {
              NewInnerItem->NextQue = iterInner;

              iterOuter->Data_ = NewInnerItem;
            }
            else
            {
              auto Temp = prevInner->NextQue;

              prevInner->NextQue = NewInnerItem;

              NewInnerItem->NextQue = Temp;
            }

            break;
          }

          if( iterInner->NextQue == nullptr )
          {
            iterInner->NextQue = NewInnerItem;

            break;
          }

          prevInner = iterInner;
          iterInner = iterInner->NextQue;
        }

        break;
      }
      
      if( XY.y < iterOuter->Key_ - WindowSizeY * 0.9f )
      {
        auto NewOuterItem = new QueList< QueData >( XY.y, NewInnerItem );

        if( iterOuter == Que )
        {
          NewOuterItem->NextQue = iterOuter;

          Que = NewOuterItem;
        }
        else
        {
          auto Temp = prevOuter->NextQue;

          prevOuter->NextQue = NewOuterItem;

          NewOuterItem->NextQue = Temp;
        }

        break;
      }

      if( iterOuter->NextQue == nullptr )
      {
        iterOuter->NextQue = new QueList< QueData >( XY.y, NewInnerItem );

        break;
      }

      prevOuter = iterOuter;
      iterOuter = iterOuter->NextQue;
    }
  }
  else
  {
    Que = new QueList< QueData >( XY.y, NewInnerItem );
  }
}

void MutliLevelQue::FillMap( std::map< size_t, BehaviorTreeWindow_BaseDoubleConstPtr > &Map ) noexcept
{
  size_t Count = 0;

  while( Que != nullptr )
  {
    auto iterInner = Que->Data_;

    while( iterInner != nullptr )
    {
      auto Temp = iterInner->NextQue;

      Map.emplace( std::make_pair( Count, iterInner->Data_ ) );

      ++Count;

      delete iterInner;

      iterInner = Temp;
    }

    Que->Data_ = nullptr;

    auto Temp = Que->NextQue;

    delete Que;

    Que = Temp;
  }
}
