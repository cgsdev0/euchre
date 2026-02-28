
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class OrderMsgType : int;
}

namespace API {

    struct OrderMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<int64_t> id;
        std::optional<int64_t> suit;
        OrderMsgType type = static_cast<OrderMsgType>(0);
    };
}
