#include "Bot.hpp"

using namespace Bot;

float BOT_WEIGHTS[BotName::BOT_TOTAL][BotWeight::BOT_WEIGHT_TOTAL] = {
    { // BOT_BETHANY
    },
    { // BOT_BILL
        1.0, 1.0, 0.0, 1.0, 1.0, 
    },
    { // BOT_BRADLEY
    },
    { // BOT_EUGINE
    },
    { // BOT_FRANK
    },
    { // BOT_GEORGINA
    },
    { // BOT_MERIDITH
    },
    { // BOT_SHERRY
    },
    { // BOT_PERFECT
    }
};

std::string BOT_NAMES[BotName::BOT_TOTAL] = {
    "NONE",
    "Bethany",
    "Bill",
    "Bradley",
    "Eugine",
    "Frank",
    "Georgina",
    "Meridith",
    "Sherry",
    "PERFECT"
};

namespace Bot {
    bool goAlone(BotDecisionState state) {
        return true;
    }
}
