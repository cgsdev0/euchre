#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class Suit : int;
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
        std::optional<bool> alone;
        std::optional<int64_t> id;
        std::optional<Suit> suit;
        OrderMsgType type = static_cast<OrderMsgType>(0);
    };
}
