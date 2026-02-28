#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "Consts.h"
#include "RichTextStream.h"

#include <chrono>
#include <functional>
#include <json.hpp>
#include <string>

#include <vector>

using json = nlohmann::json;

typedef std::function<void(std::string)> SendFunc;

class GameCoordinator;
struct HandlerArgs {
    SendFunc broadcast;
    SendFunc send;
    std::string &session;
};

class Game {
  public:
    Game();
    ~Game();
    Game(bool isPrivate) : Game() {
        this->state.private_session = isPrivate;
    }

    // Rehydrate game from disk
    Game(const API::GameState &g) : Game(true) {
        this->state = g;
        if (this->state.players.size()) {
            turn_token = g.players[g.turn].session;
        }
        for (auto &player : this->state.players) {
            player.connected = false;
        }
    }

    bool isInitialized();
    bool isPrivate() const;
    std::string hostName() const;

    bool hasPlayer(std::string &id);
    API::JoinMsg addPlayer(const PerSocketData &data);
    int getPlayerId(const std::string &id);

    json disconnectPlayer(std::string id);
    json reconnectPlayer(std::string id, std::string old_id, const PerSocketData &data);
    bool isPlayerConnected(std::string id) const;

    void advanceTurn();

    std::chrono::system_clock::time_point getUpdated() { return this->updated; }

    void handleMessage(const HandlerArgs &server, const std::string_view message);

#define X(NAME, TYPE) \
    void NAME(const HandlerArgs &server, const API::TYPE &msg);

#include "Handlers.def"
#undef X

    int connectedPlayerCount();

    API::WelcomeMsg toWelcomeMsg(const std::string &session) {
        std::vector<API::Player> players;
        for (const auto &player : this->state.players) {
            players.emplace_back(player.cards.size(), player.connected, player.name);
        }
        auto id = this->getPlayerId(session);
        auto your_cards = this->state.players[id].cards;
        return API::WelcomeMsg{
            .dealer = this->state.dealer,
            .id = id,
            .phase = this->state.phase,
            .played_cards = this->state.played_cards,
            .players = players,
            .private_session = this->state.private_session,
            .rich_chat_log = this->state.rich_chat_log,
            .scores = this->state.scores,
            .top_card = this->state.top_card,
            .trick = this->state.trick,
            .trump = this->state.trump,
            .turn = this->state.turn,
            .your_cards = your_cards,
        };
    }

    std::string toString() const {
        return this->state.toString();
    }

  private:
    std::string log_rich_chat(const RichTextStream &stream) {
        state.rich_chat_log.insert(state.rich_chat_log.begin(), stream.obj());
        if (state.rich_chat_log.size() > MAX_CHAT_LOG) {
            state.rich_chat_log.pop_back();
        }
        return stream.str();
    }
    std::chrono::system_clock::time_point updated = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point turn_start_time = std::chrono::system_clock::now();
    std::string turn_token;
    API::GameState state;
    bool was_persisted = false;
    friend GameCoordinator;
};

#endif
