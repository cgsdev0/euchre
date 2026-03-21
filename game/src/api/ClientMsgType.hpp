
#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class ClientMsgType : int { COOKIE, DISCARD, ORDER, PASS, PLAY_CARD, PLAY_JAJA_DING_DONG, PREMOVE, RESTART, TABLE_TALK, UNDO_PREMOVE, UPDATE_PLAYER };
}
