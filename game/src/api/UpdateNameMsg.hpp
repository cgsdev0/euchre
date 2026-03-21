
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class UpdateNameMsgType : int;
}

namespace API {
    /**
     * Change your display name.
     */


    /**
     * Change your display name.
     */
    struct UpdateNameMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::string name;
        UpdateNameMsgType type = static_cast<UpdateNameMsgType>(0);
    };
}
