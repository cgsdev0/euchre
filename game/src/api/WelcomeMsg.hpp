#include <cstdint>

#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class Phase : int;
    struct TaggedCard;
    struct Player;
    struct RichTextMsg;
    enum class Suit : int;
    enum class WelcomeMsgType : int;
}

namespace API {

    struct WelcomeMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t dealer;
        int64_t id;
        Phase phase;
        std::vector<TaggedCard> played_cards;
        std::vector<Player> players;
        bool private_session;
        std::vector<RichTextMsg> rich_chat_log;
        std::vector<int64_t> scores;
        std::optional<Card> top_card;
        std::vector<TaggedCard> trick;
        Suit trump;
        int64_t turn;
        WelcomeMsgType type = static_cast<WelcomeMsgType>(0);
        std::vector<Card> your_cards;
        std::vector<Card> your_premoves;
    };
}
