
#include "BaseWindow.h"

#include "Strings.h"

const ImU32 BehaviorTreeWindow_Base::LineColor = ImColor{ 1.0f, 1.0f, 0.0f, 1.0f };
const ImU32 BehaviorTreeWindow_Base::ExtraLineColor = ImColor{ 0.0f, 1.0f, 0.0f, 1.0f };

std::map< size_t, BehaviorTreeWindow_BasePtr > BehaviorTreeWindow_Base::AllWindows;
std::set< size_t > BehaviorTreeWindow_Base::OpenSlots;

BehaviorTreeWindow_BasePtr BehaviorTreeWindow_Base::FirstDerivedInstances[ TypeCount ];

bool BehaviorTreeWindow_Base::AWindowIsFocused = false;

size_t BehaviorTreeWindow_Base::Scale = 10;

ImVec2 BehaviorTreeWindow_Base::MappedScale{ 0, 0 };

ImVec2 BehaviorTreeWindow_Base::DraggedLoc{ 0, 0 };

void BehaviorTreeWindow_Base::SetNodeName( const std::string& Text ) const noexcept
{
  Name_.SetInput( Text );
}

void BehaviorTreeWindow_Base::AddParent( size_t ID ) noexcept
{
  auto Temp = AllWindows.find( ID )->second;

  ExtraLines.emplace( std::make_pair( ID - 1, std::make_pair( &Temp->Location_, &Temp->GetSizes().GetWindowSize() ) ) );
}

bool BehaviorTreeWindow_Base::RunMakeLines() noexcept
{
  if( IsFocused )
  {
    if( ImGui::IsKeyPressed( ImGui::GetKeyIndex( ImGuiKey_Delete ) ) )
    {
      isGarbage = true;

      return true;
    }

    if( ImGui::IsMouseClicked( 1 ) )
    {
      auto iter = AllWindows.find( GetWindowUnderMouse() );

      if( iter != AllWindows.end() && ExtraLines.find( iter->first - 1 ) != ExtraLines.end() )
      {
        iter->second->ChildrenIDs.erase( ID_ - 1 );

        ExtraLines.erase( iter->first - 1 );
      }
    }
  }

  if( !NewParentLine )
  {
    if( ImGui::IsMouseHoveringWindow() && ImGui::IsMouseDoubleClicked( 0 ) ) // 0 = LBM
    {
      NewParentLine = true;

      ExtraLines.emplace( std::make_pair( ID_ - 1, std::make_pair( &Location_, &GetSizes().GetWindowSize() ) ) );
    }
  }
  else if( ImGui::IsMouseClicked( 1 ) ) // 1 = RMB
  {
    NewParentLine = false;

    ExtraLines.erase( ID_ - 1 );
  }
  else if( ImGui::IsMouseClicked( 0 ) ) // 0 = LBM
  {
    auto iter = AllWindows.find( GetWindowUnderMouse() );

    if( iter != AllWindows.end() && ExtraLines.find( iter->first - 1 ) == ExtraLines.end() )
    {
      ExtraLines.erase( ID_ - 1 );

      iter->second->ChildrenIDs.emplace( ID_ - 1 );

      ExtraLines.emplace( std::make_pair( iter->first - 1, std::make_pair( &iter->second->Location_, 
                                                                           &iter->second->GetSizes().GetWindowSize() ) ) );

      NewParentLine = false;
    }
  }

  return false;
}

void BehaviorTreeWindow_Base::RunNameInput( const ImVec2 &InputScale ) noexcept
{
  ChangingName = !Name_.RunInput( InputScale );
}

static float MapNumbers( float Number, float NumberMin, float NumberMax, float OutMin, float OutMax )
{
  return ( Number - NumberMin ) / ( NumberMax - NumberMin ) * ( OutMax - OutMin ) + OutMin;
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

size_t BehaviorTreeWindow_Base::GetWindowUnderMouse() noexcept
{
  auto MousePos = ImGui::GetMousePos();
  auto OurType = GetWindowType();
  auto WindowYSize = GetSizes().GetWindowSize().y;

  for( auto iter : AllWindows )
  {
    if( iter.second != this )
    {
      auto ParentType = iter.second->GetWindowType();

      if( ( OurType == Multi && ParentType == Multi ) ||
          ( ParentType == Check && ( OurType == Check || !iter.second->GetChildren().empty() ) ) )
      {
        continue;
      }

      if( ParentType != Tree && ParentType != Null && iter.second->Location_.y + WindowYSize < Location_.y &&
          MousePos <= iter.second->Location_ + iter.second->GetSizes().GetWindowSize() + DraggedLoc &&
          MousePos >= iter.second->Location_ + DraggedLoc  )
      {
        return iter.first;
      }
    }
  }

  return AllWindows.size();
}

BehaviorTreeWindow_Base::WindowTypes BehaviorTreeWindow_Base::GetWindowType() const noexcept
{
  return WindowType_;
}

BehaviorTreeWindow_Base::BehaviorTreeWindow_Base( WindowTypes WindowType, size_t ID, const ImVec2 &Location,
                                                  const char *Name ) noexcept :
  ID_( ID ? ID : ( OpenSlots.empty() ? AllWindows.size() : *( OpenSlots.begin() ) ) ), WindowType_( WindowType ), Location_( Location ),
  Name_( Name )
{
  if( FirstDerivedInstances[ WindowType ] == nullptr )
  {
    FirstDerivedInstances[ WindowType ] = this;
  }

  switch( WindowType )
  {
    case Branch:
      Name_.SetInput( "Branch Node" );
      break;

    case Null:
      Name_.SetInput( "Null Node" );
      break;

    case Tree:
      Name_.SetInput( "Tree Node" );
      break;
    
    case Multi:
      Name_.SetInput( "Multi Node" );
      break;
    
    case Check:
      Name_.SetInput( "Check Node" );
      break;
    
    case Child:
      Name_.SetInput( "ChildTree Node" );
      break;
  }

  if( !OpenSlots.empty() )
  {
    OpenSlots.erase( OpenSlots.begin() );
  }

  AllWindows.emplace( std::make_pair( ID_, this ) );
}

BehaviorTreeWindow_Base::~BehaviorTreeWindow_Base()
{
}

void BehaviorTreeWindow_Base::RunAllWindows() noexcept
{
  static bool Dragged;

  static ImVec2 LastDrag;

  Drag( DraggedLoc, Dragged, LastDrag, true );

  auto Scroll = static_cast< int >( ImGui::GetScrollAmount() );

  Scale += ( static_cast< int >( Scale ) + Scroll < 1 ? 0 : ( static_cast< int >( Scale ) + Scroll > 10 ? 0 : Scroll ) );

  MappedScale.x = MapNumbers( Scale / 10.0f, 0, 1, 0.05f, 1 );
  MappedScale.y = MapNumbers( Scale / 10.0f, 0, 1, 0.10f, 1 );

  for( auto iter : FirstDerivedInstances )
  {
    if( iter )
    {
      iter->GetSizes().CalculateSizes( MappedScale );
    }
  }

  for( auto iter : AllWindows )
  {
    if( iter.second->isGarbage )
    {
      iter.second->DestroyWindow();

      break;
    }
  }

  auto WindowFocused = false;

  for( auto iter : AllWindows )
  {
    iter.second->RunWindow();

    if( iter.second->IsFocused )
    {
      WindowFocused = true;
    }
  }

  AWindowIsFocused = WindowFocused;
}

const std::map< size_t, BehaviorTreeWindow_BasePtr > & BehaviorTreeWindow_Base::GetAllWindows() noexcept
{
  return AllWindows;
}

std::map< size_t, BehaviorTreeWindow_BasePtr > * BehaviorTreeWindow_Base::EditAllWindows() noexcept
{
  return &AllWindows;
}

Static_NoInstance_Variables::Static_NoInstance_Variables( const ImVec2 &WindowSize ) noexcept: WindowSize_( WindowSize )
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

  DiffrenceSize = (WindowSize_ - ScaleSize) / 2.0f;
}

BehaviorTreeWindow_Level1::BehaviorTreeWindow_Level1( WindowTypes WindowType, const char* FunctionText, size_t ID ) noexcept:
  BehaviorTreeWindow_Base( WindowType, ID ), FunctionName( FunctionText, 28 )
{
}

BehaviorTreeWindow_Level1::BehaviorTreeWindow_Level1( WindowTypes WindowType, const ImVec2 &Location, const char *Name,
                                                      const char* FunctionText ) noexcept:
  BehaviorTreeWindow_Base( WindowType, 0, Location, Name ), FunctionName( FunctionText, 28 )
{
}

const char * BehaviorTreeWindow_Level1::GetFunctionName() const noexcept
{
  return FunctionName.GetInput();
}

void BehaviorTreeWindow_Level1::RunFunctionInput( const ImVec2 &InputScale ) const noexcept
{
  FunctionName.RunInput(InputScale);
}

void BehaviorTreeWindow_Level1::SetFunctionInput( const std::string &Text) const noexcept
{
  FunctionName.SetInput(Text);
}

BehaviorTreeWindow_Level2::BehaviorTreeWindow_Level2( WindowTypes WindowType, const char *FunctionText, size_t ID ) noexcept:
  BehaviorTreeWindow_Level1( WindowType, FunctionText, ID ), DataKeyString( "Data Key", 11, true )
{
}

BehaviorTreeWindow_Level2::BehaviorTreeWindow_Level2( WindowTypes WindowType, const ImVec2 &Location, const char *Name, 
                                                      const char *FunctionText ) noexcept:
  BehaviorTreeWindow_Level1( WindowType, Location, Name, FunctionText ), DataKeyString( "Data Key", 11, true )
{
}

void BehaviorTreeWindow_Level2::RunDataInput( const ImVec2 &InputScale ) const noexcept
{
  DataKeyString.RunInput( InputScale );
}

void BehaviorTreeWindow_Level2::SetDataKey( const std::string &Number ) const noexcept
{
  DataKeyString.SetInput( Number );
}

size_t BehaviorTreeWindow_Level2::GetDataKey() const noexcept
{
  return static_cast< size_t >( atoll( DataKeyString.GetInput() ) );
}

size_t BehaviorTreeWindow_Base::GetParentID() const noexcept
{
  return ParentID;
}

size_t BehaviorTreeWindow_Base::GetID() const noexcept
{
  return ID_;
}

const ExtraLinesContainer & BehaviorTreeWindow_Base::GetOtherParents() const noexcept
{
  return ExtraLines;
}

bool BehaviorTreeWindow_Base::GetIsFocused() noexcept
{
  return AWindowIsFocused;
}

void BehaviorTreeWindow_Base::RunShowLines() noexcept
{
  const auto Center = ImVec2( ( X - 200 ) / 2.0f, Y / 2.0f );

  auto Sizes = GetSizes();

  auto Diffrence = Sizes.GetDiffrence();

  auto WindowSize = Sizes.GetWindowSize();

  auto Direction = DirectionVector( Center, Location_ + DraggedLoc + WindowSize / 2.0f ) * Diffrence * 1.0f;

  ImGui::SetNextWindowSize( Sizes.GetScaleSize() );

  ImGui::SetNextWindowPos( Location_ + DraggedLoc + Diffrence + Direction );

  if( GetWindowType() != Trunk )
  {
    for( auto iter : ExtraLines )
    {
      if( iter.second.first != &Location_ )
      {
        auto LineID = ( iter.first == TrunkID ? 0 : iter.first + 1 );

        if( OpenSlots.find( LineID ) != OpenSlots.end() )
        {
          ExtraLines.erase( iter.first );

          break;
        }

        if( iter.second.first->y + iter.second.second->y  > Location_.y )
        {
          ExtraLines.erase( iter.first );

          break;
        }

        auto OtherDiffrence = AllWindows.find( LineID )->second->GetSizes().GetDiffrence();

        ImGui::GetWindowDrawList()->AddLine( ImVec2( Location_.x + WindowSize.x / 2, Location_.y ) + DraggedLoc + Direction +
                                             ImVec2( 0, Diffrence.y ),
                                             ImVec2( iter.second.first->x + iter.second.second->x / 2, 
                                                     iter.second.first->y + iter.second.second->y ) +
                                             DraggedLoc + ImVec2( 0, -OtherDiffrence.y ) +
                                             DirectionVector( Center, *iter.second.first + DraggedLoc + *iter.second.second /2.0f ) *
                                             OtherDiffrence, ExtraLineColor, 2 );
      }
      else
      {
        ImGui::GetWindowDrawList()->AddLine( ImVec2( Location_.x + WindowSize.x / 2, Location_.y ) + DraggedLoc + Diffrence + Direction,
                                             ImGui::GetMousePos(), ExtraLineColor, 2 );
      }
    }

    if( Location_.y > 100 )
    {
      auto Other = FindClosestWindow();

      if( Other != this )
      {
        auto OtherLocation = Other->Location_;

        auto OtherSizes = Other->GetSizes();

        auto OtherDiffrence = OtherSizes.GetDiffrence();

        auto OtherWindowSize = OtherSizes.GetWindowSize();

        ImGui::GetWindowDrawList()->AddLine( ImVec2( Location_.x + WindowSize.x / 2, Location_.y ) + DraggedLoc + Direction +
                                             ImVec2( 0, Diffrence.y ),
                                             ImVec2( OtherLocation.x + OtherWindowSize.x / 2, OtherLocation.y + OtherWindowSize.y ) +
                                             DraggedLoc + ImVec2( 0, -OtherDiffrence.y ) +
                                             DirectionVector( Center, OtherLocation + DraggedLoc + OtherWindowSize / 2.0f ) *
                                             OtherDiffrence, LineColor, 2 );
      }
    }

    if( !ChangingName )
    {
      TempName = Name_.GetInput();

      TempName << "##" << GetWindowType() << ID_;
    }
  }
}

void BehaviorTreeWindow_Base::RunFocused() noexcept
{
  ImGui::SetWindowFontScale( Scale / 10.0f );

  IsFocused = ImGui::IsWindowFocused();

  if( IsFocused )
  {
    Drag( Location_, Moved, PrevMov );
  }
}

void BehaviorTreeWindow_Base::SetLocation( const ImVec2& Loc ) noexcept
{
  Location_ = Loc;
}

void BehaviorTreeWindow_Base::DestroyWindow() const noexcept
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

BehaviorTreeWindow_BaseDoubleConstPtr BehaviorTreeWindow_Base::FindClosestWindow() noexcept
{
  BehaviorTreeWindow_BasePtr ClosestWindowID = nullptr;

  auto ClosestWindowDistance = std::numeric_limits< float >::max();

  for( auto iter : AllWindows )
  {
    if( iter.second != this )
    {
      auto ParentType = iter.second->GetWindowType();

      if( ParentType != Tree && ParentType != Null )
      {
        if( ( WindowType_ == Multi && ParentType == Multi ) || ( WindowType_ == Check && ParentType == Check ) )
        {
          continue;
        }

        if( ParentType == Check )
        {
          auto Children = &iter.second->ChildrenIDs;

          if( !Children->empty() )
          {
            if( Children->find( ID_ - 1 ) == Children->end() )
            {
              continue;
            }

            Children->erase( ID_ - 1 );
          }
        }

        if( ExtraLines.find( iter.first - 1 ) == ExtraLines.end() &&
            iter.second->Location_.y + iter.second->GetSizes().GetWindowSize().y < Location_.y )
        {
          auto Distance = FindSquaredDistance( iter.second );

          if( Distance < ClosestWindowDistance )
          {
            ClosestWindowDistance = Distance;

            ClosestWindowID = iter.second;

            ParentID = iter.first - 1;
          }

          auto Found = iter.second->ChildrenIDs.find( ID_ - 1 );

          if( Found != iter.second->ChildrenIDs.end() )
          {
            iter.second->ChildrenIDs.erase( ID_ - 1 );
          }
        }
      }
    }
  }

  if( ClosestWindowID )
  {
    if( ClosestWindowID->GetWindowType() == Check )
    {
      ClosestWindowID->ChildrenIDs.clear();
    }

    ClosestWindowID->ChildrenIDs.emplace( ID_ - 1 );

    return ClosestWindowID;
  }

  return this;
}

float BehaviorTreeWindow_Base::FindSquaredDistance( BehaviorTreeWindow_BasePtr Other ) const noexcept
{
  auto OtherWindowSize = Other->GetSizes().GetWindowSize();

  auto doubleX = ( Other->Location_.x + OtherWindowSize.x / 2 ) - ( Location_.x + GetSizes().GetWindowSize().x / 2 );

  auto doubleY = Other->Location_.y + OtherWindowSize.y - Location_.y;

  return doubleX * doubleX + doubleY * doubleY;
}

const char * BehaviorTreeWindow_Base::GetNodeName() const noexcept
{
  return Name_.GetInput();
}

const char * BehaviorTreeWindow_Base::GetTempName() const noexcept
{
  return TempName.c_str();
}

const std::set< size_t > & BehaviorTreeWindow_Base::GetChildren() const noexcept
{
  return ChildrenIDs;
}

const ImVec2 & BehaviorTreeWindow_Base::GetLocation() const noexcept
{
  return Location_;
}

ImVec2 BehaviorTreeWindow_Base::GetInputScale() noexcept
{
  return ImVec2( Scale / 10.0f, Scale / 10.0f );
}

const ImVec2& BehaviorTreeWindow_Base::GetMappedScale() noexcept
{
  return MappedScale;
}
