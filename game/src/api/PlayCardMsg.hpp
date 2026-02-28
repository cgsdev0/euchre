
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class PlayCardMsgType : int;
}

namespace API {

    struct PlayCardMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t card;
        std::optional<int64_t> id;
        PlayCardMsgType type = static_cast<PlayCardMsgType>(0);
    };
}
