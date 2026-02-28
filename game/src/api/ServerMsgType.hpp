
#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class ServerMsgType : int { CHAT, DEAL, DISCONNECT, ID, JOIN, ORDER, PASS, PLAY_CARD, PLAY_JAJA_DING_DONG, RECONNECT, TABLE_TALK, UPDATE, UPDATE_NAME, WELCOME };
}
