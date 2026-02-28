
#pragma once

#include <optional>
#include <variant>

namespace API {
    enum class TableTalkMsgType : int;
}

namespace API {

    struct TableTalkMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<int64_t> id;
        int64_t table_talk;
        TableTalkMsgType type = static_cast<TableTalkMsgType>(0);
    };
}
