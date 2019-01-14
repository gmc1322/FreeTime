#pragma once

#include <set>
#include <map>

#include "ImVec.h"

#include "Input.h"

typedef class BehaviorTreeWindow_Base *BehaviorTreeWindow_BasePtr;
typedef class BehaviorTreeWindow_Base const * const BehaviorTreeWindow_BaseDoubleConstPtr;

typedef class BehaviorTreeWindow_Level1 *BehaviorTreeWindow_Level1Ptr;
typedef class BehaviorTreeWindow_Level1 const * const BehaviorTreeWindow_Level1DoubleConstPtr;

typedef class BehaviorTreeWindow_Level2 *BehaviorTreeWindow_Level2Ptr;
typedef class BehaviorTreeWindow_Level2 const * const BehaviorTreeWindow_Level2DoubleConstPtr;

typedef std::map< size_t, std::pair< const ImVec2 *, const ImVec2 * > > ExtraLinesContainer;

extern int X, Y;

class BehaviorTreeWindow_Base
{
  public:
    enum WindowTypes
      {
        Branch,
        Null,
        Tree,
        Trunk,
        Multi,
        Check,
        Child,

        TypeCount
      };

  public:
    explicit BehaviorTreeWindow_Base( WindowTypes WindowType, size_t ID,
                                      const ImVec2 &Location = ImVec2( ( X - 200 ) / 2.0f, Y / 2.0f ) - DraggedLoc,
                                      const char *Name = "Node Name" ) noexcept;

    virtual ~BehaviorTreeWindow_Base();

  public:
    static void RunAllWindows() noexcept;

  public:
    static const std::map< size_t, BehaviorTreeWindow_BasePtr > & GetAllWindows() noexcept;
    static std::map< size_t, BehaviorTreeWindow_BasePtr > * EditAllWindows() noexcept;

    static bool GetIsFocused() noexcept;

    static ImVec2 GetInputScale() noexcept;

    static const ImVec2 & GetMappedScale() noexcept;

  public:
    void RunShowLines() noexcept;
    void RunFocused() noexcept;

    void RunNameInput( const ImVec2 &InputScale ) noexcept;

    bool RunMakeLines() noexcept;

  public:
    virtual class Static_NoInstance_Variables & GetSizes() const noexcept = 0;

  public:
    const char * GetNodeName() const noexcept;
    const char * GetTempName() const noexcept;

  public:
    size_t GetParentID() const noexcept;
    size_t GetID() const noexcept;

    size_t GetWindowUnderMouse() noexcept;

  public:
    WindowTypes GetWindowType() const noexcept;

  public:
    const ExtraLinesContainer & GetOtherParents() const noexcept;

    const std::set< size_t > & GetChildren() const noexcept;

    const ImVec2 & GetLocation() const noexcept;

  public:
    static const size_t TrunkID = static_cast< size_t >( -1 );

  public:
    void SetLocation( const ImVec2 &Loc ) noexcept;
    void SetNodeName( const std::string &Text ) const noexcept;
    void AddParent( size_t ID ) noexcept;

  private:
    void DestroyWindow() const noexcept;

    BehaviorTreeWindow_BaseDoubleConstPtr FindClosestWindow() noexcept;

    float FindSquaredDistance( BehaviorTreeWindow_BasePtr Other ) const noexcept;

  private:
    virtual void RunWindow() noexcept = 0;

  private:
    static const ImU32 LineColor;
    static const ImU32 ExtraLineColor;

  private:
    static std::map< size_t, BehaviorTreeWindow_BasePtr > AllWindows;

    static std::set< size_t > OpenSlots;

    static bool AWindowIsFocused;

    static ImVec2 DraggedLoc;

    static size_t Scale;

    static ImVec2 MappedScale;

    static BehaviorTreeWindow_BasePtr FirstDerivedInstances[ TypeCount ];

  private:
    const size_t ID_;

    const WindowTypes WindowType_;

  private:
    ImVec2 Location_;

    ImVec2 PrevMov;

  private:
    Input Name_;

  private:
    std::string TempName;

  private:
    ExtraLinesContainer ExtraLines;

    size_t ParentID;

    std::set< size_t > ChildrenIDs;

  private:
    bool isGarbage = false;

    bool IsFocused = false;

    bool Moved = false;

    bool NewParentLine = false;

    bool ChangingName = false;
};

class Static_NoInstance_Variables
{
  public:
    explicit Static_NoInstance_Variables( const ImVec2 &WindowSize ) noexcept;

  public:
    const ImVec2& GetWindowSize() const noexcept;

    const ImVec2& GetDiffrence() const noexcept;

    const ImVec2& GetScaleSize() const noexcept;

    void CalculateSizes( const ImVec2 &MappedScale ) noexcept;

  public:
    const ImVec2 WindowSize_;

    ImVec2 DiffrenceSize;
    ImVec2 ScaleSize;
};

class BehaviorTreeWindow_Level1 : public BehaviorTreeWindow_Base
{
  public:
    BehaviorTreeWindow_Level1( WindowTypes WindowType, const char *FunctionText, size_t ID ) noexcept;

    BehaviorTreeWindow_Level1( WindowTypes WindowType, const ImVec2 &Location, const char *Name, const char *FunctionText ) noexcept;

  public:
    const char * GetFunctionName() const noexcept;

    void RunFunctionInput( const ImVec2 &InputScale ) const noexcept;

    void SetFunctionInput( const std::string &Text ) const noexcept;

  private:
    Input FunctionName;
};

class BehaviorTreeWindow_Level2 : public BehaviorTreeWindow_Level1
{
  public:
  BehaviorTreeWindow_Level2( WindowTypes WindowType, const char* FunctionText, size_t ID ) noexcept;

  BehaviorTreeWindow_Level2( WindowTypes WindowType, const ImVec2 &Location, const char *Name, const char *FunctionText ) noexcept;

  public:
    size_t GetDataKey() const noexcept;

  void RunDataInput( const ImVec2 &InputScale ) const noexcept;

  void SetDataKey( const std::string &Number ) const noexcept;

  private:
    Input DataKeyString;
};
