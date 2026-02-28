
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class UpdateMsgType : int;
}

namespace API {
    /**
     * Update everything
     */


    /**
     * Update everything
     */
    struct UpdateMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t dealer;
        int64_t phase;
        std::vector<int64_t> scores;
        int64_t trump;
        int64_t turn;
        UpdateMsgType type = static_cast<UpdateMsgType>(0);
    };
}
