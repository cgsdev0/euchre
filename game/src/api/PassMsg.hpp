#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class PassMsgType : int;
}

namespace API {
    /**
     * Pass during the voting rounds.
     */


    /**
     * Pass during the voting rounds.
     */
    struct PassMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<int64_t> id;
        PassMsgType type = static_cast<PassMsgType>(0);
    };
}
