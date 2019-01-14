/*!----------------------------------------------------------------------------
\file   GameStateManager.h

\author Garrett Conti

\par    Project: GameStateManager
\par    Course:  S17

\copyright 
   ---------------------------------------------------------------------------- */

#include <iostream>

#include "GameStateManager.h"

GameStateManager::GameStateManager( GameState *const FirstState ) noexcept : CurrentState( FirstState )
{
}

GameStateManager::~GameStateManager() noexcept
{
  const size_t Size = GameStateStack.size();

  for( size_t i = 0; i < Size; ++i )
  {
    delete GameStateStack.top();

    GameStateStack.pop();
  }

  delete CurrentState;
}

///////////////////////////////////////////////////////////////////////////////

void GameStateManager::Set( GameState *const NewState ) noexcept
{ 
  delete CurrentState;
  CurrentState = NewState;
}

void GameStateManager::Push( GameState *const NewState ) noexcept
{
  GameStateStack.push( CurrentState );
  CurrentState = NewState;
}

void GameStateManager::Pop() ExceptDebug
{
  AssertDebug( GameStateStack.empty(), NoPushedGameStates{} )

  delete CurrentState;
  CurrentState = GameStateStack.top();
  GameStateStack.pop();

  // In case a resize occured
  CurrentState->ObjectShutdown();

  CurrentState->ObjectInitialize();
}

///////////////////////////////////////////////////////////////////////////////

void GameStateManager::Pause( GameState *const OverlayState ) ExceptDebug
{
  AssertDebug( Paused, ManagerAlreadyPaused{} )

  PausedOverlay = OverlayState;
  Paused = true;
}

void GameStateManager::Resume() ExceptDebug
{
  AssertDebug( !Paused, ManagerNotPaused{} )

  delete PausedOverlay;
  PausedOverlay = nullptr;
  Paused = false;
}

///////////////////////////////////////////////////////////////////////////////

void GameStateManager::Resize() noexcept
{
  CurrentState->ObjectShutdown();

  CurrentState->ObjectInitialize();
}

void GameStateManager::Restart() noexcept
{
  CurrentState->ObjectShutdown();

  CurrentState->DataShutdown();

  CurrentState->DataInitialize();

  CurrentState->ObjectInitialize();
}

///////////////////////////////////////////////////////////////////////////////

void GameStateManager::Update( const double dt ) noexcept
{
  if( Paused )
  {
    CurrentState->Draw( 0 );

    if( PausedOverlay )
    {
      PausedOverlay->Update( dt );
      PausedOverlay->Draw( dt );
    }
  }
  else
  {
    CurrentState->Update( dt );
    CurrentState->Draw( dt );
  }
}

///////////////////////////////////////////////////////////////////////////////

GameStateManager::NoPushedGameStates::NoPushedGameStates() noexcept
{
  std::cerr << "There are no previously pushed game states to pop!" << std::endl;
}

GameStateManager::ManagerAlreadyPaused::ManagerAlreadyPaused() noexcept
{
  std::cerr << "The Game State Manager is already paused!" << std::endl;
}

GameStateManager::ManagerNotPaused::ManagerNotPaused() noexcept
{
  std::cerr << "The Game State Manager is not paused!" << std::endl;
}
