
#pragma once

#include <optional>
#include <variant>

namespace API {
    struct Card;
    enum class LastCardMsgType : int;
}

namespace API {
    /**
     * Auto-play the last card of the hand.
     */


    /**
     * Auto-play the last card of the hand.
     */
    struct LastCardMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::vector<Card> cards;
        LastCardMsgType type = static_cast<LastCardMsgType>(0);
    };
}
