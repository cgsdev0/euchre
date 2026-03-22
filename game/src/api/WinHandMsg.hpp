#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class WinHandMsgType : int;
}

namespace API {
    /**
     * A team won a hand!
     */


    /**
     * A team won a hand!
     */
    struct WinHandMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        WinHandMsgType type = static_cast<WinHandMsgType>(0);
    };
}
