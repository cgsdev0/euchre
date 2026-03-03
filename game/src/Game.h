#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "Consts.h"
#include "Deck.h"
#include "RichTextStream.h"

#include <chrono>
#include <functional>
#include <optional>
#include <string>

#include <vector>

typedef std::function<void(const std::string &)> SendFunc;
typedef std::function<void(const std::string &, const std::string &)> DmFunc;

class GameCoordinator;
struct HandlerArgs {
    SendFunc broadcast;
    DmFunc dm;
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

    std::optional<API::DisconnectMsg> disconnectPlayer(std::string id);
    std::optional<API::ReconnectMsg> reconnectPlayer(std::string id, const PerSocketData &data);
    bool isPlayerConnected(std::string id) const;

    void advanceTurn();
    void endGame(const HandlerArgs &server, int winner);
    void endHand(const HandlerArgs &server);
    void endTrick(const HandlerArgs &server);
    void dealCards(const HandlerArgs &server);
    void cascadePremoves(const HandlerArgs &server);

    std::chrono::system_clock::time_point getUpdated() { return this->updated; }

    void handleMessage(const HandlerArgs &server, const std::string_view message);

#define X(NAME, TYPE) \
    void NAME(const HandlerArgs &server, API::TYPE &msg);

#include "Handlers.def"
#undef X

    void fillWithBots(const HandlerArgs &server);
    void handleBotUpdates(const HandlerArgs &server);

    int connectedPlayerCount();

    API::WelcomeMsg toWelcomeMsg(const std::string &session) {
        std::vector<API::Player> players;
        for (const auto &player : this->state.players) {
            players.emplace_back(player.cards.size(), player.connected, player.name, player.premoves.size(), player.sitting_out, player.tricks);
        }
        auto id = this->getPlayerId(session);
        auto your_cards = this->state.players[id].cards;
        auto played_cards_clean = this->state.played_cards;
        // hide the legality of the cards
        for (auto &card : played_cards_clean) {
            card.card.illegal = std::nullopt;
        }
        return API::WelcomeMsg{
            .dealer = this->state.dealer,
            .id = id,
            .phase = this->state.phase,
            .played_cards = played_cards_clean,
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
    Deck deck;
    std::chrono::system_clock::time_point updated = std::chrono::system_clock::now();
    std::chrono::system_clock::time_point turn_start_time = std::chrono::system_clock::now();
    std::string turn_token;
    API::GameState state;
    bool was_persisted = false;
    // gavin said it was ok
    // coleslawski approved the PR
    // send all complaints to noreply@rollycubes.com
    bool cascading = false;
    friend GameCoordinator;
};

#endif
