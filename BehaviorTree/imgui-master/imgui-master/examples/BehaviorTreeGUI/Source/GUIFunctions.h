#pragma once

#include "TreeWindow.h"
#include "BranchWindow.h"
#include "NullWindow.h"
#include "MultiWindow.h"
#include "CheckWindow.h"
#include "ChildWindow.h"

#include "Input.h"

BehaviorTreeWindow_BranchPtr AddBranch( size_t ID = 0 ) noexcept;

BehaviorTreeWindow_TreePtr AddTree( size_t ID = 0 ) noexcept;

BehaviorTreeWindow_NullPtr AddNull( size_t ID = 0 ) noexcept;

BehaviorTreeWindow_MultiPtr AddMulti( size_t ID = 0 ) noexcept;

BehaviorTreeWindow_CheckPtr AddCheck( size_t ID = 0 ) noexcept;

BehaviorTreeWindow_ChildPtr AddChild( size_t ID = 0 ) noexcept;

InputPtr AddData( void *Data ) noexcept;

void RunBehaviorTreeCanvas() noexcept;

void RunBehaviorTreeNodeWindow() noexcept;

void RunHelpWindow() noexcept;

void RemoveData( void *Data ) noexcept;
