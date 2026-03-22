#include "API.hpp"
#include "Game.h"
#include "gtest/gtest.h"

using namespace API;

class GameTest : public testing::Test {
  protected:
    GameTest() {
        g.addPlayer(PerSocketData{});
        g.addPlayer(PerSocketData{});
        g.addPlayer(PerSocketData{});
        g.addPlayer(PerSocketData{});
    }
    Game g;
};

TEST_F(GameTest, Something) {
    EXPECT_EQ(g.isPrivate(), true);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
