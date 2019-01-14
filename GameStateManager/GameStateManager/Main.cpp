
#include <iostream>

#include "GameStateManager.h"

class MyGameState : public GameState
{
  public:
    explicit MyGameState() noexcept : GameState(), i( 0 ) { /*Load Stuff*/ MyGameState::DataInitialize(); MyGameState::ObjectInitialize();  }
    ~MyGameState() noexcept { MyGameState::ObjectShutdown(); MyGameState::DataShutdown(); /*Unload Stuff*/  }

  public:
    void DataInitialize() noexcept override { std::cout << "YAY" << std::endl; }
    void ObjectInitialize() noexcept override {}
    void Update( const double ) noexcept override { std::cout << "Update" << ++i << std::endl; }
    void Draw( const double dt ) noexcept override { std::cout << dt << std::endl; }
    void ObjectShutdown() noexcept override {}
    void DataShutdown() noexcept override {}

  private:
    size_t i;
};

class MyNextGameState : public GameState
{
  public:
    explicit MyNextGameState() noexcept : GameState() { /*Load Stuff*/ MyNextGameState::DataInitialize(); MyNextGameState::ObjectInitialize(); }
    ~MyNextGameState() noexcept { MyNextGameState::ObjectShutdown(); MyNextGameState::DataShutdown(); /*Unload Stuff*/ }

  public:
    void DataInitialize() noexcept override { std::cout << "NAY" << std::endl; }
    void ObjectInitialize() noexcept override {}
    void Update( const double ) noexcept override { std::cout << "Etadpu" << std::endl; }
    void Draw( const double dt ) noexcept override { std::cout << -dt << std::endl; }
    void ObjectShutdown() noexcept override {}
    void DataShutdown() noexcept override {}
};

int main()
{
  {
    GameStateManager Manager{ new MyGameState{} };

    Manager.Resize();
    Manager.Restart();

    Manager.Update( 5.2f );
    Manager.Pause();
    Manager.Update( 5 );
    Manager.Resume();
    Manager.Update( 6.0 );


    Manager.Push( new MyNextGameState{} );

    Manager.Resize();
    Manager.Restart();

    Manager.Update( 5.2f );
    Manager.Pause();
    Manager.Update( 5 );
    Manager.Resume();
    Manager.Update( 6.0 );


    Manager.Pop();

    Manager.Resize();
    Manager.Restart();

    Manager.Update( 5.2f );
    Manager.Pause();
    Manager.Update( 5 );
    Manager.Resume();
    Manager.Update( 6.0 );
  }

  return 0;
}