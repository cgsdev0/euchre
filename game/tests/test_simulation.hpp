#ifndef TEST_SIMULATION_H
#define TEST_SIMULATION_H

#include "Game.h"
#include "Bot.hpp"
#include "gtest/gtest.h"
#include <optional>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

using namespace API;
using namespace Bot;

enum class LogLevel {
    QUIET,
    SUMMARY,
    VERBOSE
};

struct SimulationResult {
    int winner;          // 0 or 1
    std::array<int, 2> scores;
    int tricks_team0;
    int tricks_team1;
};

class SimulationRunner {
public:
    SimulationRunner(LogLevel level = LogLevel::QUIET) : log_level(level) {}

    SimulationResult runRound() {
        Game game;
        HandlerArgs server = makeServer();
        
        RestartMsg msg;
        game.restart(server, msg);
        
        auto state = game.getState();
        if (log_level >= LogLevel::SUMMARY) {
            std::cout << "Starting round. Scores: " << state.scores[0] << "-" << state.scores[1] << std::endl;
        }
        
        playUntilEnd(game, server);
        
        state = game.getState();
        int tricks0 = state.players[0].tricks + state.players[2].tricks;
        int tricks1 = state.players[1].tricks + state.players[3].tricks;
        
        if (log_level >= LogLevel::SUMMARY) {
            std::cout << "Round complete. Winner: Team " << (state.scores[0] > state.scores[1] ? 0 : 1) 
                      << ". Scores: " << state.scores[0] << "-" << state.scores[1] << std::endl;
        }
        
        return SimulationResult{
            .winner = state.scores[0] > state.scores[1] ? 0 : 1,
            .scores = {state.scores[0], state.scores[1]},
            .tricks_team0 = tricks0,
            .tricks_team1 = tricks1
        };
    }

private:
    LogLevel log_level;
    std::vector<std::string> broadcasts;
    std::string dummy_session = "test_session";
    
    HandlerArgs makeServer() {
        return HandlerArgs{
            .broadcast = [this](const std::string &msg) {
                this->broadcasts.push_back(msg);
            },
            .dm = [](const std::string &who, const std::string &msg) {},
            .session = dummy_session
        };
    }
    
    void playUntilEnd(Game& game, HandlerArgs& server) {
        while (true) {
            auto state = game.getState();
            
            if (state.phase == Phase::ENDED || state.phase == Phase::LOBBY) {
                break;
            }
            
            int turn = state.turn;
            if (turn >= 0 && turn < (int)state.players.size()) {
                auto& player = state.players[turn];
                if (player.bot_enum != BotName::BOT_NONE) {
                    game.handleBotUpdates(server);
                } else {
                    break;
                }
            } else {
                break;
            }
        }
    }
};

class SimulationTest : public testing::Test {
protected:
    LogLevel log_level = LogLevel::QUIET;
    std::vector<std::string> broadcasts;
    
    HandlerArgs makeServer() {
        return HandlerArgs{
            .broadcast = [this](const std::string &msg) {
                this->broadcasts.push_back(msg);
            },
            .dm = [](const std::string &who, const std::string &msg) {},
            .session = broadcasts[0]
        };
    }
};

class MonteCarloTest : public SimulationTest {};
class ScenarioTest : public SimulationTest {};

TEST_F(MonteCarloTest, TenRoundsWithFourBots) {
    int team0_wins = 0;
    int team1_wins = 0;
    
    for (int i = 0; i < 10; i++) {
        SimulationRunner runner(log_level);
        auto result = runner.runRound();
        
        if (result.winner == 0) team0_wins++;
        else team1_wins++;
    }
    
    EXPECT_GT(team0_wins, 0);
    EXPECT_GT(team1_wins, 0);
    EXPECT_EQ(team0_wins + team1_wins, 10);
    
    if (log_level >= LogLevel::SUMMARY) {
        std::cout << "Team 0 wins: " << team0_wins << ", Team 1 wins: " << team1_wins << std::endl;
    }
}

#endif // TEST_SIMULATION_H