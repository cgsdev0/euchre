
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class OrderMsgType : int;
}

namespace API {
    /**
     * Order up a particular suit as trump.
     */


    /**
     * Order up a particular suit as trump.
     */
    struct OrderMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        std::optional<int64_t> suit;
        OrderMsgType type = static_cast<OrderMsgType>(0);
    };
}
