#include <cstdint>

#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class PremoveMsgType : int;
}

namespace API {
    /**
     * Select a card to pre-move.
     */


    /**
     * Select a card to pre-move.
     */
    struct PremoveMsg {
std::string toString() const;
void fromString(const std::string &str);
        Card card;
        PremoveMsgType type = static_cast<PremoveMsgType>(0);
    };
}
