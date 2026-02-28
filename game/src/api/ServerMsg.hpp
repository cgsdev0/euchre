
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"
#include "Msg.hpp"
#include "RichTextChunk.hpp"

namespace API {
    enum class Phase : int;
    struct Player;
    struct RichTextMsg;
    enum class Trump : int;
    enum class ServerMsgType : int;
}

namespace API {
    /**
     * Update everything
     */


    /**
     * Update everything
     */
    struct ServerMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<int64_t> dealer;
        std::optional<Phase> phase;
        std::optional<std::vector<Card>> played_cards;
        std::optional<std::vector<Player>> players;
        std::optional<bool> private_session;
        std::optional<std::vector<RichTextMsg>> rich_chat_log;
        std::optional<std::vector<int64_t>> scores;
        std::optional<Card> top_card;
        std::optional<std::vector<int64_t>> trick;
        std::optional<Trump> trump;
        std::optional<int64_t> turn;
        std::optional<ServerMsgType> type;
        std::optional<std::vector<Card>> your_cards;
        std::optional<int64_t> your_id;
        std::optional<int64_t> id;
        std::optional<int64_t> suit;
        std::optional<int64_t> table_talk;
        std::optional<Card> card;
        std::optional<std::string> name;
        std::optional<std::vector<Msg>> msg;
    };
}
