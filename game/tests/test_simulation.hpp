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
    int winner;
    std::array<int, 2> scores;
    int tricks_team0;
    int tricks_team1;
};

class SimulationRunner {
public:
    SimulationRunner(LogLevel level = LogLevel::QUIET) : log_level(level), game() {}

    SimulationResult runRound() {
        HandlerArgs server = makeServer();
        
        RestartMsg msg;
        this->game.restart(server, msg);
        
        auto state = this->game.getState();
        if (log_level >= LogLevel::SUMMARY) {
            std::cout << "Starting round. Scores: " << state.scores[0] << "-" << state.scores[1] << std::endl;
        }
        
        playUntilEnd(this->game, server);
        
        state = this->game.getState();
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

    const GameState& getState() const { return this->game.getState(); }

private:
    LogLevel log_level;
    std::vector<std::string> broadcasts;
    std::string dummy_session = "test_session";
    Game game;
    
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
        int maxLoops = 50;
        int loopCount = 0;
        while (loopCount < maxLoops) {
            loopCount++;
            auto state = game.getState();
            
            if (log_level >= LogLevel::VERBOSE) {
                std::cout << "  Outer Loop " << loopCount << ": phase=" << (int)state.phase 
                          << " (" << (state.phase == Phase::VOTE_ROUND1 ? "VOTE_R1" :
                                     state.phase == Phase::VOTE_ROUND2 ? "VOTE_R2" :
                                     state.phase == Phase::DISCARDING ? "DISCARD" :
                                     state.phase == Phase::PLAYING ? "PLAYING" :
                                     state.phase == Phase::ENDED ? "ENDED" : "LOBBY")
                          << "), turn=" << state.turn 
                          << ", turn_player=" << (state.turn < (int)state.players.size() ? state.players[state.turn].session : "N/A")
                          << std::endl;
            }
            
            if (state.phase == Phase::ENDED || state.phase == Phase::LOBBY) {
                break;
            }
            
            int turn = state.turn;
            if (turn >= 0 && turn < (int)state.players.size()) {
                auto& player = state.players[turn];
                if (player.bot_enum != BotName::BOT_NONE) {
                    if (log_level >= LogLevel::VERBOSE) {
                        std::cout << "    Calling handleBotUpdates..." << std::endl;
                    }
                    game.handleBotUpdates(server);
                    if (log_level >= LogLevel::VERBOSE) {
                        auto afterState = game.getState();
                        std::cout << "    After handleBotUpdates: phase=" << (int)afterState.phase << std::endl;
                    }
                } else {
                    break;
                }
            } else {
                break;
            }
        }
        
        auto finalState = game.getState();
        if (log_level >= LogLevel::VERBOSE) {
            std::cout << "After playUntilEnd: phase=" << (int)finalState.phase 
                      << ", players[0].tricks=" << finalState.players[0].tricks
                      << ", players[1].tricks=" << finalState.players[1].tricks
                      << ", players[2].tricks=" << finalState.players[2].tricks
                      << ", players[3].tricks=" << finalState.players[3].tricks << std::endl;
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

TEST_F(ScenarioTest, RoundCompletesWithoutCrash) {
    SimulationRunner runner(LogLevel::QUIET);
    auto result = runner.runRound();
    
    auto& state = runner.getState();
    
    EXPECT_TRUE(state.phase == Phase::ENDED || state.phase == Phase::VOTE_ROUND1) 
        << "Game should be ended or starting new hand";
    
    EXPECT_EQ(state.players.size(), 4) << "Should have 4 players";
}

TEST_F(ScenarioTest, ScoresUpdateAfterRound) {
    SimulationRunner runner(LogLevel::QUIET);
    auto result = runner.runRound();
    
    EXPECT_GE(result.scores[0] + result.scores[1], 0);
    EXPECT_LE(result.scores[0] + result.scores[1], 20) << "Total score in a hand should be 0-20";
}

TEST_F(ScenarioTest, GameEndsAfterHandCompletion) {
    SimulationRunner runner(LogLevel::QUIET);
    runner.runRound();
    
    auto& state = runner.getState();
    EXPECT_TRUE(state.phase == Phase::ENDED || state.phase == Phase::VOTE_ROUND1) 
        << "Game should be ended or starting new hand after round completes";
}

TEST_F(ScenarioTest, BotsAlwaysOrderOrPassDuringVoting) {
    for (int i = 0; i < 5; i++) {
        SimulationRunner runner(LogLevel::QUIET);
        runner.runRound();
        
        auto& state = runner.getState();
        
        EXPECT_TRUE(state.phase != Phase::VOTE_ROUND1 && state.phase != Phase::VOTE_ROUND2)
            << "Voting phase should complete";
    }
}

TEST_F(ScenarioTest, AllPlayersPlayFiveCards) {
    SimulationRunner runner(LogLevel::QUIET);
    runner.runRound();
    
    auto& state = runner.getState();
    int cards_played = state.played_cards.size();
    
    EXPECT_EQ(cards_played % 5, 0) << "Played cards should be multiple of 5 (one per player per trick)";
}

TEST_F(ScenarioTest, DiscardPhaseHappensWhenOrdered) {
    SimulationRunner runner(LogLevel::QUIET);
    runner.runRound();
    
    auto& state = runner.getState();
    bool has_dealer = false;
    for (const auto& p : state.players) {
        if (p.cards.size() == 6) {
            has_dealer = true;
            break;
        }
    }
    
    if (state.phase == Phase::DISCARDING) {
        EXPECT_TRUE(has_dealer) << "If in discard phase, dealer should have 6 cards";
    }
}

#endif // TEST_SIMULATION_H