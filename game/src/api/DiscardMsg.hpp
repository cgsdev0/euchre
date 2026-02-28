
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class DiscardMsgType : int;
}

namespace API {

    struct DiscardMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t card;
        DiscardMsgType type = static_cast<DiscardMsgType>(0);
    };
}
