
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class DiscardMsgType : int;
}

namespace API {
    /**
     * Dealer chooses a card to discard.
     */


    /**
     * Dealer chooses a card to discard.
     */
    struct DiscardMsg {
std::string toString() const;
void fromString(const std::string &str);
        Card card;
        std::optional<int64_t> id;
        DiscardMsgType type = static_cast<DiscardMsgType>(0);
    };
}
