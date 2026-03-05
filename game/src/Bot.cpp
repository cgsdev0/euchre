#include <assert.h>

#include "Bot.hpp"
#include "api/Phase.hpp"
#include "api/Suit.hpp"
#include "api/Rank.hpp"

using namespace Bot;
using namespace API;

float BOT_WEIGHTS[BotName::BOT_TOTAL][BotWeight::BOT_WEIGHT_TOTAL] = {
    { // BOT_BETHANY
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_BILL
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_BRADLEY
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_EUGINE
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_FRANK
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_GEORGINA
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_MERIDITH
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_SHERRY
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
    },
    { // BOT_PERFECT
        1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5
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

    // NOTE Each of these functions create a ranked choice given the cards in the hand.

    static int color(const Suit suit) {
        return suit == Suit::DIAMONDS || suit == Suit::HEARTS ? 0 : 1;
    }

    bool orderTrump(BotDecisionState &state, std::optional<Suit> &suit, bool &go_alone) {
        assert(state.hand.size() == 5);
        assert(state.top_card.has_value());
        go_alone = false;
        suit = std::nullopt;

        if (state.phase == Phase::VOTE_ROUND1) {
            // Simple heuristic to start, if we have three trump, or two trump and a bower, we call
            // the suit.

            int trump_count = 0;
            int bower_count = 0;
            int off_aces = 0;

            for (size_t i = 0; i < state.hand.size(); i++) {
                Card card = state.hand[i];

                bool is_trump_color = color((*state.top_card).suit) == color(card.suit);

                bool is_bower = card.rank == Rank::JACK && is_trump_color;
                if (is_bower) {
                    bower_count++;
                }

                if (is_trump_color && card.rank >= Rank::JACK) {
                    trump_count++;
                }

                if (!is_trump_color && card.rank == Rank::ACE) {
                    off_aces++;
                }
            }

            bool order_up =
                (trump_count >= 3) ||
                (trump_count >= 2 && bower_count >= 1) ||
                (trump_count >= 2 && off_aces >= 1);

            if (order_up) {
                suit = (*state.top_card).suit;
                if (bower_count == 2)
                    go_alone = true;
            }

            return order_up;
        } else if (state.phase == Phase::VOTE_ROUND2) {
        } else {
        }

        return false;
    }

    Card playCard(BotDecisionState state) {
        // just play the first card in the hand
        Card card = state.hand[0];
        state.hand.erase(state.hand.begin());
        return card;
    }

    Card discard(BotDecisionState state) {
        Card card = state.hand[0];
        state.hand.erase(state.hand.begin());
        return card;
    }
}
