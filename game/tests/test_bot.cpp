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
        card(Rank::JACK,  Suit::SPADES),
        card(Rank::JACK,  Suit::CLUBS),
        card(Rank::ACE,   Suit::SPADES),
        card(Rank::KING,  Suit::SPADES),
        card(Rank::QUEEN, Suit::SPADES),
    });

    auto you = 1; // left of the dealer
    auto dealer = 0;

    EXPECT_TRUE(order_up_p1(you, dealer, top, hand, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderUpP1Test, Bottom5Trump) {
    // When we have the top 5 trump cards, we should order up and go alone.

    bool go_alone = false;
    auto top = card(Rank::JACK, Suit::SPADES);
    auto hand = c({
        card(Rank::ACE,   Suit::SPADES),
        card(Rank::KING,  Suit::SPADES),
        card(Rank::QUEEN, Suit::SPADES),
        card(Rank::TEN,   Suit::SPADES),
        card(Rank::NINE,  Suit::SPADES),
    });

    auto you = 1; // left of the dealer
    auto dealer = 0;

    EXPECT_TRUE(order_up_p1(you, dealer, top, hand, go_alone));
    EXPECT_FALSE(go_alone);
}
TEST_F(OrderUpP1Test, NoneTrump) {
    // When we have the top 5 trump cards, we should order up and go alone.

    bool go_alone = false;
    auto top = card(Rank::NINE, Suit::HEARTS);
    auto hand = c({
        card(Rank::JACK,  Suit::SPADES),
        card(Rank::JACK,  Suit::CLUBS),
        card(Rank::ACE,   Suit::SPADES),
        card(Rank::KING,  Suit::SPADES),
        card(Rank::QUEEN, Suit::SPADES),
    });

    auto you = 1; // left of the dealer
    auto dealer = 0;

    EXPECT_FALSE(order_up_p1(you, dealer, top, hand, go_alone));
    EXPECT_FALSE(go_alone);
}

TEST_F(OrderUpP1Test, TurnDownABowerPainForAnHour) {
    // When there's a Bower on top of the kitty, the bots should order it up for their partner or
    // themselves
    // - if the bot has the left, they should have one other trump
    // - if the bot has the right, they should have two other trump

    auto right_bower = card(Rank::JACK, Suit::SPADES);
    auto left_bower  = card(Rank::JACK, Suit::CLUBS);

    bool go_alone = false;

    auto hand_one_trump = c({
        left_bower,
        card(Rank::ACE,   Suit::SPADES),
        card(Rank::NINE,  Suit::DIAMONDS),
        card(Rank::TEN,   Suit::DIAMONDS),
        card(Rank::QUEEN, Suit::HEARTS),
    });

    auto hand_two_trump = c({
        card(Rank::ACE,   Suit::SPADES),
        card(Rank::TEN,   Suit::SPADES),
        card(Rank::JACK,  Suit::HEARTS),
        card(Rank::NINE,  Suit::DIAMONDS),
        card(Rank::QUEEN, Suit::HEARTS),
    });

    auto you = 1;
    auto e1 = 0;
    auto e2 = 2;
    auto partner = 3;

    // Right bower up, we should call if our partner is dealer.

    EXPECT_TRUE(order_up_p1(you, partner, right_bower, hand_one_trump, go_alone));
    EXPECT_FALSE(go_alone);
    EXPECT_TRUE(order_up_p1(you, partner, right_bower, hand_two_trump, go_alone));
    EXPECT_FALSE(go_alone);

    EXPECT_FALSE(order_up_p1(you, e1, right_bower, hand_one_trump, go_alone));
    EXPECT_FALSE(order_up_p1(you, e2, right_bower, hand_one_trump, go_alone));
}

class OrderTrumpTest : public BotTest {};
class DiscardTest : public BotTest {};
class PlayCardTest : public BotTest {};

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
    EXPECT_FALSE((state, suit, go_alone));
}

TEST_F(OrderTrumpTest, StrongHandRound1OrdersAndGoesAlone) {
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::NINE, Suit::HEARTS)});
    auto top = card(Rank::ACE, Suit::HEARTS);
    auto state = makeState(0, Phase::VOTE_ROUND1, Suit::HEARTS, hand, {}, top);

    std::optional<Suit> suit;
    bool go_alone = false;
    EXPECT_TRUE(orderTrump(state, suit, go_alone));
    EXPECT_TRUE(go_alone);
}

TEST_F(OrderTrumpTest, VeryStrongHandGoAlone) {
    auto hand = c({card(Rank::JACK, Suit::HEARTS),
                   card(Rank::JACK, Suit::SPADES),
                   card(Rank::ACE, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS)});
    auto top = card(Rank::ACE, Suit::HEARTS);
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

TEST_F(OrderTrumpTest, Round2NoGoodSuitPasses) {
    auto hand = c({card(Rank::NINE, Suit::HEARTS),
                   card(Rank::TEN, Suit::HEARTS),
                   card(Rank::NINE, Suit::CLUBS),
                   card(Rank::NINE, Suit::DIAMONDS),
                   card(Rank::NINE, Suit::SPADES)});
    auto top = card(Rank::ACE, Suit::CLUBS);
    auto state = makeState(0, Phase::VOTE_ROUND2, Suit::CLUBS, hand, {}, top);

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
    auto hand = c({
        card(Rank::TEN,   Suit::HEARTS),
        card(Rank::TEN,   Suit::SPADES),
        card(Rank::NINE,  Suit::SPADES),
        card(Rank::QUEEN, Suit::HEARTS),
        card(Rank::KING,  Suit::SPADES)
    });

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

TEST_F(PlayCardTest, LeadWithMixedSuitReturnsLowestScoring) {
    auto hand = c({card(Rank::NINE, Suit::HEARTS),
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::CLUBS),
                   card(Rank::NINE, Suit::SPADES),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto state = makeState(0, Phase::PLAYING, Suit::HEARTS, hand, {});

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::DIAMONDS);
    EXPECT_EQ(result.rank, Rank::ACE);
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


