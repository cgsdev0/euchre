
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class Rank : int;
    enum class Suit : int;
}

namespace API {

    struct Card {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<Rank> rank;
        std::optional<Suit> suit;
    };
}
