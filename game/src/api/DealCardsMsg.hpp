
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class DealCardsMsgType : int;
}

namespace API {

    struct DealCardsMsg {
std::string toString() const;
void fromString(const std::string &str);
        Card top_card;
        std::optional<DealCardsMsgType> type;
        std::vector<Card> your_cards;
    };
}
