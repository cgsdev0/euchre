#include <algorithm>
#include <iostream>
#include <unordered_map>

#include "Consts.h"
#include "Game.h"
#include "Loop.h"
#include "StringUtils.h"
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

std::string Game::hostName() const {
    if (state.players.size() < 1)
        return "unknown";
    if (state.players[0].name == std::nullopt)
        return "unknown";
    return *state.players[0].name;
}

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

json Game::addPlayer(const PerSocketData &data) {
    json result;
    if (state.players.size() >= MAX_PLAYERS) {
        return result;
    }
    auto &player = state.players.emplace_back();
    player.connected = true;
    player.session = data.session;
    result["type"] = "join";
    result["id"] = state.players.size() - 1;

    // This is our first player joining
    if (state.players.size() == 1) {
        turn_token = data.session;
        state.turn = 0;
    }
    return result;
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

json Game::disconnectPlayer(std::string id) {
    json result;
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = false;
            result["type"] = "disconnect";
            result["id"] = i;
            break;
        }
    }
    return result;
}

bool Game::isPlayerConnected(std::string id) const {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            return state.players[i].connected;
        }
    }
    return false;
}

json Game::reconnectPlayer(std::string id, std::string old_id, const PerSocketData &data) {
    json result;
    bool has_old_id = (id != old_id);
    for (uint i = 0; i < state.players.size(); ++i) {
        if (has_old_id && state.players[i].session == old_id) {
            // Update our session to the new id
            state.players[i].session = id;
            if (state.turn == i) {
                turn_token = id;
            }
        }
        if (state.players[i].session == id) {
            state.players[i].connected = true;
            result["type"] = "reconnect";
            result["id"] = i;
            break;
        }
    }
    return result;
}

int Game::connectedPlayerCount() {
    int count = 0;
    for (const auto &player : state.players) {
        if (player.connected)
            ++count;
    }
    return count;
}

#define ACTION(x) \
    {             \
        #x, std::mem_fn(&Game::x)}

static const std::unordered_map<
    std::string, std::function<void(Game *, SendFunc, HandlerArgs, json &,
                                    const std::string &)>>
    action_map = {ACTION(chat), ACTION(leave),
                  ACTION(restart), ACTION(update_name)};

#undef ACTION

inline const API::ServerPlayer &getPlayer(const API::GameState &state, const std::string &session) {
    for (const auto &player : state.players) {
        if (player.session == session) {
            return player;
        }
    }
    throw API::GameError({.error = "unknown player"});
}

void Game::handleMessage(HANDLER_ARGS) {
    if (!data["type"].is_string())
        throw API::GameError({.error = "Type is not specified correctly"});
    // Copy a snapshot of the current game state
    API::GameState prev(state);
    const API::ServerPlayer *player = nullptr;
    player = &getPlayer(state, session);
    if (player == nullptr)
        throw API::GameError({.error = "unknown player"});
    auto action_type = data["type"].get<std::string>();
    auto it = action_map.find(action_type);
    if (it != action_map.end()) {
        it->second(this, broadcast, server, data, session);
    } else {
        throw API::GameError({.error = "Unknown action type"});
    }
    updated = std::chrono::system_clock::now();
}

void Game::chat(HANDLER_ARGS) {
    json res;

    if (!data["msg"].is_string())
        throw API::GameError({.error = "Message must be a string"});
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session) {
            res["type"] = data["type"];
            auto msg = data["msg"].get<std::string>();
            std::string name;
            if (state.players[i].name)
                name = *state.players[i].name;
            if (name == "") {
                name = "User" + std::to_string(i + 1);
            }

            auto msg2 = msg;
            if (msg.length() > MAX_CHAT_LEN) {
                msg2 = trimString(msg, MAX_CHAT_LEN, true);
            }
            RichTextStream stream;
            stream << state.players[i] << ": " << msg2;
            broadcast(log_rich_chat(stream));
            return;
        }
    }
};

void Game::leave(HANDLER_ARGS) {
    json res;
    res["type"] = "leave";
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session) {
            res["id"] = i;
            if (turn_token == state.players[i].session) {
                advanceTurn();
                json turn;
                turn["type"] = "update_turn";
                turn["id"] = state.turn;
                broadcast(turn.dump());
            }
            state.players.erase(state.players.begin() + i);
            broadcast(res.dump());
            break;
        }
    }
}

void Game::restart(HANDLER_ARGS) {
    if (state.players.empty())
        throw API::GameError({.error = "uhhh this should never happen"});
    if (state.phase != API::Phase::ENDED)
        throw API::GameError({.error = "game still in progress"});
}

void Game::update_name(HANDLER_ARGS) {
    if (!data["name"].is_string())
        throw API::GameError({.error = "name must be a string"});
    std::string name = trimString(data["name"].get<std::string>(), MAX_PLAYER_NAME);
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == session) {
            state.players[i].name = std::optional<std::string>(name);
            json msg;
            msg["type"] = "update_name";
            if (state.players[i].name != std::nullopt) {
                msg["name"] = *state.players[i].name;
            }
            msg["id"] = i;
            broadcast(msg.dump());
            return;
        }
    }
}
