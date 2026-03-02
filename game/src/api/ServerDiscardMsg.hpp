
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class ServerDiscardMsgType : int;
}

namespace API {
    /**
     * Dealer discarded something.
     */


    /**
     * Dealer discarded something.
     */
    struct ServerDiscardMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        ServerDiscardMsgType type = static_cast<ServerDiscardMsgType>(0);
    };
}
