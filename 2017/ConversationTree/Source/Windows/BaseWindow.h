#pragma once

#include <set>
#include <map>

#include "Input.h"

typedef class ConversationTreeWindow_Base *ConversationTreeWindow_BasePtr;
typedef class ConversationTreeWindow_Base const * const ConversationTreeWindow_BaseDoubleConstPtr;

typedef std::map< size_t, std::pair< const ImVec2 *, const ImVec2 * > > ExtraLinesContainer;

extern int X, Y;

class ConversationTreeWindow_Base
{
  public:
    enum WindowTypes
    {
      NPC,    // The NPC dialogue
      Player, // The Player dialogue

      TypeCount
    };

  public:
    explicit ConversationTreeWindow_Base( WindowTypes WindowType, const ImVec2 &Location, size_t ParentID ) noexcept;

    explicit ConversationTreeWindow_Base( WindowTypes WindowType, const ImVec2 &ParentLocation, const ImVec2 &ParentSize,
                                          float OurXSize, size_t ParentID ) noexcept;

    virtual ~ConversationTreeWindow_Base() = 0;

  public:
    static float MapNumber( float Number, float NumberMin, float NumberMax, float OutMin, float OutMax ) noexcept;

    static ImVec2 CalculateLocation( const ImVec2 &ParentLocation, const ImVec2 &ParentSize, float OurXSize ) noexcept;

  public:
    static void RunAllWindows() noexcept;

  public:
    void RunSetWindow() const noexcept;
    void RunShowLines() const noexcept;
    void RunFocused() noexcept;

  public:
    static const std::map< size_t, ConversationTreeWindow_BasePtr > & GetAllWindows() noexcept;
    static std::map< size_t, ConversationTreeWindow_BasePtr > * EditAllWindows() noexcept;

    static bool GetIsFocused() noexcept;

    static ImVec2 GetInputScale() noexcept;

    static const ImVec2 & GetMappedScale() noexcept;

    static const ImVec2 & GetDraggedLoc() noexcept;

  public:
    virtual class Static_NoInstance_Variables & GetSizes() const noexcept = 0;

  public:
    const char * GetTempName() const noexcept;

  public:
    size_t GetParentID() const noexcept;
    size_t GetID() const noexcept;

    size_t GetWindowUnderMouse() noexcept;

  public:
    WindowTypes GetWindowType() const noexcept;

  public:
    const ImVec2 & GetLocation() const noexcept;

  public:
    virtual void SetLocation( const ImVec2 &Loc ) noexcept = 0;

  public:
    void SetGarbage() noexcept;

  public:
    virtual int CheckPosition( const std::map< size_t, ConversationTreeWindow_BasePtr > &AllWindows ) noexcept = 0;

  public:
    static const size_t TrunkID = static_cast< size_t >( -1 );

  private:
    void InitWindow() noexcept;

    void DestroyWindow() const noexcept;

  private:
    virtual void RunWindow() noexcept = 0;

  private:
    static const ImU32 LineColor;

  private:
    static std::map< size_t, ConversationTreeWindow_BasePtr > AllWindows;

    static std::set< size_t > OpenSlots;

    static bool AWindowIsFocused;

    static ImVec2 DraggedLoc;

    static size_t Scale;

    static ImVec2 MappedScale;

    static ConversationTreeWindow_BasePtr FirstDerivedInstances[ TypeCount ];

  private:
    const size_t ID_;

    const size_t ParentID_;

    const WindowTypes WindowType_;

  private:
    ImVec2 Location_;

  private:
    std::string TempName;

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

  public:
      void CalculateSizes( const ImVec2 &MappedScale ) noexcept;
      void SetNewWindowSize( const ImVec2 &WindowSize ) noexcept;

  public:
    ImVec2 WindowSize_;
    ImVec2 DiffrenceSize;
    ImVec2 ScaleSize;
};
