#pragma once

#include "ConversationTree.h"

#include "Buttons/Button.h"

class ConversationDisplay
{
  public:
    explicit ConversationDisplay( ConversationTree &&Tree, float NPCWindowSizeY, float PlayerWindowSizeY ) noexcept;

  public:
    /**
     * \brief Draws and updates the conversation display.
     *
     * \param Width - Window Width.
     * \param Hight - Window Hight.
     *
     * \return int - If and how the conversation ended, see ConversationTreeCreator::DefaultOptions for defaults.
     */
    int Draw( int Width, int Hight ) const noexcept;

  private:
    struct NPCDataStruct
    {
      explicit NPCDataStruct( const std::string &NPCString ) noexcept;
      ~NPCDataStruct() noexcept;

      void operator=( NPCDataStruct &&Move ) const noexcept;

      mutable char const *NPCText;
    };
  
    struct PlayerDataStruct
    {
      struct ConversationFunctor
      {
        struct ReturnType
        {
          ReturnType() noexcept : Value( ConversationTreeCreator::ContinueConversation ) {}
          ReturnType( int Value ) noexcept : Value( Value ) {}

          const int Value;
        };

        typedef size_t ArgType;

        explicit ConversationFunctor( const ConversationTree &Tree ) noexcept;

        int operator()( ArgType Index ) const noexcept;

        const ConversationTree &Tree;
      };

      explicit PlayerDataStruct( const ConversationTree &Tree ) noexcept;

      void operator=( PlayerDataStruct &&Move ) const noexcept;

      mutable std::vector< Button< ConversationFunctor > > PlayerOptions;
    };
    
  private:
    void DrawNPC( float Width ) const noexcept;
    int DrawPlayer( float Width, float Hight ) const noexcept;

  private:
    const ConversationTree Tree;

    const float NPCWindowSizeY;
    const float PlayerWindowSizeY;

    const NPCDataStruct NPCData;
    const PlayerDataStruct PlayerData;
};
