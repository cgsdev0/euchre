
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class DealCardsMsgType : int;
}

namespace API {

    struct DealCardsMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t top_card;
        std::optional<DealCardsMsgType> type;
        std::vector<int64_t> your_cards;
    };
}
