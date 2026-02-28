
#pragma once

#include <optional>
#include <variant>

namespace API {

    struct ServerPlayer {
std::string toString() const;
void fromString(const std::string &str);
        std::vector<int64_t> cards;
        bool connected;
        std::optional<std::string> name;
        std::string session;
    };
}
