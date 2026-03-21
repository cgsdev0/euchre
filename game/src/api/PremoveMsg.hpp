
#pragma once

#include <optional>
#include <variant>

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
        int64_t card;
        PremoveMsgType type = static_cast<PremoveMsgType>(0);
    };
}
