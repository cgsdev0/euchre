
#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class ServerMsgType : int { CHAT, DEAL, DISCONNECT, JOIN, RECONNECT, RESTART, UPDATE, UPDATE_NAME, WELCOME };
}
