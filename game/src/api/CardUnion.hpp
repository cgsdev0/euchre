
#pragma once

#include <optional>
#include <variant>

namespace API {
    struct Card;
}

namespace API {

    using CardUnion = std::variant<Card, int64_t>;
}
