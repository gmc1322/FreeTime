#pragma once

#include <string>
#include <vector>

#include "Triple.h"

struct ConversationTreeCreator
{
  public:
    enum DefaultOptions
    {
      ContinueConversation = -1, // The conversation shall continue!
      EndConversation,           // The conversation has ended un-event-fully.

      CustomOption               // The conversation has ended in a special event! (I.E. opening up the shop)
    };

  public:
    struct NPC_PLAYER_Pairs
    {
       explicit NPC_PLAYER_Pairs( const std::string &NPCText ) noexcept;

       NPC_PLAYER_Pairs( const NPC_PLAYER_Pairs &Copy ) noexcept;
       NPC_PLAYER_Pairs( NPC_PLAYER_Pairs &&Move ) noexcept;

      const std::string NPC; // NPC Text

      std::vector< Triple< int, std::string, NPC_PLAYER_Pairs > > PlayerResponses; // Player Text
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
    * \brief Creates a conversation tree starting with the passed in string.
    *
    * \param NPCStartText - The text the NPC should start with when talked to.
    */
     explicit ConversationTreeCreator( const std::string &NPCStartText ) noexcept;

  public:
    /**
     * \brief Add text to the conversation.
     *
     *
     * \param PlayerText - The text for the player.
     * \param NPCText    - The text for the npc.
     * \param NodeIndex  - Where to add the text.
     *
     * \return ID - The ID for the new text.
     */
     ID AddText( const std::string &PlayerText, const std::string &NPCText, const ID &NodeIndex = ID{} ) noexcept;

    /**
     * \brief Add text to the conversation, but end it.
     *
     * \param PlayerText - The text for the player.
     * \param OptionType - How to end the conversation.
     * \param NodeIndex  - Where to add the text.
     *
     * \return void
     */
     void AddText( const std::string &PlayerText, int OptionType = EndConversation,
                                       const ID &NodeIndex = ID{} ) noexcept;

  public:
    NPC_PLAYER_Pairs Head;
};

namespace ConversationTreeSpace
{
  typedef ConversationTreeCreator::DefaultOptions DefaultResponseOptions;
  typedef ConversationTreeCreator::NPC_PLAYER_Pairs TreeText;
}

class ConversationTree
{
  public:
    class Responses
    {
      public:
        typedef std::vector< Triple< int, std::string, TreeText > > ResponseType;

      public:
        class ConstIter
        {
          public:
             explicit constexpr ConstIter( ResponseType::const_iterator &&ConstIter ) noexcept;

          public:
             friend bool operator!=( const ResponseType::const_iterator &Lhs, const ConstIter &Rhs ) noexcept;

          private:
            const ResponseType::const_iterator IterIndex;
        };

        class Iter
        {
          public:
             explicit Iter( ResponseType::const_iterator &&Iter ) noexcept;

          public:
             const std::string & operator*() const noexcept;

             bool operator!=( const ConstIter &Rhs ) const noexcept;

             void operator++() noexcept;

          private:
            ResponseType::const_iterator IterIndex;
        };

      public:
         explicit constexpr Responses( const ResponseType &Response ) noexcept;

      public:
         Iter begin() const noexcept;
         const ConstIter end() const noexcept;

         size_t size() const noexcept;

      private:
        const ResponseType &PlayerResponses;
    };

    struct InvalidOptionNumber
    {
       explicit InvalidOptionNumber( size_t Number, size_t Amount ) noexcept;

      const std::string What;
    };

  public:
    /**
     * \brief Creates a conversation tree from the passed in creator.
     *
     * \param Move - The creator to move the tree from.
     */
     explicit ConversationTree( ConversationTreeCreator &&Move ) noexcept;

     ConversationTree( ConversationTree &&Move ) noexcept;

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
     * \return const std::vector<const std::string>& - A vector with all of the text for the player.
     */
     Responses GetPlayerOptions() const noexcept;

  public:
    /**
     * \brief Tell the Tree the option that was chosen.
     *
     * \param Number - The index for the option, starting from 0.
     * 
     * \exception InvalidOptionNumber - The number passed in was too large!
     *
     * \return int - If and how the conversation ended. See ConversationCreator::DefaultOptions for defaults.
     */
     int ChooseOption( size_t Number ) const noexcept( false );

    /**
     * \brief Tell the Tree the option that was chosen.
     *
     * \param Number - The index for the option, starting from 0.
     *
     * \return int - If and how the conversation ended. See ConversationCreator::DefaultOptions for defaults.
     */
     int ChooseOption_NoThrow( size_t Number ) const noexcept;

  private:
    const TreeText Head;
    mutable const TreeText *CurrentText = &Head; //!< Make this mutable since the text can never change, just the ptr to it.
};

namespace ConversationTreeSpace
{
   std::ostream & operator<<( std::ostream &Lhs, const std::string &Rhs ) noexcept;
}