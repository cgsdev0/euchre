#include "API.hpp"
#include "Game.h"
#include "gtest/gtest.h"
#include <sstream>

using namespace API;

class GameTest : public testing::Test {
  protected:
    GameTest() {
    }
    Game g;
    std::string session;
    std::vector<std::string> broadcasts;
    HandlerArgs server = {
        .broadcast = [this](const std::string &msg) { this->broadcasts.push_back(msg); },
        .dm = [](const std::string &who, const std::string &msg) {},
        .send = [](const std::string &msg) {},
        .session = session};

    std::string log_broadcasts() {
        std::stringstream res;
        res << "BROADCASTS:" << std::endl;
        for (const auto &b : broadcasts) {
            res << b << std::endl;
        }
        return res.str();
    }
};

TEST_F(GameTest, LayDownLoner) {
    GameState state;
    state.turn = 1;
    state.scores = {0, 0};
    state.phase = Phase::PLAYING;
    state.trump = Suit::CLUBS;
    state.players = {
        {.cards = {/**/
                   {.rank = Rank::ACE, .suit = Suit::HEARTS},
                   {.rank = Rank::KING, .suit = Suit::HEARTS},
                   {.rank = Rank::QUEEN, .suit = Suit::HEARTS},
                   {.rank = Rank::NINE, .suit = Suit::HEARTS},
                   {.rank = Rank::TEN, .suit = Suit::HEARTS}
                   /**/},
         .connected = true,
         .name = "Alice",
         .premoves = {},
         .session = "alice",
         .sitting_out = false,
         .tricks = 0},
        {.cards = {/**/
                   {.rank = Rank::ACE, .suit = Suit::CLUBS},
                   {.rank = Rank::JACK, .suit = Suit::CLUBS},
                   {.rank = Rank::JACK, .suit = Suit::SPADES},
                   {.rank = Rank::NINE, .suit = Suit::CLUBS},
                   {.rank = Rank::TEN, .suit = Suit::CLUBS}
                   /**/},
         .connected = true,
         .name = "Bill",
         .premoves = {},
         .session = "bill",
         .sitting_out = false,
         .tricks = 0},
        {.cards = {/**/
                   {.rank = Rank::ACE, .suit = Suit::DIAMONDS},
                   {.rank = Rank::KING, .suit = Suit::DIAMONDS},
                   {.rank = Rank::QUEEN, .suit = Suit::DIAMONDS},
                   {.rank = Rank::NINE, .suit = Suit::DIAMONDS},
                   {.rank = Rank::TEN, .suit = Suit::DIAMONDS}
                   /**/},
         .connected = true,
         .name = "Chuck",
         .premoves = {},
         .session = "chuck",
         .sitting_out = false,
         .tricks = 0},
        {.cards = {/**/
                   {.rank = Rank::ACE, .suit = Suit::SPADES},
                   {.rank = Rank::KING, .suit = Suit::SPADES},
                   {.rank = Rank::QUEEN, .suit = Suit::SPADES},
                   {.rank = Rank::NINE, .suit = Suit::SPADES},
                   {.rank = Rank::TEN, .suit = Suit::SPADES}
                   /**/},
         .connected = true,
         .name = "Dave",
         .premoves = {},
         .session = "dave",
         .sitting_out = true,
         .tricks = 0},
    };
    {
        g = Game(state);
        session = "bill";
        LayDownMsg msg;
        g.lay_down(server, msg);
        const GameState &s = g.getState();
        EXPECT_EQ(s.scores[0], 0) << log_broadcasts();
        EXPECT_EQ(s.scores[1], 4) << log_broadcasts();
        EXPECT_EQ(s.phase, Phase::VOTE_ROUND1);
    }

    // reset
    {
        state.players[1].cards[4] =
            {.rank = Rank::ACE, .suit = Suit::DIAMONDS};
        g = Game(state);
        LayDownMsg msg;
        EXPECT_THROW(g.lay_down(server, msg), GameError);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
