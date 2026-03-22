#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class RestartMsgType : int;
}

namespace API {
    /**
     * Start a new game.
     */


    /**
     * Start a new game.
     */
    struct RestartMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<int64_t> id;
        RestartMsgType type = static_cast<RestartMsgType>(0);
    };
}
