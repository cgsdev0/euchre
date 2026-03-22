#include "Bot.hpp"
#include "api/Rank.hpp"
#include "api/Suit.hpp"
#include "api/Phase.hpp"
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
    EXPECT_FALSE(orderTrump(state, suit, go_alone));
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

TEST_F(PlayCardTest, LeadReturnsLowestScoringCard) {
    auto hand = c({card(Rank::NINE, Suit::HEARTS),
                   card(Rank::TEN, Suit::HEARTS),
                   card(Rank::KING, Suit::HEARTS),
                   card(Rank::QUEEN, Suit::HEARTS),
                   card(Rank::ACE, Suit::HEARTS)});
    auto state = makeState(0, Phase::PLAYING, Suit::HEARTS, hand, {});

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::HEARTS);
    EXPECT_EQ(result.rank, Rank::NINE);
}

TEST_F(PlayCardTest, LeadWithMixedSuitReturnsLowestScoring) {
    auto hand = c({card(Rank::NINE, Suit::HEARTS),
                   card(Rank::ACE, Suit::DIAMONDS),
                   card(Rank::KING, Suit::CLUBS),
                   card(Rank::NINE, Suit::SPADES),
                   card(Rank::NINE, Suit::DIAMONDS)});
    auto state = makeState(0, Phase::PLAYING, Suit::HEARTS, hand, {});

    Card result = playCard(state);
    EXPECT_EQ(result.suit, Suit::SPADES);
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


