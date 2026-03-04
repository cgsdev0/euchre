#ifndef BOT_H
#define BOT_H

#include <string>
#include <vector>

#include "api/Card.hpp"
#include "api/GameState.hpp"

using namespace API;

namespace Bot {
    enum BotName : int {
        BOT_NONE, // regular players are BOT_NONE
        BOT_BETHANY,
        BOT_BILL,
        BOT_BRADLEY,
        BOT_EUGINE,
        BOT_FRANK,
        BOT_GEORGINA,
        BOT_MERIDITH,
        BOT_SHERRY,
        BOT_PERFECT,
        BOT_TOTAL
    };

    // NOTE These weights are from 0.0 (not likely at all) to 1.0 (likely to do something). The bot
    // will attempt to do these things (go alone, order trump, cheat, etc.) if it's a good idea, and
    // the weight permits the bot to do the thing.
    enum BotWeight : int {
        BOT_WEIGHT_ALONE,        // chances of going alone
        BOT_WEIGHT_ORDERTRUMP,   // chances of ordering up trump
        BOT_WEIGHT_CHEAT,        // chances of cheating
        BOT_WEIGHT_TRUST,        // chances of the bot trusting their partner for 1
        BOT_WEIGHT_MEMORY,       // chances the bot correctly remembers played cards
        BOT_WEIGHT_BANTERLOSS,   // chances of bantering on a loss
        BOT_WEIGHT_BANTERWIN,    // chances of bantering on a win
        BOT_WEIGHT_TOTAL
    };

    struct BotDecisionState {
        BotName name;
        Phase phase;
        Suit trump;
        std::vector<API::Card> hand;        // bot's hand
        std::vector<API::Card> stack;       // stack of cards played
        std::optional<API::Card> top_card;  // top card (for ordering up trump)
    };
}

namespace Bot {
    bool goAlone(BotDecisionState &state);
    bool pickItUp(BotDecisionState &state);
}

#endif // BOT_H
