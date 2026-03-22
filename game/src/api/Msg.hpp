#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {
    struct RichTextChunk;
}

namespace API {

    using Msg = std::variant<RichTextChunk, std::string>;
}
