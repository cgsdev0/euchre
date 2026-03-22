#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    struct Card;
    enum class LayDownMsgType : int;
}

namespace API {
    /**
     * End the hand with your ridiculous cards.
     */


    /**
     * End the hand with your ridiculous cards.
     */
    struct LayDownMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<std::vector<Card>> cards;
        std::optional<int64_t> id;
        LayDownMsgType type = static_cast<LayDownMsgType>(0);
    };
}
