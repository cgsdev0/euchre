#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    struct Card;
}

namespace API {

    struct ServerPlayer {
std::string toString() const;
void fromString(const std::string &str);
        int64_t bot_enum;
        std::vector<Card> cards;
        bool connected;
        std::optional<std::string> name;
        std::vector<Card> premoves;
        std::string session;
        bool sitting_out;
        int64_t tricks;
    };
}
