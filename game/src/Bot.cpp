#include <assert.h>

#include <unordered_map>
#include <ranges>
#include <algorithm>

#include "api/API.hpp"
#include "Bot.hpp"
#include "Utils.hpp"

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

    size_t trump_count(std::vector<Card> hand, const Suit trump) {
        size_t count = 0;
        for (size_t i = 0; i < hand.size(); i++) {
            if (hand[i].suit == trump || isLeft(hand[i], trump)) {
                count++;
            }
        }
        return count;
    }

    static int score_card_order(Card card, Suit trump) {
        if (color(card.suit) == color(trump)) {
            if (card.rank == Rank::JACK) {
                return card.suit == trump ? 16 : 15;
            } else {
                return (int)card.rank + 9; // in this system, '0' is the card value for '9'
            }
        } else {
            return (int)card.rank;
        }
    }

    static int score_card(Card card, Suit trump, Suit led) {
        return scoreCard(card, trump, led);
    }

    static int score_hand_for_suit(std::vector<Card> hand, Suit trump) {
        int score = 0;
        for (size_t i = 0; i < hand.size(); i++) {
            score += score_card_order(hand[i], trump);
        }
        return score;
    }

    bool orderTrump(BotDecisionState &state, std::optional<Suit> &suit, bool &go_alone) {
        assert(state.hand.size() == 5);
        assert(state.top_card.has_value());
        go_alone = false;
        suit = std::nullopt;

        if (state.phase == Phase::VOTE_ROUND1) {
            int score = score_hand_for_suit(state.hand, (*state.top_card).suit);
            if (score > 20) {
                if (score > 35) {
                    go_alone = true;
                }
                return true;
            }
        } else if (state.phase == Phase::VOTE_ROUND2) {
            for (int i = 0; i < 4; i++) {
                if (i == (int)(*state.top_card).suit)
                    continue;

                int score = score_hand_for_suit(state.hand, (*state.top_card).suit);
                if (score > 20) {
                    suit = (*state.top_card).suit;
                    if (score > 35) {
                        go_alone = true;
                    }
                    return true;
                }
            }
        }

        return false;
    }

    Card discard(BotDecisionState state) {
        // first, check if we have a single off-suit card that we can discard to consolidate

        std::unordered_map<Suit, int> counts;

        for (const auto &c : state.hand) {
            counts[c.suit]++;
        }

        auto it = std::ranges::find_if(state.hand, [&](const Card &c) {
            return counts[c.suit] == 1 && c.suit != state.trump;
        });

        if (it != state.hand.end()) return *it;

        // second, attempt to discard a low off-suit (the lowest off-suit)

        auto non_trump = state.hand | std::views::filter([&](const Card &c) {
            return c.suit != state.trump;
        });

        auto non_trump_it = std::ranges::min_element(non_trump, {}, [](const Card &c) {
            return static_cast<int>(c.rank);
        });

        if (non_trump_it != std::ranges::end(non_trump)) return *non_trump_it;

        // last, we just discard the lowest value card, if SOMEHOW we didn't get rid of a card with this

        auto greedy_lowest_card = std::ranges::min_element(state.hand, {}, [](const Card &c) {
            return static_cast<int>(c.rank);
        });

        assert(greedy_lowest_card != state.hand.end());

        return *greedy_lowest_card;
    }

    static std::vector<Card> sort_hands_by_value(std::vector<Card> cards, Suit trump, std::optional<Suit> led) {
        auto copy = cards;
        std::sort(copy.begin(), copy.end(), [&](Card a, Card b){
            if (led == std::nullopt) { // we're leading
                return score_card_order(a, trump) > score_card_order(b, trump);
            } else {
                return score_card(a, trump, *led) > score_card(b, trump, *led);
            }
        });
        return copy;
    }

    static bool played_by_partner(BotDecisionState &state, TaggedCard &c) {
        return state.id % 2 == c.id % 2;
    }

    static bool is_team_winning_hand(BotDecisionState &state) {
        auto it = std::ranges::max_element(state.stack, {}, [&](const TaggedCard &c) {
            return score_card(c.card, state.trump, state.stack[0].card.suit);
        });
        return played_by_partner(state, *it);
    }

    Card playCard(BotDecisionState state) {
        if (state.stack.size() == 0) { // leading (BEST CARD!??!)
            auto sorted_cards = sort_hands_by_value(state.hand, state.trump, std::nullopt);
            return sorted_cards[0];
        } else {
            auto sorted_cards = sort_hands_by_value(state.hand, state.trump, state.stack[0].card.suit);

            TaggedCard led = state.stack[0];
            Suit led_suit = led.card.suit;

            auto legal_view = state.hand | std::views::filter([&](const Card &c) {
                return c.suit == led.card.suit;
            });
            std::vector<Card> legal_cards_vec(legal_view.begin(), legal_view.end());

            if (!legal_cards_vec.empty()) {
                auto lowest_legal_card = std::ranges::min_element(legal_cards_vec, {}, [&](const Card &c) {
                    return score_card(c, state.trump, led_suit);
                });
                assert(lowest_legal_card != legal_cards_vec.end());

                if (is_team_winning_hand(state)) { // toss lowest card - partner for 1
                    return *lowest_legal_card;
                } else {
                    // TODO make a smarter decision about _which_ card to play, for now, we just
                    // play the highest legal card, but maybe in the future, we want to play like,
                    // the one-higher card if there is one? Maybe this is nonsense.

                    auto highest_legal_card = std::ranges::max_element(legal_cards_vec, {}, [&](const Card &c) {
                        return score_card(c, state.trump, led_suit);
                    });
                    assert(highest_legal_card != legal_cards_vec.end());

                    bool will_win = std::all_of(state.stack.begin(), state.stack.end(), [&](const TaggedCard &tc) {
                        return score_card(*highest_legal_card, state.trump, led_suit) > score_card(tc.card, state.trump, led_suit);
                    });

                    if (will_win) {
                        return *highest_legal_card;
                    } else {
                        return *lowest_legal_card;
                    }
                }
            } else {
                // TODO just like the inner situation, if we _can_ win, we play the winning card,
                // otherwise, we just dump a low card.

                auto lowest_card = std::ranges::min_element(state.hand, {}, [&](const Card &c) {
                    return score_card(c, state.trump, led_suit);
                });
                assert(lowest_card != state.hand.end());

                auto highest_card = std::ranges::max_element(state.hand, {}, [&](const Card &c) {
                    return score_card(c, state.trump, led_suit);
                });
                assert(highest_card != state.hand.end());

                bool will_win = std::all_of(state.stack.begin(), state.stack.end(), [&](const TaggedCard &tc) {
                    return score_card(*highest_card, state.trump, led_suit) > score_card(tc.card, state.trump, led_suit);
                });

                if (will_win) {
                    return *highest_card;
                } else {
                    return *lowest_card;
                }
            }
        }
    }
}
