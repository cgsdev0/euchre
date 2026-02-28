
#pragma once

#include <optional>
#include <variant>

namespace API {
    struct Player;
    struct RichTextMsg;
    enum class WelcomeMsgType : int;
}

namespace API {

    struct WelcomeMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t dealer;
        int64_t phase;
        std::vector<int64_t> played_cards;
        std::vector<Player> players;
        bool private_session;
        std::vector<RichTextMsg> rich_chat_log;
        std::vector<int64_t> scores;
        std::optional<int64_t> top_card;
        std::vector<int64_t> trick;
        int64_t turn;
        WelcomeMsgType type = static_cast<WelcomeMsgType>(0);
        std::vector<int64_t> your_cards;
        int64_t your_id;
    };
}
