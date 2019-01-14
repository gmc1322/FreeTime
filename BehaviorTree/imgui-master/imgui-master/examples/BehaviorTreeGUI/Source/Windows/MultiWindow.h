/*!------------------------------------------------------------------------------
\file   MultiWindow.h

\author Garrett Conti

\par    Project: BehaviorTreeGUI
\par    Course:  GAM200S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include "BaseWindow.h"

#include "Toggle.h"
#include "Input.h"
#include "Switch.h"

typedef class BehaviorTreeWindow_Multi *BehaviorTreeWindow_MultiPtr;
typedef class BehaviorTreeWindow_Multi const * const BehaviorTreeWindow_MultiDoubleConstPtr;

class BehaviorTreeWindow_Multi : public BehaviorTreeWindow_Base
{
  public:
    enum RegeneratingState
    {
      Constant,
      Dynamic,
      Regenerating,

      Count
    };

  public:
    BehaviorTreeWindow_Multi( size_t ID = 0 ) noexcept;

    ~BehaviorTreeWindow_Multi() override;

  public:
    Static_NoInstance_Variables & GetSizes() const noexcept override;

  public:
    bool GetIsGenerating() const noexcept;
    const char * GetGenFunction() const noexcept;
    const char * GetGenCount() const noexcept;
    const char * GetRunFunction() const noexcept;
    const char * GetGenType() const noexcept;

  public:
    void SetGenerating() noexcept;

    void SetGenFunction( const std::string &String ) const noexcept;
    void SetGenCount( const std::string &String ) const noexcept;
    void SetRunFunction( const std::string &String ) const noexcept;
    void SetGenType( const std::string &String ) noexcept;

  private:
    void RunWindow() noexcept override;

  private:
    static Static_NoInstance_Variables SmallSize;
    static Static_NoInstance_Variables LargeSize;

    static BehaviorTreeWindow_MultiPtr SizesRunner;

  private:
    Toggle EnableGenerator;

  private:
    Switch RegenerationType;

  private:
    Input GeneratorFunction;
    Input GenerateCount;
    Input RunFunction;

  private:
    bool IsGenerating = false;
};