
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class PassMsgType : int;
}

namespace API {

    struct PassMsg {
std::string toString() const;
void fromString(const std::string &str);
        int64_t id;
        PassMsgType type = static_cast<PassMsgType>(0);
    };
}
