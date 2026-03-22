#include <cstdint>

#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class Rank : int;
    enum class Suit : int;
}

namespace API {

    struct CardElement {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<Card> card;
        std::optional<int64_t> id;
        std::optional<bool> illegal;
        std::optional<Rank> rank;
        std::optional<Suit> suit;
    };
}
