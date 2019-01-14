
#include "BaseWindow.h"

#include "ImVec.h"

const ImU32 ConversationTreeWindow_Base::LineColor = ImColor{ 1.0f, 1.0f, 0.0f, 1.0f };

std::map< size_t, ConversationTreeWindow_BasePtr > ConversationTreeWindow_Base::AllWindows;
std::set< size_t > ConversationTreeWindow_Base::OpenSlots;

ConversationTreeWindow_BasePtr ConversationTreeWindow_Base::FirstDerivedInstances[ TypeCount ];

bool ConversationTreeWindow_Base::AWindowIsFocused = false;

size_t ConversationTreeWindow_Base::Scale = 10;

ImVec2 ConversationTreeWindow_Base::MappedScale{ 1, 1 };

ImVec2 ConversationTreeWindow_Base::DraggedLoc{ 0, 0 };

ConversationTreeWindow_Base::ConversationTreeWindow_Base( WindowTypes WindowType, const ImVec2 &Location, size_t ParentID ) noexcept :
  ID_( OpenSlots.empty() ? AllWindows.size() : *( OpenSlots.begin() ) ), ParentID_( ParentID ), WindowType_( WindowType ), 
  Location_( Location )
{
  InitWindow();
}

ConversationTreeWindow_Base::ConversationTreeWindow_Base( WindowTypes WindowType, const ImVec2 &ParentLocation,
                                                            const ImVec2 &ParentSize, float OurXSize, size_t ParentID ) noexcept :
  ID_( OpenSlots.empty() ? AllWindows.size() : *( OpenSlots.begin() ) ), ParentID_( ParentID ), WindowType_( WindowType ),
  Location_( CalculateLocation( ParentLocation, ParentSize, OurXSize ) )
{
  InitWindow();
}

ConversationTreeWindow_Base::~ConversationTreeWindow_Base()
{
}

void ConversationTreeWindow_Base::SetGarbage() noexcept
{
  isGarbage = true;
}

void ConversationTreeWindow_Base::InitWindow() noexcept
{
  auto WindowType = GetWindowType();

  if( FirstDerivedInstances[ WindowType ] == nullptr )
  {
    FirstDerivedInstances[ WindowType ] = this;
  }

  switch( WindowType )
  {
    case NPC:
      TempName = "NPC Node##" + std::to_string( ID_ );
      break;

    case Player:
      TempName = "Player Node##" + std::to_string( ID_ );
      break;
  }

  if( !OpenSlots.empty() )
  {
    OpenSlots.erase( OpenSlots.begin() );
  }

  AllWindows.emplace( std::make_pair( ID_, this ) );
}

static void Drag( ImVec2 &Vec, bool &Dragged, ImVec2 &LastDrag, bool RMB = false )
{
  auto Temp( ImGui::GetMouseDragDelta( RMB ) );

  Temp *= -1;

  if( ( Temp.x || Temp.y ) && !Dragged )
  {
    Vec += Temp;

    LastDrag = Temp;

    Dragged = true;

    return;
  }

  if( Dragged && !Temp.x && !Temp.y )
  {
    LastDrag = Temp;

    Dragged = false;

    return;
  }

  if( LastDrag.x != Temp.x )
  {
    Vec.x += LastDrag.x - Temp.x;

    LastDrag.x = Temp.x;
  }

  if( LastDrag.y != Temp.y )
  {
    Vec.y += LastDrag.y - Temp.y;

    LastDrag.y = Temp.y;
  }
}

static ImVec2 DirectionVector( const ImVec2 &Vec1, const ImVec2 &Vec2 )
{
  auto Temp( Vec1 - Vec2 );

  return ( Temp /= sqrtf( Temp.x * Temp.x + Temp.y * Temp.y ) );
}

size_t ConversationTreeWindow_Base::GetWindowUnderMouse() noexcept
{
  auto MousePos = ImGui::GetMousePos();
  auto WindowYSize = GetSizes().GetWindowSize().y;

  for( auto iter : AllWindows )
  {
    if( iter.second != this && iter.second->Location_.y + WindowYSize < Location_.y &&
        MousePos <= iter.second->Location_ + iter.second->GetSizes().GetWindowSize() + DraggedLoc &&
        MousePos >= iter.second->Location_ + DraggedLoc )
    {
      return iter.first;
    }
  }

  return AllWindows.size();
}

float ConversationTreeWindow_Base::MapNumber( float Number, float NumberMin, float NumberMax, float OutMin, float OutMax ) noexcept
{
  return ( Number - NumberMin ) / ( NumberMax - NumberMin ) * ( OutMax - OutMin ) + OutMin;
}

ImVec2 ConversationTreeWindow_Base::CalculateLocation( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, float OurXSize ) noexcept
{
  return ImVec2{ ParentLocation.x + ( ParentSize.x - OurXSize ) / 2, ParentLocation.y + ParentSize.y + 40 };
}

ConversationTreeWindow_Base::WindowTypes ConversationTreeWindow_Base::GetWindowType() const noexcept
{
  return WindowType_;
}

void ConversationTreeWindow_Base::RunAllWindows() noexcept
{
  static bool Dragged;

  static ImVec2 LastDrag;

  Drag( DraggedLoc, Dragged, LastDrag, true );

  /*auto Scroll = static_cast< int >( ImGui::GetScrollAmount() );

  Scale += ( static_cast< int >( Scale ) + Scroll < 1 ? 0 : ( static_cast< int >( Scale ) + Scroll > 10 ? 0 : Scroll ) );

  MappedScale.x = MapNumber( Scale / 10.0f, 0, 1, 0.05f, 1 );
  MappedScale.y = MapNumber( Scale / 10.0f, 0, 1, 0.10f, 1 );*/

  for( auto Iter : FirstDerivedInstances )
  {
    if( Iter )
    {
      Iter->GetSizes().CalculateSizes( MappedScale );
    }
  }

  bool Destroyed;

  do
  {
    Destroyed = false;

    for( auto Iter : AllWindows )
    {
      if( Iter.second->isGarbage )
      {
        Iter.second->DestroyWindow();

        Destroyed = true;

        break;
      }
    }
  }
  while( Destroyed );

  for( auto Iter : AllWindows )
  {
    Iter.second->RunWindow();

    if( Iter.second->IsFocused )
    {
      AWindowIsFocused = true;
    }
  }

  AllWindows.find( 0 )->second->CheckPosition( AllWindows );
}

const std::map< size_t, ConversationTreeWindow_BasePtr > & ConversationTreeWindow_Base::GetAllWindows() noexcept
{
  return AllWindows;
}

std::map< size_t, ConversationTreeWindow_BasePtr > * ConversationTreeWindow_Base::EditAllWindows() noexcept
{
  return &AllWindows;
}

Static_NoInstance_Variables::Static_NoInstance_Variables( const ImVec2 &WindowSize ) noexcept : WindowSize_( WindowSize )
{
}

const ImVec2 & Static_NoInstance_Variables::GetWindowSize() const noexcept
{
  return WindowSize_;
}

const ImVec2 & Static_NoInstance_Variables::GetDiffrence() const noexcept
{
  return DiffrenceSize;
}

const ImVec2 & Static_NoInstance_Variables::GetScaleSize() const noexcept
{
  return ScaleSize;
}

void Static_NoInstance_Variables::CalculateSizes( const ImVec2 &MappedScale ) noexcept
{
  ScaleSize = WindowSize_ * MappedScale;

  DiffrenceSize = ( WindowSize_ - ScaleSize ) / 2.0f;
}

void Static_NoInstance_Variables::SetNewWindowSize( const ImVec2 &WindowSize ) noexcept
{
  WindowSize_ = WindowSize;
}

size_t ConversationTreeWindow_Base::GetParentID() const noexcept
{
  return ParentID_;
}

size_t ConversationTreeWindow_Base::GetID() const noexcept
{
  return ID_;
}

bool ConversationTreeWindow_Base::GetIsFocused() noexcept
{
  return AWindowIsFocused;
}


void ConversationTreeWindow_Base::RunSetWindow() const noexcept
{
  const auto Sizes = GetSizes();

  const auto Diffrence = Sizes.GetDiffrence();

  ImGui::SetNextWindowSize( Sizes.GetScaleSize() );

  ImGui::SetNextWindowPos( Location_ + DraggedLoc + Diffrence +
                           DirectionVector( ImVec2( X / 2 - 450.0f / 4, Y / 2.0f - 20 ), Location_ + DraggedLoc + Sizes.GetWindowSize() / 2.0f ) * Diffrence * 1.0f );
}

void ConversationTreeWindow_Base::RunShowLines() const noexcept
{
  const auto Center = ImVec2( X / 2 - 450.0f / 4, Y / 2.0f - 20 );

  const auto Sizes = GetSizes();

  const auto Diffrence = Sizes.GetDiffrence();

  const auto WindowSize = Sizes.GetWindowSize();

  const auto Other = AllWindows.find( GetParentID() )->second;

  const auto OtherLocation = Other->Location_;

  const auto OtherSizes = Other->GetSizes();

  const auto OtherDiffrence = OtherSizes.GetDiffrence();

  const auto OtherWindowSize = OtherSizes.GetWindowSize();

  ImGui::GetWindowDrawList()->AddLine( ImVec2( Location_.x + WindowSize.x / 2, Location_.y ) + DraggedLoc +
                                       DirectionVector( Center, Location_ + DraggedLoc + WindowSize / 2.0f ) * Diffrence * 1.0f +
                                       ImVec2( 0, Diffrence.y ),
                                       ImVec2( OtherLocation.x + OtherWindowSize.x / 2, OtherLocation.y + OtherWindowSize.y ) +
                                       DraggedLoc + ImVec2( 0, -OtherDiffrence.y ) +
                                       DirectionVector( Center, OtherLocation + DraggedLoc + OtherWindowSize / 2.0f ) *
                                       OtherDiffrence, LineColor, 2 );
}

void ConversationTreeWindow_Base::RunFocused() noexcept
{
  ImGui::SetWindowFontScale( Scale / 10.0f );

  IsFocused = ImGui::IsWindowFocused();
}

void ConversationTreeWindow_Base::SetLocation( const ImVec2& Loc ) noexcept
{
  Location_ = Loc;
}

void ConversationTreeWindow_Base::DestroyWindow() const noexcept
{
  OpenSlots.emplace( ID_ );

  auto Deleter = AllWindows.find( ID_ )->second;

  auto Type = Deleter->GetWindowType();

  if( FirstDerivedInstances[ Type ] == Deleter )
  {
    FirstDerivedInstances[ Type ] = nullptr;
  }

  delete Deleter;

  AllWindows.erase( ID_ );
}

const char * ConversationTreeWindow_Base::GetTempName() const noexcept
{
  return TempName.c_str();
}

const ImVec2 & ConversationTreeWindow_Base::GetLocation() const noexcept
{
  return Location_;
}

ImVec2 ConversationTreeWindow_Base::GetInputScale() noexcept
{
  return ImVec2( Scale / 10.0f, Scale / 10.0f );
}

const ImVec2 & ConversationTreeWindow_Base::GetMappedScale() noexcept
{
  return MappedScale;
}

const ImVec2 & ConversationTreeWindow_Base::GetDraggedLoc() noexcept
{
  return DraggedLoc;
}
