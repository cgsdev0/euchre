#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "Consts.h"
#include "Game.h"
#include "Loop.h"
#include "StringUtils.h"
#include <json.hpp>
using json = nlohmann::json;
using namespace std::chrono_literals;

Game::Game() {
    this->state.phase = API::Phase::LOBBY;
    this->state.dealer = 0;
    this->state.trump = API::Trump::HEARTS;
    this->state.turn = 0;
    this->state.turn = 0;
    this->state.private_session = true;
    this->state.players.reserve(MAX_PLAYERS);
}

Game::~Game() {
}

bool Game::isInitialized() { return state.players.size() > 0; }
bool Game::isPrivate() const { return state.private_session; }

bool Game::hasPlayer(std::string &id) {
    for (const auto &player : state.players) {
        if (player.session == id) {
            return true;
        }
    }
    return false;
}

int Game::getPlayerId(const std::string &id) {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            return i;
        }
    }
    return -1;
}

API::JoinMsg Game::addPlayer(const PerSocketData &data) {
    if (state.players.size() >= MAX_PLAYERS) {
        throw API::GameError("lobby is full");
    }
    auto &player = state.players.emplace_back();
    player.connected = true;
    player.session = data.session;
    API::JoinMsg msg;
    msg.id = state.players.size() - 1;

    // This is our first player joining
    if (state.players.size() == 1) {
        turn_token = data.session;
        state.turn = 0;
    }
    return msg;
}

void Game::advanceTurn() {
    this->turn_start_time = std::chrono::system_clock::now();
    if (state.players.size() == 1) {
        turn_token = state.players[0].session;
        state.turn = 0;
    } else {
        state.turn = (int)(state.turn + 1) % state.players.size();
        turn_token = state.players[state.turn].session;
    }
}

std::optional<API::DisconnectMsg> Game::disconnectPlayer(std::string id) {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = false;
            return API::DisconnectMsg{.id = i};
        }
    }
    return std::nullopt;
}

bool Game::isPlayerConnected(std::string id) const {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            return state.players[i].connected;
        }
    }
    return false;
}

std::optional<API::ReconnectMsg> Game::reconnectPlayer(std::string id, const PerSocketData &data) {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = true;
            return API::ReconnectMsg{.id = i};
        }
    }
    return std::nullopt;
}

int Game::connectedPlayerCount() {
    int count = 0;
    for (const auto &player : state.players) {
        if (player.connected)
            ++count;
    }
    return count;
}

inline const API::ServerPlayer &getPlayer(const API::GameState &state, const std::string &session) {
    for (const auto &player : state.players) {
        if (player.session == session) {
            return player;
        }
    }
    throw API::GameError({.error = "unknown player"});
}

void Game::handleMessage(const HandlerArgs &server, const std::string_view message) {
    auto data = json::parse(message);
    if (!data["type"].is_string())
        throw API::GameError({.error = "Type is not specified correctly"});
    // Copy a snapshot of the current game state
    API::GameState prev(state);
    const API::ServerPlayer *player = nullptr;
    player = &getPlayer(state, server.session);
    if (player == nullptr)
        throw API::GameError({.error = "unknown player"});

    const auto type = data["type"].get<std::string>();

#define X(name, Msg)                                \
    if (type == #name) {                            \
        API::Msg msg;                               \
        msg.fromString(std::string(message));       \
        this->name(server, msg);                    \
        updated = std::chrono::system_clock::now(); \
        return;                                     \
    }
#include "Handlers.def"
#undef X

    throw API::GameError({.error = "Unknown action type"});
}

void Game::order(const HandlerArgs &server, const API::OrderMsg &msg) {
}
void Game::pass(const HandlerArgs &server, const API::PassMsg &msg) {
}
void Game::play_card(const HandlerArgs &server, const API::PlayCardMsg &msg) {
}
void Game::discard(const HandlerArgs &server, const API::DiscardMsg &msg) {
}
void Game::play_jaja_ding_dong(const HandlerArgs &server, const API::PlayJajaDingDongMsg &msg) {
}

void Game::table_talk(const HandlerArgs &server, const API::TableTalkMsg &msg) {
};

void Game::restart(const HandlerArgs &server, const API::RestartMsg &msg) {
    if (state.players.empty())
        throw API::GameError({.error = "uhhh this should never happen"});
    if (state.phase != API::Phase::ENDED)
        throw API::GameError({.error = "game still in progress"});
}

void Game::update_name(const HandlerArgs &server, const API::UpdateNameMsg &msg) {
    std::string name = trimString(msg.name, MAX_PLAYER_NAME);
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == server.session) {
            state.players[i].name = std::optional<std::string>(name);
            auto res = msg;
            res.id = i;
            server.broadcast(res.toString());
            return;
        }
    }
}
