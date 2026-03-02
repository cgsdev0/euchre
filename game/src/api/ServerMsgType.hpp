
#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class ServerMsgType : int { CHAT, DEAL, DISCARDED, DISCONNECT, ERROR, JOIN, ORDER, PASS, PLAY_CARD, PLAY_JAJA_DING_DONG, RECONNECT, REDIRECT, TABLE_TALK, UPDATE, UPDATE_NAME, WELCOME };
}
