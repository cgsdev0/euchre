
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class DiscardMsgType : int;
}

namespace API {

    struct DiscardMsg {
std::string toString() const;
void fromString(const std::string &str);
        Card card;
        DiscardMsgType type = static_cast<DiscardMsgType>(0);
    };
}
