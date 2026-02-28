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

using namespace API;

Game::Game() {
    this->state.phase = Phase::LOBBY;
    this->state.dealer = 0;
    this->state.trump = Suit::HEARTS;
    this->state.turn = 0;
    this->state.private_session = true;
    this->state.players.reserve(MAX_PLAYERS);
    this->state.scores = {0, 0};
    this->deck.shuffle();
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

JoinMsg Game::addPlayer(const PerSocketData &data) {
    if (state.players.size() >= MAX_PLAYERS) {
        throw GameError("lobby is full");
    }
    auto &player = state.players.emplace_back();
    player.connected = true;
    player.session = data.session;
    JoinMsg msg;
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
    state.turn = (int)(state.turn + 1) % MAX_PLAYERS;
    if (state.players[state.turn].sitting_out) {
        // womp womp
        state.turn = (int)(state.turn + 1) % MAX_PLAYERS;
    }
    turn_token = state.players[state.turn].session;
}

std::optional<DisconnectMsg> Game::disconnectPlayer(std::string id) {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = false;
            return DisconnectMsg{.id = i};
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

std::optional<ReconnectMsg> Game::reconnectPlayer(std::string id, const PerSocketData &data) {
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == id) {
            state.players[i].connected = true;
            return ReconnectMsg{.id = i};
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

inline const ServerPlayer &getPlayer(const GameState &state, const std::string &session) {
    for (const auto &player : state.players) {
        if (player.session == session) {
            return player;
        }
    }
    throw GameError({.error = "unknown player"});
}

void Game::handleMessage(const HandlerArgs &server, const std::string_view message) {
    auto data = json::parse(message);
    if (!data["type"].is_string())
        throw GameError({.error = "Type is not specified correctly"});
    // Copy a snapshot of the current game state
    GameState prev(state);
    const ServerPlayer *player = nullptr;
    player = &getPlayer(state, server.session);
    if (player == nullptr)
        throw GameError({.error = "unknown player"});

    const auto type = data["type"].get<std::string>();

#define X(name, Msg)                                \
    if (type == #name) {                            \
        Msg msg;                                    \
        msg.fromString(std::string(message));       \
        this->name(server, msg);                    \
        updated = std::chrono::system_clock::now(); \
        return;                                     \
    }
#include "Handlers.def"
#undef X

    throw GameError({.error = "Unknown action type"});
}

static void sendUpdate(const HandlerArgs &server, const GameState &state) {
    UpdateMsg msg;
    msg.phase = state.phase;
    msg.dealer = state.dealer;
    msg.turn = state.turn;
    msg.trump = state.trump;
    msg.scores = state.scores;
    server.broadcast(msg.toString());
}

void Game::order(const HandlerArgs &server, OrderMsg &msg) {
    if (state.phase != Phase::VOTE_ROUND1 && state.phase != Phase::VOTE_ROUND2)
        throw GameError({.error = "it's not voting time"});
    if (turn_token != server.session)
        throw GameError({.error = "it's not your turn"});
    if (state.phase == Phase::VOTE_ROUND1) {
        int sitting_out = -1;
        if (msg.alone && *msg.alone) {
            sitting_out = (state.turn + 2) % MAX_PLAYERS;
            state.players[sitting_out].sitting_out = true;
        }
        caller = state.turn;
        msg.id = state.turn;
        server.broadcast(msg.toString());
        if (sitting_out == state.dealer) {
            // guess we can skip discard here
            state.phase = Phase::PLAYING;
            state.trump = state.top_card->suit;
            state.turn = state.dealer;
            advanceTurn();
        } else {
            state.phase = Phase::DISCARDING;
            state.trump = state.top_card->suit;
            state.players[state.dealer].cards.push_back(*state.top_card);
        }
    } else { // ROUND 2
        if (msg.suit == std::nullopt) {
            throw GameError({.error = "which suit?"});
        }
        if (msg.suit == state.top_card->suit) {
            throw GameError({.error = "that suit was turned down"});
        }
        if (msg.alone && *msg.alone) {
            state.players[(state.turn + 2) % MAX_PLAYERS].sitting_out = true;
        }
        state.trump = *msg.suit;
        caller = state.turn;
        msg.id = state.turn;
        server.broadcast(msg.toString());
        state.phase = Phase::PLAYING;
        state.turn = state.dealer;
        advanceTurn();
    }
    sendUpdate(server, state);
}

void Game::pass(const HandlerArgs &server, PassMsg &msg) {
    if (state.phase != Phase::VOTE_ROUND1 && state.phase != Phase::VOTE_ROUND2)
        throw GameError({.error = "it's not voting time"});
    if (turn_token != server.session)
        throw GameError({.error = "it's not your turn"});
    if (state.turn == state.dealer) {
        if (state.phase == Phase::VOTE_ROUND2) {
            throw GameError({.error = "you are screwed"});
        } else {
            state.phase = Phase::VOTE_ROUND2;
        }
    }
    msg.id = state.turn;
    server.broadcast(msg.toString());
    advanceTurn();
    sendUpdate(server, state);
}

namespace API {
    bool operator==(const Card &a, const Card &b) {
        return a.suit == b.suit &&
               a.rank == b.rank;
    }
} // namespace API

int color(const Suit suit) {
    return suit == Suit::DIAMONDS || suit == Suit::HEARTS ? 0 : 1;
}

Suit effectiveSuit(const Card &card, const Suit trump) {
    if (card.rank != Rank::JACK)
        return card.suit;
    if (card.suit != trump && color(card.suit) == color(trump)) {
        return trump; // somebody call jack bauer
    }
    return card.suit;
}

void Game::endGame(const HandlerArgs &server, int winner) {
    // TODO: think about what state needs to get reset here
    WinGameMsg msg;
    msg.id = winner;
    server.broadcast(msg.toString());
    state.phase = Phase::ENDED;
}

void Game::endHand(const HandlerArgs &server) {
    // TODO: think about what state needs to get reset here
    state.played_cards.clear();
    WinHandMsg msg;
    int t0 = state.players[0].tricks + state.players[2].tricks;
    int t1 = state.players[1].tricks + state.players[3].tricks;
    int winner = t0 > t1 ? 0 : 1;
    bool alone = state.players[0 + winner].sitting_out || state.players[2 + winner].sitting_out;
    int caller_team = caller % 2;
    int mag = std::max(t0, t1);
    int winnings = 1;
    // sweep || euchre'd
    if (mag == 5 || caller_team != winner) {
        winnings = 2;
    }
    if (mag == 5 && alone) {
        winnings = 4;
    }
    msg.id = winner;
    state.scores[winner] += winnings;
    turn_token = state.players[state.turn].session;
    server.broadcast(msg.toString());
    for (auto &player : state.players) {
        player.tricks = 0;
        player.sitting_out = false;
    }
    deck.shuffle();
    if (state.scores[winner] >= 10) {
        endGame(server, winner);
    } else {
        state.phase = Phase::VOTE_ROUND1;
        state.dealer = (state.dealer + 1) % MAX_PLAYERS;
        state.turn = state.dealer;
        advanceTurn();
        dealCards(server);
    }
}

static int scoreCard(const Card &card, Suit trump, Suit led) {
    bool isBauer = card.rank == Rank::JACK && color(trump) == color(card.suit);
    if (isBauer)
        return trump == card.suit ? 1000 : 999; // right : left

    if (card.suit == trump) {
        return ((int)card.rank) * 10; // trump gets super-charged
    }

    if (card.suit == led) {
        return (int)card.rank; // in-order
    }

    return 0; // actually worthless
}

void Game::endTrick(const HandlerArgs &server) {
    WinTrickMsg msg;
    Suit led = effectiveSuit(state.trick[0], state.trump);
    int best = 0;
    int winner = trick_leader;
    int current = trick_leader;
    for (size_t i = 0; i < state.trick.size(); ++i) {
        int score = scoreCard(state.trick[i], state.trump, led);
        if (score > best) {
            best = score;
            winner = current;
        }
        do {
            current = (current + 1) % MAX_PLAYERS;
        } while (state.players[current].sitting_out);
    }
    state.trick.clear();
    msg.id = winner;
    state.turn = winner;
    state.players[winner].tricks += 1;
    turn_token = state.players[state.turn].session;
    server.broadcast(msg.toString());
    int total_tricks = 0;
    for (const auto &player : state.players) {
        total_tricks += player.tricks;
    }
    if (total_tricks == 5) {
        endHand(server);
    }
}

void Game::play_card(const HandlerArgs &server, PlayCardMsg &msg) {
    if (state.phase != Phase::PLAYING)
        throw GameError({.error = "not just yet"});
    if (turn_token != server.session)
        throw GameError({.error = "it's not your turn"});
    if (!std::erase(state.players[state.turn].cards, msg.card))
        throw GameError({.error = "you don't have that card"});
    Card server_copy = msg.card;
    msg.card.illegal = std::nullopt; // this is not up to the client
    if (state.trick.size() > 0) {
        // check what suit we need to follow
        auto suit = effectiveSuit(state.trick[0], state.trump);
        if (suit != effectiveSuit(msg.card, state.trump)) {
            // ok we played an off-card, let's see if we cheated

            for (const auto &card : state.players[state.turn].cards) {
                if (suit == effectiveSuit(card, state.trump)) {
                    // we have determined this player is definitely frank
                    server_copy.illegal = true;
                    break;
                }
            }
        }
    }
    if (state.trick.empty()) {
        trick_leader = state.turn;
    }
    state.trick.push_back(msg.card);
    state.played_cards.push_back(server_copy);

    msg.id = state.turn;
    server.broadcast(msg.toString()); // let everyone know
    size_t active_players = 0;
    for (const auto &p : state.players) {
        if (!p.sitting_out) active_players++;
    }
    if (state.trick.size() == active_players) {
        endTrick(server);
    } else {
        advanceTurn();
    }
    sendUpdate(server, state);
}

void Game::discard(const HandlerArgs &server, DiscardMsg &msg) {
    if (state.phase != Phase::DISCARDING)
        throw GameError({.error = "not right now"});
    if (server.session != state.players[state.dealer].session)
        throw GameError({.error = "you didn't even deal this lol"});
    if (msg.card == state.top_card)
        throw GameError({.error = "you just picked that up..."});
    if (!std::erase(state.players[state.dealer].cards, msg.card))
        throw GameError({.error = "you don't have that card"});
    msg.id = state.turn;
    server.broadcast(msg.toString());
    state.phase = Phase::PLAYING;
    state.turn = state.dealer;
    advanceTurn();
    sendUpdate(server, state);
}

void Game::play_jaja_ding_dong(const HandlerArgs &server, PlayJajaDingDongMsg &msg) {
    // TODO
}

void Game::table_talk(const HandlerArgs &server, TableTalkMsg &msg) {
    // TODO
};

void Game::dealCards(const HandlerArgs &server) {
    state.top_card = deck.deal(1)[0];
    auto kitty = deck.deal(3);
    state.kitty = std::vector(kitty.begin(), kitty.end());
    for (auto &player : state.players) {
        auto cards = deck.deal(5);
        player.cards = std::vector(cards.begin(), cards.end());
        DealCardsMsg hand;
        hand.your_cards = player.cards;
        hand.top_card = *state.top_card;
        server.dm(player.session, hand.toString());
    }
}

void Game::restart(const HandlerArgs &server, RestartMsg &msg) {
    if (state.players.size() < MAX_PLAYERS)
        throw GameError({.error = "not enough players"});
    if (state.phase != Phase::ENDED && state.phase != Phase::LOBBY)
        throw GameError({.error = "not in a start-able phase"});
    for (auto &player : state.players) {
        player.tricks = 0;
        player.sitting_out = false;
    }
    deck.shuffle();
    state.phase = Phase::VOTE_ROUND1;
    state.dealer = (state.dealer + 1) % MAX_PLAYERS;
    state.turn = state.dealer;
    advanceTurn();

    sendUpdate(server, state);
    this->dealCards(server);
}

void Game::update_name(const HandlerArgs &server, UpdateNameMsg &msg) {
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
