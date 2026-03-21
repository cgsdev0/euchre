
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class PlayerUpdateMsgType : int;
}

namespace API {
    /**
     * Update an individual player's state.
     */


    /**
     * Update an individual player's state.
     */
    struct PlayerUpdateMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        std::optional<std::string> name;
        std::optional<bool> premoved;
        PlayerUpdateMsgType type = static_cast<PlayerUpdateMsgType>(0);
    };
}
