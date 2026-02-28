
#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class ClientMsgType : int { DISCARD, ID, ORDER, PASS, PLAY_CARD, PLAY_JAJA_DING_DONG, RESTART, TABLE_TALK, UPDATE_NAME };
}
