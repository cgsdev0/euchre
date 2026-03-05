
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"
#include "Msg.hpp"
#include "RichTextChunk.hpp"

namespace API {
    enum class ServerMsgType : int;
    enum class Phase : int;
    struct Player;
    struct RichTextMsg;
    enum class Suit : int;
}

namespace API {
    /**
     * Dealer discarded something.
     *
     * Pass during the voting rounds.
     *
     * Order up a particular suit as trump.
     *
     * Use the quick-chat feature.
     *
     * Choose a card to play for the current trick.
     *
     * Self-explanatory.
     *
     * Change your display name.
     *
     * Update everything
     */


    /**
     * Dealer discarded something.
     *
     * Pass during the voting rounds.
     *
     * Order up a particular suit as trump.
     *
     * Use the quick-chat feature.
     *
     * Choose a card to play for the current trick.
     *
     * Self-explanatory.
     *
     * Change your display name.
     *
     * Update everything
     */
    struct ServerMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<std::string> error;
        ServerMsgType type = static_cast<ServerMsgType>(0);
        std::optional<int64_t> id;
        std::optional<std::string> room;
        std::optional<int64_t> dealer;
        std::optional<Phase> phase;
        std::optional<std::vector<Card>> played_cards;
        std::optional<std::vector<Player>> players;
        std::optional<bool> private_session;
        std::optional<std::vector<RichTextMsg>> rich_chat_log;
        std::optional<std::vector<int64_t>> scores;
        std::optional<Card> top_card;
        std::optional<std::vector<Card>> trick;
        std::optional<Suit> trump;
        std::optional<int64_t> turn;
        std::optional<std::vector<Card>> your_cards;
        std::optional<bool> alone;
        std::optional<Suit> suit;
        std::optional<int64_t> table_talk;
        std::optional<Card> card;
        std::optional<std::string> name;
        std::optional<std::vector<Msg>> msg;
    };
}
