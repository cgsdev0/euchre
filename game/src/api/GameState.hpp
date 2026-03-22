#include <cstdint>

#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class Phase : int;
    struct TaggedCard;
    struct ServerPlayer;
    struct RichTextMsg;
    enum class Suit : int;
    enum class GameStateType : int;
}

namespace API {

    struct GameState {
std::string toString() const;
void fromString(const std::string &str);
        int64_t caller;
        int64_t dealer;
        std::optional<std::vector<Card>> kitty;
        Phase phase;
        std::vector<TaggedCard> played_cards;
        std::vector<ServerPlayer> players;
        bool private_session;
        std::vector<RichTextMsg> rich_chat_log;
        std::vector<int64_t> scores;
        std::optional<Card> top_card;
        std::vector<TaggedCard> trick;
        int64_t trick_leader;
        Suit trump;
        int64_t turn;
        GameStateType type = static_cast<GameStateType>(0);
    };
}
