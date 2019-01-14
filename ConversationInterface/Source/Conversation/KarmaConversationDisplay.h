#pragma once

#include "KarmaTree.h"

#include "Buttons/Button.h"

class KarmaConversationDisplay
{
  public:
    explicit KarmaConversationDisplay( KarmaTree &&Tree, int InitialKarma, int MaxBad, int MaxGood, float NPCWindowSizeY, 
                                       float PlayerWindowSizeY ) noexcept;

  public:
    /**
    * \brief Draws and updates the conversation display.
    *
    * \param Width - Window Width.
    * \param Hight - Window Hight.
    *
    * \return std::pair< int, int > - If/how the conversation ended, see ConversationTreeCreator::DefaultOptions for defaults,
    *                                   and the resulting karma increment/decrement.
    */
    std::pair< int, int > Draw( int Width, int Hight ) const noexcept;

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
      struct KarmaConversationFunctor
      {
        struct ReturnType
        {
          ReturnType() noexcept : Value( KarmaTreeCreator::ContinueConversation ) {}
          ReturnType( int Value ) noexcept : Value( Value ) {}

          const int Value;
        };

        typedef size_t ArgType;

        explicit KarmaConversationFunctor( const KarmaTree &Tree ) noexcept;

        int operator()( ArgType Index ) const noexcept;

        const KarmaTree &Tree;
      };

      explicit PlayerDataStruct( const KarmaTree &Tree, int InitialKarma, float MaxBad, float MaxGood ) noexcept;

      void operator=( PlayerDataStruct &&Move ) const noexcept;

      mutable std::vector< std::pair< Button< KarmaConversationFunctor >, int > > PlayerOptions;
    };

  private:
    void DrawNPC( float Width ) const noexcept;
    std::pair< int, int > DrawPlayer( float Width, float Hight ) const noexcept;

  private:
    const KarmaTree Tree;

    const float NPCWindowSizeY;
    const float PlayerWindowSizeY;

    const NPCDataStruct NPCData;
    const PlayerDataStruct PlayerData;

    const int InitialKarma;
    const float MaxBad;
    const float MaxGood;
    mutable int KarmaChange;
};
