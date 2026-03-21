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

    static int score_card(Card card, Suit trump) {
        if (color(card.suit) == color(trump)) {
            if (card.rank == Rank::JACK) {
                return card.suit == trump ? 19 : 18;
            } else {
            }
        } else {
            return (int)card.rank;
        }
    }

    static int score_hand_for_suit(std::vector<Card> hand, Suit trump) {
    }

    bool orderTrump(BotDecisionState &state, std::optional<Suit> &suit, bool &go_alone) {
        assert(state.hand.size() == 5);
        assert(state.top_card.has_value());
        go_alone = false;
        suit = std::nullopt;

        if (state.phase == Phase::VOTE_ROUND1) {
            int score = score_for_suit(state.hand, (*state.top_card).suit);
        } else if (state.phase == Phase::VOTE_ROUND2) {
            int score = score_for_suit(state.hand, (*state.top_card).suit);
        }

        return false;
    }

    Card discard(BotDecisionState state) {
        Card card = state.hand[0];
        state.hand.erase(state.hand.begin());
        return card;
    }

    static std::vector<Card> sort_hands_by_value(std::vector<Card> cards, Suit trump) {
        auto copy = cards;
        std::sort(copy.begin(), copy.end(), [&](Card a, Card b){
            return score_card(a, trump) < score_card(b, trump);
        });
        return cards;
    }

    bool has_suit(std::vector<Card> &cards, Suit check) {
    }

    Card playCard(BotDecisionState state) {
        auto sorted_cards = sort_hands_by_value(state.hand, state.trump);
        if (state.stack.size() == 0) { // leading
            return sorted_cards[0];
        } else {
            Card led = state.stack[0];
            if (has_suit(sorted_cards, led.suit)) {
            }
        }
    }
}
