#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class ServerMsgType : int { CHAT, DEAL, DISCARDED, DISCONNECT, ERROR, JOIN, LAST_CARD, LAY_DOWN, ORDER, PASS, PLAY_CARD, PLAY_JAJA_DING_DONG, RECONNECT, REDIRECT, TABLE_TALK, UPDATE, UPDATE_NAME, WELCOME };
}
