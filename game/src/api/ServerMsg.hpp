
#pragma once

#include <optional>
#include <variant>

#include "Msg.hpp"
#include "RichTextChunk.hpp"

namespace API {
    struct Player;
    struct RichTextMsg;
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
        std::optional<int64_t> phase;
        std::optional<std::vector<int64_t>> played_cards;
        std::optional<std::vector<Player>> players;
        std::optional<bool> private_session;
        std::optional<std::vector<RichTextMsg>> rich_chat_log;
        std::optional<std::vector<int64_t>> scores;
        std::optional<int64_t> top_card;
        std::optional<std::vector<int64_t>> trick;
        std::optional<int64_t> turn;
        std::optional<ServerMsgType> type;
        std::optional<std::vector<int64_t>> your_cards;
        std::optional<int64_t> your_id;
        std::optional<int64_t> id;
        std::optional<int64_t> suit;
        std::optional<int64_t> table_talk;
        std::optional<int64_t> card;
        std::optional<std::string> name;
        std::optional<std::vector<Msg>> msg;
        std::optional<int64_t> trump;
    };
}
