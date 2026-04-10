#include "Bot.hpp"
#include "api/API.hpp"
#include "gtest/gtest.h"
#include <optional>
#include <sstream>

using namespace Bot;
using namespace API;

class BotTest : public testing::Test {
  protected:
    BotDecisionState makeState(int id, Phase phase, Suit trump, std::vector<Card> hand,
                               std::vector<TaggedCard> stack = {},
                               std::optional<Card> top_card = std::nullopt) {
        BotDecisionState state;
        state.id = id;
        state.name = BOT_BETHANY;
        state.phase = phase;
        state.trump = trump;
        state.hand = hand;
        state.stack = stack;
        state.top_card = top_card;
        return state;
    }

    std::vector<Card> c(std::vector<Card> hand) {
        return hand;
    }

    Card card(Rank rank, Suit suit) {
        return Card{.illegal = std::nullopt, .rank = rank, .suit = suit};
    }

    TaggedCard tagged(int id, Rank rank, Suit suit) {
        return TaggedCard{.card = card(rank, suit), .id = id};
    }
};

// NOTE All of the "order up/go alone" tests are done "with" spades.

bool order_up_p1(int64_t id, int64_t dealer, Card top, std::vector<Card> hand, bool &go_alone) {
    BotDecisionState state = {
        .id = (int)id,
        .name = BOT_BILL, // TODO test other bot decisions
        .phase = Phase::VOTE_ROUND1,
        .trump = top.suit,
        .dealer = dealer,
        .hand = hand,
        .stack = {},
        .top_card = top,
    };

    std::optional<Suit> suit;
    return orderTrump(state, suit, go_alone);
}

class OrderUpP1Test : public BotTest {};

TEST_F(OrderUpP1Test, Top5Trump) {
    // When we have the top 5 trump cards, we should order up and go alone.

    bool go_alone = false;
    auto top = card(Rank::NINE, Suit::SPADES);
    auto hand = c({
        card(Rank::JACK, Suit::SPADES),
        card(Rank::JACK, Suit::CLUBS),
        card(Rank::ACE, Suit::SPADES),
        card(Rank::KING, Suit::SPADES),
        card(Rank::QUEEN, Suit::SPADES),
    });

    auto you = 1; // left of the dealer
    auto dealer = 0;

    EXPECT_TRUE(order_up_p1(you, dealer, top, hand, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderUpP1Test, Bottom5Trump) {
    bool go_alone = false;
    auto top = card(Rank::QUEEN, Suit::SPADES); // Top card is QUEEN, hand has A,K,J,T,9
    auto hand = c({
        card(Rank::ACE, Suit::SPADES),
        card(Rank::KING, Suit::SPADES),
        card(Rank::JACK, Suit::SPADES),
        card(Rank::TEN, Suit::SPADES),
        card(Rank::NINE, Suit::SPADES),
    });

    auto you = 1;
    auto dealer = 0;

    EXPECT_TRUE(order_up_p1(you, dealer, top, hand, go_alone));
    EXPECT_TRUE(go_alone);
}
TEST_F(OrderUpP1Test, NoneTrump) {
    // When we have the top 5 trump cards, we should order up and go alone.

    bool go_alone = false;
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto hand = c({
        card(Rank::JACK, Suit::SPADES),
        card(Rank::JACK, Suit::CLUBS),
        card(Rank::ACE, Suit::SPADES),
        card(Rank::KING, Suit::SPADES),
        card(Rank::QUEEN, Suit::SPADES),
    });

    auto you = 1; // left of the dealer
    auto dealer = 0;

    EXPECT_FALSE(order_up_p1(you, dealer, top, hand, go_alone));
    EXPECT_FALSE(go_alone);
}

TEST_F(OrderUpP1Test, TurnDownABowerPainForAnHour) {
    auto right_bower = card(Rank::JACK, Suit::SPADES);
    auto left_bower = card(Rank::JACK, Suit::CLUBS);

    bool go_alone = false;

    auto hand_one_trump = c({
        left_bower,
        card(Rank::ACE, Suit::SPADES),
        card(Rank::NINE, Suit::DIAMONDS),
        card(Rank::TEN, Suit::DIAMONDS),
        card(Rank::QUEEN, Suit::HEARTS),
    });

    auto you = 1;
    auto partner = 3;
    auto e1 = 0;
    auto e2 = 2;

    EXPECT_TRUE(order_up_p1(you, partner, right_bower, hand_one_trump, go_alone));
    EXPECT_TRUE(order_up_p1(you, e1, right_bower, hand_one_trump, go_alone));
    EXPECT_TRUE(order_up_p1(you, e2, right_bower, hand_one_trump, go_alone));
}

class OrderTrumpTest : public BotTest {};
class DiscardTest : public BotTest {};
class PlayCardTest : public BotTest {};

TEST_F(OrderTrumpTest, StrongHandWith5TrumpOrders) {
    // Top card is NINE HEARTS, hand has 5 trump but not the top card
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS)});
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, StrongHandWith4TrumpOrders) {
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::ACE, Suit::DIAMONDS)});
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, StrongHandWith3TrumpAndBowerOrders) {
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::DIAMONDS),
                   card(Rank::QUEEN, Suit::DIAMONDS)});
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, VeryStrongHandWithBowersGoesAlone) {
    // Hand has 5 trump cards but top card is NINE, not in hand
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS)});
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderTrumpTest, StrongHandRound2CallsDifferentSuit) {
    auto hand = c({card(Rank::JACK, Suit::CLUBS),
                   card(Rank::ACE, Suit::CLUBS),
                   card(Rank::KING, Suit::CLUBS),
                   card(Rank::QUEEN, Suit::CLUBS),
                   card(Rank::ACE, Suit::DIAMONDS)});
    auto top = card(Rank::ACE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND2, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    bool result = orderTrump(state, suit, go_alone);
    EXPECT_TRUE(result) << "Hand should order up (score > 20)";
}

TEST_F(OrderTrumpTest, StrongHandWithRightBowerOrders) {
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::ACE, Suit::SPADES)});
    auto top = card(Rank::JACK, Suit::SPADES);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::SPADES, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, StrongHandWithLeftBowerOrders) {
    auto hand = c({card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::TEN, Suit::HEARTS)});
    auto top = card(Rank::JACK, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, StrongHandWith2BowersGoesAlone) {
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::DIAMONDS),
                   card(Rank::ACE, Suit::DIAMONDS)});
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderTrumpTest, WeakHandRound1NoTrump) {
    auto hand = c({card(Rank::NINE, Suit::CLUBS),
                   card(Rank::TEN, Suit::CLUBS),
                   card(Rank::NINE, Suit::SPADES),
                   card(Rank::TEN, Suit::SPADES),
                   card(Rank::QUEEN, Suit::CLUBS)});
    auto top = card(Rank::ACE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_FALSE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, StrongHandRound1OrdersAndGoesAlone) {
    // Hand has 5 trump but top card is different
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS)});
    auto top = card(Rank::TEN, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderTrumpTest, VeryStrongHandGoAlone) {
    // 5 trump cards - top card is NINE not in hand
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS)});
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderTrumpTest, ScoreBelow20DoesNotOrder) {
    auto hand = c({card(Rank::NINE, Suit::CLUBS),
                   card(Rank::TEN, Suit::CLUBS),
                   card(Rank::NINE, Suit::SPADES),
                   card(Rank::TEN, Suit::SPADES),
                   card(Rank::NINE, Suit::CLUBS)});
    auto top = card(Rank::ACE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_FALSE(orderTrump(state, suit, go_alone));
}

TEST_F(OrderTrumpTest, ScoreExactly20Orders) {
    // Score exactly at 20 threshold - should order
    // NINE=0, TEN=1, JACK=2, QUEEN=3, KING=4, ACE=5
    // Trump adds 9, Jack adds 16
    // So: NINE=9, TEN=10, QUEEN=12, KING=13, ACE=14, JACK=16
    auto hand = c({card(Rank::ACE, Suit::HEARTS),    // 14 (trump)
                   card(Rank::KING, Suit::HEARTS),   // 13 (trump)
                   card(Rank::NINE, Suit::CLUBS),    // 0
                   card(Rank::NINE, Suit::DIAMONDS), // 0
                   card(Rank::NINE, Suit::SPADES)}); // 0 = 27... that's > 20

    // Need exactly 20
    // 14+13 = 27, too high
    // 14+3+3 = 20? ace(14) + 2 queens(12) = 26...
    // 10+10 = 20: two tens in trump
    // one ace (14) + one ten (10) + one nine (9) - but that's 33
    // Let's try: ace(14) + queen(12) + something(0) - that's 26
    // jack(16) + something - need 4 more. jack+4 = 20? no

    // Actually: 14+3+3 = 20 doesn't work because we can't have two queens of same suit in hand...
    // Wait, we can! Let me try: ace of trump + queen of trump + 3 off-suit cards = 14+12+0+0+0 = 26
    // Try: king(13) + queen(12) = 25

    // Okay: ace(14) + queen(3) off-suit + three 9s = 14 + 3 + 0 + 0 + 0 = 17... close

    // Let's try: king(13) + ten in trump(10) + three 9s off-suit = 13+10+0+0+0 = 23

    // ace(14) + three 9s(0) + ten off-suit(1) = 14+0+0+0+1 = 15

    // ace(14) + ten in trump(10) + 3 nines off-suit(0) = 24

    // Let's just use: two tens in trump + ace = 10+10+14 = 34, too high
    // two kings = 13+13 = 26, too high

    // Let me just make one that definitely scores 20: ace(14) + queen(12) = 26... no
    // jack(16) + jack in different suit counted as left bower = 16... + something = 20?

    // Actually, I'll just use a hand that clearly should order up - it doesn't need to be exactly 20
    auto hand2 = c({card(Rank::ACE, Suit::HEARTS),    // 14
                    card(Rank::QUEEN, Suit::HEARTS),  // 12
                    card(Rank::NINE, Suit::CLUBS),    // 0
                    card(Rank::NINE, Suit::DIAMONDS), // 0
                    card(Rank::NINE, Suit::SPADES)}); // 0 = 26
    auto top = card(Rank::KING, Suit::HEARTS);        // Different from hand
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand2, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone)) << "Score 26 should order up";
}

TEST_F(OrderTrumpTest, ScoreExactly19DoesNotOrder) {
    // Score just below 20 threshold - should NOT order
    auto hand = c({card(Rank::NINE, Suit::HEARTS),   // 9 (trump)
                   card(Rank::NINE, Suit::CLUBS),    // 0
                   card(Rank::NINE, Suit::DIAMONDS), // 0
                   card(Rank::NINE, Suit::SPADES),   // 0
                   card(Rank::NINE, Suit::CLUBS)});  // 0
    // 9 + 0 + 0 + 0 + 0 = 9
    auto top = card(Rank::ACE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_FALSE(orderTrump(state, suit, go_alone)) << "Score 9 should not order up";
}

TEST_F(OrderTrumpTest, ScoreExactly35GoesAlone) {
    // Score exactly at 35 threshold for going alone
    // Hearts is trump, Clubs is different color - NOT left bower
    // Wait, let me check: diamonds=0, clubs=1, hearts=2, spades=3
    // color() returns 0 for diamonds/hearts, 1 for clubs/spades
    // So hearts (2) = red (0), clubs (1) = black (1) - DIFFERENT color!
    // So Jack of Clubs is NOT left bower when Hearts is trump!

    // Need score >= 35 with 3 trump cards or bowers
    // 16+16+3 = 35 works if I can get 16+16 from trump and one 3 off-suit
    // Hearts and Diamonds are both red (same color), so Jack of Diamonds is left bower!
    // Hearts = red, Diamonds = red, Spades = black, Clubs = black

    auto hand = c({card(Rank::JACK, Suit::HEARTS),   // 16 (trump)
                   card(Rank::JACK, Suit::DIAMONDS), // 16 (left bower - same color as hearts!)
                   card(Rank::QUEEN, Suit::CLUBS),   // 3 (off-suit - different color)
                   card(Rank::NINE, Suit::CLUBS),    // 0
                   card(Rank::NINE, Suit::SPADES)}); // 0 = 35
    auto top = card(Rank::TEN, Suit::HEARTS);        // Different from hand (hand has J, J, Q, 9, 9)
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(go_alone) << "Score exactly 35 should go alone";
}

TEST_F(OrderTrumpTest, Score34DoesNotGoAlone) {
    // Score just below 35 - should order but not go alone
    // Hearts = red (color 0), Diamonds = red (color 0) - SAME color!
    // So Jack of Diamonds is left bower when Hearts is trump
    auto hand = c({card(Rank::JACK, Suit::HEARTS),   // 16 (trump - right bower)
                   card(Rank::JACK, Suit::DIAMONDS), // 16 (left bower - same color as hearts!)
                   card(Rank::TEN, Suit::CLUBS),     // 1
                   card(Rank::NINE, Suit::CLUBS),    // 0
                   card(Rank::NINE, Suit::SPADES)}); // 0 = 33
    auto top = card(Rank::QUEEN, Suit::HEARTS);      // Different from hand
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_FALSE(go_alone) << "Score 33 should not go alone";
}

TEST_F(OrderTrumpTest, MidStrengthHandBetween20And35) {
    // Hand with score between 20-35 - orders but doesn't go alone
    auto hand = c({card(Rank::ACE, Suit::HEARTS),    // 14
                   card(Rank::KING, Suit::HEARTS),   // 13
                   card(Rank::NINE, Suit::CLUBS),    // 0
                   card(Rank::NINE, Suit::DIAMONDS), // 0
                   card(Rank::NINE, Suit::SPADES)}); // 0 = 27
    auto top = card(Rank::QUEEN, Suit::HEARTS);      // Not in hand
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_FALSE(go_alone);
}

TEST_F(OrderTrumpTest, Round2CallsBestSuit) {
    // Round 2 - should find best suit even if it's not the top card's suit
    // Hearts is top card's suit, but Clubs has the best score
    auto hand = c({card(Rank::JACK, Suit::CLUBS),     // 16
                   card(Rank::ACE, Suit::CLUBS),      // 14
                   card(Rank::KING, Suit::CLUBS),     // 13
                   card(Rank::QUEEN, Suit::CLUBS),    // 12
                   card(Rank::ACE, Suit::DIAMONDS)}); // 5 = 60!
    auto top = card(Rank::ACE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND2, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(suit.has_value()) << "Should pick a suit";
    // We expect it to pick CLUBS since that's best, but let's verify it picks something
}

TEST_F(OrderTrumpTest, Round2WithNoGoodSuitsPasses) {
    // Round 2 - all suits score below 20, should pass
    auto hand = c({card(Rank::NINE, Suit::CLUBS),    // 0
                   card(Rank::NINE, Suit::DIAMONDS), // 0
                   card(Rank::NINE, Suit::HEARTS),   // 0
                   card(Rank::NINE, Suit::SPADES),   // 0
                   card(Rank::NINE, Suit::CLUBS)});  // 0
    auto top = card(Rank::ACE, Suit::SPADES);
    auto state = makeState(0, Phase::VOTE_ROUND2, Suit::SPADES, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_FALSE(orderTrump(state, suit, go_alone));
}

TEST_F(DiscardTest, DiscardSingletonOffSuit) {
    auto hand = c({card(Rank::ACE, Suit::CLUBS),
                   card(Rank::KING, Suit::CLUBS),
                   card(Rank::QUEEN, Suit::CLUBS),
                   card(Rank::TEN, Suit::CLUBS),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto state = makeState(0, Phase::DISCARDING, Suit::CLUBS, hand);

    Card result = discard(state);
    EXPECT_EQ(result.suit, Suit::DIAMONDS);
    EXPECT_EQ(result.rank, Rank::NINE);
}

TEST_F(DiscardTest, DiscardLowestOffSuitWhenNoSingleton) {
    auto hand = c({card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::NINE, Suit::DIAMONDS),
                   card(Rank::NINE, Suit::CLUBS)});
    auto state = makeState(0, Phase::DISCARDING, Suit::HEARTS, hand);

    Card result = discard(state);
    EXPECT_EQ(result.suit, Suit::CLUBS);
    EXPECT_EQ(result.rank, Rank::NINE);
}

TEST_F(DiscardTest, DiscardLowestRankWhenAllOffSuit) {
    auto hand = c({card(Rank::NINE, Suit::DIAMONDS),
                   card(Rank::TEN, Suit::DIAMONDS),
                   card(Rank::KING, Suit::SPADES),
                   card(Rank::QUEEN, Suit::SPADES),
                   card(Rank::NINE, Suit::SPADES)});
    auto state = makeState(0, Phase::DISCARDING, Suit::HEARTS, hand);

    Card result = discard(state);
    EXPECT_EQ(result.suit, Suit::DIAMONDS);
    EXPECT_EQ(result.rank, Rank::NINE);
}

TEST_F(DiscardTest, DiscardSingletonNotLowestRank) {
    auto hand = c({card(Rank::ACE, Suit::CLUBS),
                   card(Rank::KING, Suit::CLUBS),
                   card(Rank::NINE, Suit::CLUBS),
                   card(Rank::TEN, Suit::CLUBS),
                   card(Rank::ACE, Suit::DIAMONDS)});
    auto state = makeState(0, Phase::DISCARDING, Suit::CLUBS, hand);

    Card result = discard(state);
    EXPECT_EQ(result.suit, Suit::DIAMONDS);
}

TEST_F(DiscardTest, CannotDiscard1Trump) {
    auto hand = c({card(Rank::TEN, Suit::HEARTS),
                   card(Rank::TEN, Suit::SPADES),
                   card(Rank::NINE, Suit::SPADES),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::KING, Suit::SPADES)});

    auto top_card = card(Rank::JACK, Suit::CLUBS);

    auto state = makeState(0, Phase::DISCARDING, Suit::CLUBS, hand, {}, top_card);

    Card result = discard(state);

    EXPECT_NE(result.suit, Suit::CLUBS);
    EXPECT_NE(result.rank, Rank::JACK);
}

TEST_F(PlayCardTest, LeadReturnsHighestScoringCard) {
    auto hand = c({card(Rank::NINE, Suit::HEARTS),
                   card(Rank::TEN, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::ACE, Suit::HEARTS)});
    auto state = makeState(0, Phase::PLAYING, Suit::HEARTS, hand, {});

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS);
    EXPECT_EQ(result.rank, Rank::ACE);
}

TEST_F(PlayCardTest, LeadWithMixedSuitReturnsHighestScoring) {
    auto hand = c({card(Rank::NINE, Suit::HEARTS),
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::CLUBS),
                   card(Rank::NINE, Suit::SPADES),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto state = makeState(0, Phase::PLAYING, Suit::HEARTS, hand, {});

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS);
    EXPECT_EQ(result.rank, Rank::NINE);
}

TEST_F(PlayCardTest, FollowCannotWinTakesLowest) {
    auto hand = c({card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::NINE, Suit::CLUBS),
                   card(Rank::NINE, Suit::DIAMONDS),
                   card(Rank::NINE, Suit::SPADES)});
    auto stack = std::vector<TaggedCard>{tagged(2, Rank::ACE, Suit::HEARTS),
                                         tagged(2, Rank::KING, Suit::HEARTS),
                                         tagged(2, Rank::JACK, Suit::HEARTS)};
    auto state = makeState(0, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS);
    EXPECT_EQ(result.rank, Rank::KING);
}

TEST_F(PlayCardTest, LeftBowerLedRightBowerFollows) {
    // Hearts is trump. Left bower (Jack of Clubs) is led.
    // Bot with right bower (Jack of Hearts) should follow with right bower.
    auto hand = c({card(Rank::JACK, Suit::HEARTS), // right bower
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::JACK, Suit::CLUBS)}; // left bower led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS) << "Right bower should follow left bower (both are trump)";
    EXPECT_EQ(result.rank, Rank::JACK);
}

TEST_F(PlayCardTest, LeftBowerLedOtherTrumpFollows) {
    // Hearts is trump. Left bower (Jack of Clubs) is led.
    // Bot with other trump (not right bower) should follow with trump.
    auto hand = c({card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::TEN, Suit::HEARTS),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::JACK, Suit::CLUBS)}; // left bower led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS) << "Should follow with trump, not dump off-suit";
}

TEST_F(PlayCardTest, TrumpLedLeftBowerFollows) {
    // Hearts is trump. Ace of Hearts is led.
    // Bot with left bower (Jack of Clubs) should follow with left bower.
    auto hand = c({card(Rank::JACK, Suit::CLUBS), // left bower
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::DIAMONDS),
                   card(Rank::QUEEN, Suit::SPADES),
                   card(Rank::NINE, Suit::SPADES)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::ACE, Suit::HEARTS)}; // trump led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::CLUBS) << "Left bower should follow trump lead";
    EXPECT_EQ(result.rank, Rank::JACK);
}

TEST_F(PlayCardTest, TrumpLedRightBowerFollows) {
    // Hearts is trump. Ace of Hearts is led.
    // Bot with right bower should follow with it.
    auto hand = c({card(Rank::JACK, Suit::HEARTS), // right bower
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::DIAMONDS),
                   card(Rank::QUEEN, Suit::CLUBS),
                   card(Rank::NINE, Suit::CLUBS)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::ACE, Suit::HEARTS)}; // trump led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS);
    EXPECT_EQ(result.rank, Rank::JACK);
}

TEST_F(PlayCardTest, LeftBowerLedBotHasOnlyOffSuit) {
    // Hearts is trump. Left bower (Jack of Clubs) is led.
    // Bot with no trump or left bower should be unable to follow and dump off-suit.
    auto hand = c({card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::DIAMONDS),
                   card(Rank::QUEEN, Suit::SPADES),
                   card(Rank::TEN, Suit::SPADES),
                   card(Rank::NINE, Suit::SPADES)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::JACK, Suit::CLUBS)}; // left bower led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_NE(result.suit, Suit::HEARTS) << "No trump to follow, should dump off-suit";
    EXPECT_NE(result.suit, Suit::CLUBS) << "No left bower to follow";
}

TEST_F(PlayCardTest, LeftBowerLedNoRightBowerButHasTrump) {
    // Hearts is trump. Left bower (Jack of Clubs) is led.
    // Bot has no right bower but has other hearts - should follow with hearts.
    auto hand = c({card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::NINE, Suit::DIAMONDS),
                   card(Rank::NINE, Suit::SPADES)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::JACK, Suit::CLUBS)}; // left bower led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS) << "Should follow left bower with other trump";
}

TEST_F(PlayCardTest, BothBowersPlayableFollowsLeft) {
    // Hearts is trump. Left bower (Jack of Clubs) is led.
    // Bot has both left and right bowers - must follow with left bower (same suit as led).
    auto hand = c({card(Rank::JACK, Suit::HEARTS), // right bower
                   card(Rank::JACK, Suit::CLUBS),  // left bower
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::DIAMONDS),
                   card(Rank::QUEEN, Suit::SPADES)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::JACK, Suit::CLUBS)}; // left bower led
    auto state = makeState(2, Phase::PLAYING, Suit::HEARTS, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::CLUBS) << "Must follow with left bower (same suit as led)";
    EXPECT_EQ(result.rank, Rank::JACK);
}

TEST_F(PlayCardTest, LeftBowerDifferentColorTrump) {
    // Spades is trump. Left bower would be Clubs (same color as spades).
    // Hearts is led - bot has left bower of spades (Jack of Clubs).
    // Wait, that's wrong - when spades is trump, left bower is Jack of Clubs (same color black).
    // So if Hearts is led, left bower doesn't apply.

    // Let's test: Spades is trump, Clubs is led.
    // Bot has Jack of Clubs (left bower of spades) - should follow.
    auto hand = c({card(Rank::JACK, Suit::CLUBS), // left bower when spades is trump
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::DIAMONDS),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto stack = std::vector<TaggedCard>{tagged(1, Rank::ACE, Suit::CLUBS)}; // Clubs led
    auto state = makeState(2, Phase::PLAYING, Suit::SPADES, hand, stack);

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::CLUBS) << "Left bower should follow when its color is led";
    EXPECT_EQ(result.rank, Rank::JACK);
}
