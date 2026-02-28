
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class PlayCardMsgType : int;
}

namespace API {

    struct PlayCardMsg {
std::string toString() const;
void fromString(const std::string &str);
        Card card;
        std::optional<int64_t> id;
        PlayCardMsgType type = static_cast<PlayCardMsgType>(0);
    };
}
