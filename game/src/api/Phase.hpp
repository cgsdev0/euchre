#include <cstdint>

#pragma once

#include <optional>
#include <variant>

namespace API {

    enum class Phase : int { DISCARDING, ENDED, LOBBY, PLAYING, VOTE_ROUND1, VOTE_ROUND2 };
}
