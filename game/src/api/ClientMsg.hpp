
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class ClientMsgType : int;
}

namespace API {

    struct ClientMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<std::string> msg;
        ClientMsgType type = static_cast<ClientMsgType>(0);
        std::optional<Card> card;
        std::optional<int64_t> id;
        std::optional<int64_t> suit;
        std::optional<int64_t> table_talk;
        std::optional<std::string> name;
    };
}
