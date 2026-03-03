
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
        std::vector<Card> cards;
        bool connected;
        std::optional<std::string> name;
        std::vector<Card> premoves;
        std::string session;
        bool sitting_out;
        bool is_bot;
        int64_t tricks;
    };
}
