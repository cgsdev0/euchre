
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class Suit : int;
}

namespace API {

    struct ChatMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::string msg;
        Suit type;
    };
}
