#pragma once

#include <string>
#include <set>
#include <vector>
#include <tuple>
#include <stack>

#include "Triple.h"

class KarmaTreeCreator
{
  public:
    enum DefaultOptions
    {
      ContinueConversation = -1, // The conversation shall continue!
      EndConversation,           // The conversation has ended un-event-fully.

      CustomOption               // The conversation has ended in a special event! (I.E. opening up the shop)
    };

    typedef std::initializer_list< size_t > TempKeyHolder;

  public:
    struct NPC_PLAYER_Pairs
    {
      typedef std::vector< size_t > KeyType;
      typedef Triple< int, KeyType, int > KarmaContainer; //!< Required Karma, Required Keys, Resulting Karma

                       // OptionType      Text         Karma           Next 
      typedef std::tuple< int, std::string, KarmaContainer, NPC_PLAYER_Pairs > ResponseType;
      typedef std::vector< ResponseType > ResponseContainer;

      explicit NPC_PLAYER_Pairs( const std::string &NPCText ) noexcept;

      NPC_PLAYER_Pairs( const NPC_PLAYER_Pairs &Copy ) noexcept;
      NPC_PLAYER_Pairs( NPC_PLAYER_Pairs &&Move ) noexcept;

      const std::string NPC; // NPC Text
                        
      ResponseContainer PlayerResponses; // Player Text
    };

  private:
    class ID
    {
      public:
        ID() noexcept = default;
        ID( const ID &Copy ) noexcept;
        ID( ID &&Move ) noexcept;

      public:
        std::vector< size_t >::const_iterator begin() const noexcept;
        std::vector< size_t >::const_iterator end() const noexcept;

      public:
        void push_back( size_t NewID ) noexcept;

      private:
        std::vector< size_t > IDs = std::vector< size_t >{};
    };

  public:
    /**
     * \brief Creates a karma tree starting with the passed in string.
     *
     * \param NPCStartText  - The text the NPC should start with when talked to.
     * \param BadThreshold  - The number at which bad karma starts at.
     * \param GoodThreshold - The number at which good karma starts at.
     */
    explicit KarmaTreeCreator( const std::string &NPCStartText, int BadThreshold = -50, int GoodThreshold = 50 ) noexcept;

  public:
    /**
     * \brief Add text to the conversation.
     *
     * \param PlayerText     - The text for the player.
     * \param NPCText        - The text for the npc.
     * \param ResultingKarma - The Karma returned resulting from chosing this option.
     * \param NeededKarma    - The karma at or above/below needed to see this text.  0 = Always see it.
     * \param NeededKeys     - The keys needed to see this text.  Karma is checked first!
     * \param NodeIndex      - Where to add the text.
     *
     * \return ID - The ID for the new text.
     */
    ID AddText( const std::string &PlayerText, const std::string &NPCText, const ID &NodeIndex = DefaultID, int ResultingKarma = 0, 
                int NeededKarma = 0, TempKeyHolder NeededKeys = TempKeyHolder{} ) noexcept;

    /**
     * \brief Add text to the conversation, but end it.
     *
     * \param PlayerText     - The text for the player.
     * \param ResultingKarma - The Karma returned resulting from chosing this option.
     * \param NeededKarma    - The karma at or above/below needed to see this text.  0 = Always see it.
     * \param NeededKeys     - The keys needed to see this text.  Karma is checked first!
     * \param OptionType     - How to end the conversation.
     * \param NodeIndex      - Where to add the text.
     *
     * \return void
     */
    void AddText( const std::string &PlayerText, int OptionType = EndConversation, const ID &NodeIndex = DefaultID,
                  int ResultingKarma = 0, int NeededKarma = 0, TempKeyHolder NeededKeys = TempKeyHolder{} ) noexcept;

  public:
    static const ID DefaultID;

  public:
    NPC_PLAYER_Pairs Head;

    const int BadThreshold;
    const int GoodThreshold;
};

namespace KarmaTreeSpace
{
  typedef KarmaTreeCreator::DefaultOptions                 DefaultResponseOptions;
  typedef KarmaTreeCreator::NPC_PLAYER_Pairs               TreeText;
}

typedef KarmaTreeCreator::NPC_PLAYER_Pairs::KarmaContainer KarmaContainer;
typedef KarmaTreeCreator::NPC_PLAYER_Pairs::KeyType        KeyType;

class KarmaTree
{
  public:
    class Responses
    {
      public:
        class ConstIter
        {
          public:
            explicit constexpr ConstIter( TreeText::ResponseContainer::const_iterator &&ConstIter ) noexcept;

          public:
            friend bool operator!=( const TreeText::ResponseContainer::const_iterator &Lhs, const ConstIter &Rhs ) noexcept;

          private:
            const TreeText::ResponseContainer::const_iterator IterIndex;
        };

        class Iter
        {
          public:
            typedef std::pair< std::string, int > ReturnType; //!< Player Text and the Resulting Karma

          public:
            explicit Iter( TreeText::ResponseContainer::const_iterator &&Iter ) noexcept;

          public:
            const ReturnType operator*() const noexcept;

            bool operator!=( const ConstIter &Rhs ) const noexcept;

            void operator++() noexcept;

          public: 
            const TreeText::ResponseType GetCurrentResponse() const noexcept;

          private:
            TreeText::ResponseContainer::const_iterator IterIndex;
        };

      public:
        explicit constexpr Responses( const TreeText::ResponseContainer &Response ) noexcept;

      public:
              Iter    begin() const noexcept;
        const ConstIter end() const noexcept;

      private:
        const TreeText::ResponseContainer &PlayerResponses;
    };

    struct InvalidOptionNumber
    {
      explicit InvalidOptionNumber( size_t Number, size_t Amount ) noexcept;

      const std::string What;
    };

  public:
    /**
     * \brief Creates a karma tree from the passed in creator.
     *
     * \param Move - The creator to move the tree from.
     */
    explicit KarmaTree( KarmaTreeCreator &&Move ) noexcept;

    KarmaTree( KarmaTree &&Move ) noexcept;

  public:
    /**
     * \brief Get what the NPC is currently saying.
     *
     * \return const std::string& - The string with the text for the npc.
     */
    const std::string & GetNPCText() const noexcept;

    /**
     * \brief All of the responses the player can make to the NPC.
     *
     * \return const Responses - All the possible responses you can see.
     */
    const Responses GetPlayerOptions( int Karma ) const noexcept;

    int GetGoodKarmaThreshold() const noexcept;
    int GetBadKarmaThreshold() const noexcept;
    
  public:
    void SetKarmaKey( int Karma, size_t Key ) noexcept;
    void RemoveKarmaKey( int Karma, size_t Key ) noexcept;
    void ClearKarmaKeys( int Karma ) noexcept;

  public:
    /**
     * \brief Tell the Tree the option that was chosen.
     *
     * \param Number - The index for the option.
     *
     * \exception InvalidOptionNumber - The number passed in was too large!
     *
     * \return int - If and how the conversation ended. See KarmaTreeCreator::DefaultOptions for defaults.
     */
    int ChooseOption( size_t Number ) const noexcept( false );

    /**
     * \brief Tell the Tree the option that was chosen.
     *
     * \param Number - The index for the option.
     *
     * \return int - If and how the conversation ended. See KarmaTreeCreator::DefaultOptions for defaults.
     */
    int ChooseOption_NoThrow( size_t Number ) const noexcept;

  private:
    const TreeText Head;

    mutable const TreeText *CurrentText = &Head;                          //!< Mutable since only the ptr changes, not the text.
    mutable TreeText::ResponseContainer CurrentOptions;                   //!< Mutable since no text change.
    mutable std::stack< TreeText::ResponseContainer > CurrentOptionStack; //!< Mutable since no text change.

    const int BadThreshold, GoodThreshold;

    std::set< size_t > BadKeys, NeutralKeys, GoodKeys;
};

namespace KarmaTreeSpace
{
  std::ostream & operator<<( std::ostream &Lhs, const std::string &Rhs ) noexcept;
}
