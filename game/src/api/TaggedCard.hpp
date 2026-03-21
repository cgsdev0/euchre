
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {

    struct TaggedCard {
std::string toString() const;
void fromString(const std::string &str);
        Card card;
        int64_t id;
    };
}
