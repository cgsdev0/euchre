#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class WinTrickMsgType : int;
}

namespace API {
    /**
     * A player won a trick!
     */


    /**
     * A player won a trick!
     */
    struct WinTrickMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        WinTrickMsgType type = static_cast<WinTrickMsgType>(0);
    };
}
