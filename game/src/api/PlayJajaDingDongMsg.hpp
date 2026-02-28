
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class PlayJajaDingDongMsgType : int;
}

namespace API {

    struct PlayJajaDingDongMsg {
std::string toString() const;
void fromString(const std::string &str);
        PlayJajaDingDongMsgType type = static_cast<PlayJajaDingDongMsgType>(0);
    };
}
