/*!------------------------------------------------------------------------------
\file   GameState.h

\author Garrett Conti

\par    Project: GameStateManager
\par    Course:  S17

\copyright 
   ------------------------------------------------------------------------------ */

#pragma once

class GameState
{
  public:
    /**
     * \brief Loads everything that is not affected by a restart or screen resize, also runs all of the loading functions.
     *
     * \return void
     */
    explicit GameState() noexcept {}

    /**
     * \brief Unloads everything that is not affected by a restart or screen resize, also runs all of the unloading functions.
     *
     * \return void
     */
    virtual ~GameState() noexcept = 0 {} // Seems contradictory, but every class requires a dtor

  public:
    /**
     * \brief Initializes everything that is affected by a restart.
     *
     * \return void
     *
     * This will be called when the Game State restarts.
     */
    virtual void DataInitialize() = 0;

    /**
     * \brief Initializes everything that is affected by a screen resize.
     *
     * \return void
     *
     * This will be called when the Game State restarts or the screen is resized.
     */
    virtual void ObjectInitialize() = 0;

    /**
     * \brief Updates the data for the Game State.
     *
     * \param dt - The delta time, this will never be zero.
     *     
     * \return void
     *
     * This will be called every update cycle, unless the State is paused.
     */
    virtual void Update( const double dt ) = 0;

    /**
     * \brief Draws the objects for the Game State.
     *
     * \param dt - The delta time, this can be zero.
     *
     * \return void
     * 
     * This will be called every update cycle, even if the State is paused. (If paused, a 0 dt will be passed in)
     */
    virtual void Draw( const double dt ) = 0;

    /**
     * \brief Shuts down everything that is affected by a screen resize.
     *
     * \return void
     *
     * This will be called when the Game State restarts or the screen is resized.
     */
    virtual void ObjectShutdown() noexcept = 0;

    /**
     * \brief Shuts down everything that is affected by a restart.
     *
     * \return void
     *
     * This will be called when the Game State restarts.
     */
    virtual void DataShutdown() noexcept = 0;
};
