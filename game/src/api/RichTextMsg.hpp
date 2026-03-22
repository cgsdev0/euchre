#include <cstdint>

#pragma once

#include <optional>
#include <variant>

#include "Msg.hpp"
#include "RichTextChunk.hpp"

namespace API {
    enum class RichTextMsgType : int;
}

namespace API {

    struct RichTextMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::vector<Msg> msg;
        RichTextMsgType type = static_cast<RichTextMsgType>(0);
    };
}
