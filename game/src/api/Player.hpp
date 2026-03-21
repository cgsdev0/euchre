
#pragma once

#include <optional>
#include <variant>

namespace API {

    struct Player {
std::string toString() const;
void fromString(const std::string &str);
        int64_t card_count;
        bool connected;
        std::optional<std::string> name;
        int64_t premoved;
        bool sitting_out;
        int64_t tricks;
    };
}
