#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class WinGameMsgType : int;
}

namespace API {
    /**
     * A team won the game!
     */


    /**
     * A team won the game!
     */
    struct WinGameMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        WinGameMsgType type = static_cast<WinGameMsgType>(0);
    };
}
