
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class UndoPremoveMsgType : int;
}

namespace API {
    /**
     * Undo a premove action.
     */


    /**
     * Undo a premove action.
     */
    struct UndoPremoveMsg {
std::string toString() const;
void fromString(const std::string &str);
        UndoPremoveMsgType type = static_cast<UndoPremoveMsgType>(0);
    };
}
