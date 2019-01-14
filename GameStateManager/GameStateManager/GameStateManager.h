/*!------------------------------------------------------------------------------
\file   GameStateManager.h

\author Garrett Conti

\par    Project: GameStateManager
\par    Course:  S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

#include <stack>

#include "GameState.h"
#include "Tracer.h"

#ifdef GAMESTATE_EXPORTS
#define GAMESTATE_API __declspec(dllexport)
#else

#ifdef GAMESTATE_DEBUG
#define GAMESTATE_API
#else
#define GAMESTATE_API __declspec(dllimport)
#endif

#endif

#ifdef RDEBUG
#define _DEBUG
#endif

class GameStateManager
{
  public:
    GAMESTATE_API explicit GameStateManager( GameState *FirstState ) noexcept;
    GAMESTATE_API ~GameStateManager() noexcept;

  public:
    /**
     * \brief Runs a new Games State
     *
     * \param NewState - The new Game State to run
     *
     * \return void
     * 
     * Only affects the currently running Game State and not the other States in the stack.
     * Use this when you don't need to go back to the previous State.
     */
    GAMESTATE_API void Set( GameState *NewState ) noexcept;

    /**
     * \brief Pushes a new Game State onto the stack.
     *
     * \param NewState - The new Game State to run
     *
     * \return void
     * 
     * Does not affect the currently running State or the other States in the stack.
     * Use this when you could to go back to the previous State.
     */
    GAMESTATE_API void Push( GameState *NewState ) noexcept;

    /**
     * \brief Pops off the currently running Game State and runs the one under it.
     *
     * \return void
     * 
     * Use this when you need to go back to the previous State.
     */
    GAMESTATE_API void Pop() ExceptDebug;

  public:
    /**
     * \brief Pauses the currently running Game State, optionally running an Overlay State on top of it.
     *
     * \param OverlayState - The Game State to run over top of the paused Game State.
     *
     * \return void
     * 
     * Only the Draw funtion will be called on the Paused State.
     * If an Overlay State is provided, its Update and Draw will be called after the Paused State's Draw.
     */
    GAMESTATE_API void Pause( GameState *OverlayState = nullptr ) ExceptDebug;
    GAMESTATE_API void Resume() ExceptDebug;

    GAMESTATE_API void Resize() noexcept;
    GAMESTATE_API void Restart() noexcept;

  public:
    GAMESTATE_API void Update( double dt ) noexcept;

  private:
    struct NoPushedGameStates
    {
      GAMESTATE_API explicit NoPushedGameStates() noexcept;
    };

    struct ManagerAlreadyPaused
    {
      GAMESTATE_API explicit ManagerAlreadyPaused() noexcept;
    };

    struct ManagerNotPaused
    {
      GAMESTATE_API explicit ManagerNotPaused() noexcept;
    };

  private:
    GameState *CurrentState;
    std::stack< GameState* > GameStateStack;

    GameState *PausedOverlay = nullptr;
    bool Paused = false;
};
