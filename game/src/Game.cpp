#include "Game.h"
#include "Consts.h"
#include <algorithm>
#include <json.hpp>

#define STRING_UTILS_IMPLEMENTATION
#include "StringUtils.h"

#include "Bot.hpp"

using json = nlohmann::json;
using namespace std::chrono_literals;

using namespace API;
using namespace Bot;

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
        handleBotUpdates(server);                   \
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

static bool premoveAvailable(const GameState &state) {
    size_t total_cards = state.players[state.turn].cards.size() + state.players[state.turn].premoves.size();
    return !state.players[state.turn].premoves.empty() && total_cards > 1;
}

void Game::cascadePremoves(const HandlerArgs &server) {
    while (premoveAvailable(state)) {
        cascading = true;
        PlayCardMsg msg;
        msg.card = *state.players[state.turn].premoves.erase(state.players[state.turn].premoves.begin());
        play_card(server, msg);
        cascading = false;
    }
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
        state.caller = state.turn;
        msg.id = state.turn;
        server.broadcast(msg.toString());
        if (sitting_out == state.dealer) {
            // guess we can skip discard here
            state.played_cards.clear();
            state.phase = Phase::PLAYING;
            state.trump = state.top_card->suit;
            state.turn = state.dealer;
            advanceTurn();
        } else {
            state.phase = Phase::DISCARDING;
            state.turn = state.dealer;
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
        state.caller = state.turn;
        msg.id = state.turn;
        server.broadcast(msg.toString());
        state.played_cards.clear();
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
    WinHandMsg msg;
    int t0 = state.players[0].tricks + state.players[2].tricks;
    int t1 = state.players[1].tricks + state.players[3].tricks;
    int winner = t0 > t1 ? 0 : 1;
    bool alone = state.players[0 + winner].sitting_out || state.players[2 + winner].sitting_out;
    int caller_team = state.caller % 2;
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
    // why was this here?
    // turn_token = state.players[state.turn].session;
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

static bool isLeft(const Card &card, Suit trump) {
    bool isBauer = card.rank == Rank::JACK && color(trump) == color(card.suit);
    return isBauer && trump != card.suit;
}

static bool isRight(const Card &card, Suit trump) {
    return card.rank == Rank::JACK && trump == card.suit;
}

static bool isTrumpAce(const Card &card, Suit trump) {
    return card.rank == Rank::ACE && trump == card.suit;
}

static int scoreCard(const Card &card, Suit trump, Suit led) {
    bool isBauer = card.rank == Rank::JACK && color(trump) == color(card.suit);
    if (isBauer)
        return trump == card.suit ? 1000 : 999; // right : left

    if (card.suit == trump) {
        return (((int)card.rank) + 1) * 10; // trump gets super-charged
    }

    if (card.suit == led) {
        return ((int)card.rank) + 1; // in-order
    }

    return 0; // actually worthless
}

void Game::endTrick(const HandlerArgs &server) {
    WinTrickMsg msg;
    Suit led = effectiveSuit(state.trick[0], state.trump);
    int best = 0;
    int winner = state.trick_leader;
    int current = state.trick_leader;
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
    if (total_tricks == 4) {
        LastCardMsg last;

        size_t active_players = 0;
        for (const auto &p : state.players) {
            if (!p.sitting_out) active_players++;
        }
        while (state.trick.size() < active_players) {
            auto &player = state.players[state.turn];
            Card card;
            if (player.cards.size()) {
                card = *player.cards.erase(player.cards.begin());
            } else {
                card = *player.premoves.erase(player.premoves.begin());
            }
            Card server_copy = card;
            card.illegal = std::nullopt; // this is not up to the client
            if (state.trick.size() > 0) {
                // check what suit we need to follow
                auto suit = effectiveSuit(state.trick[0], state.trump);
                if (suit != effectiveSuit(card, state.trump)) {
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
                state.trick_leader = state.turn;
            }
            state.trick.push_back(card);
            auto tagged = TaggedCard{.card = server_copy, .id = state.turn};
            state.played_cards.push_back(tagged);
            last.cards.push_back(tagged);
            advanceTurn();
        }

        server.broadcast(last.toString());
        endTrick(server);
    } else if (total_tricks == 5) {
        endHand(server);
    }
}

void Game::lay_down(const HandlerArgs &server, LayDownMsg &msg) {
    if (state.phase != Phase::PLAYING)
        throw GameError({.error = "not just yet"});
    if (turn_token != server.session)
        throw GameError({.error = "it's not your turn"});
    size_t total_cards = state.players[state.turn].cards.size();
    if (total_cards != 5)
        throw GameError({.error = "you gotta do that at the beginnin"});
    size_t active_players = 0;
    for (const auto &p : state.players) {
        if (!p.sitting_out) active_players++;
    }
    size_t cards_played = state.trick.size();
    bool ez_mode = (cards_played == 0 || cards_played == active_players - 1);

    bool hasLeft = false;
    bool hasRight = false;
    bool hasAce = false;
    for (const auto &card : state.players[state.turn].cards) {
        if (effectiveSuit(card, state.trump) != state.trump) {
            throw GameError({.error = "not enough trump!"});
        }
        if (isRight(card, state.trump)) hasRight = true;
        if (isLeft(card, state.trump)) hasLeft = true;
        if (isTrumpAce(card, state.trump)) hasAce = true;
    }

    if (!hasRight || !hasLeft) {
        throw GameError({.error = "need both bauers"});
    }
    if (!hasAce && !ez_mode) {
        throw GameError({.error = "need the ace, no ez mode for u"});
    }

    msg.cards = state.players[state.turn].cards;
    server.broadcast(msg.toString());

    WinHandMsg msg2;
    int winner = state.turn % 2;
    int partner = (state.turn + 2) % 4;
    int winnings = 2;
    if (state.players[partner].sitting_out) {
        winnings = 4;
    }
    msg2.id = winner;
    deck.shuffle();
    for (auto &player : state.players) {
        player.tricks = 0;
        player.sitting_out = false;
    }
    server.broadcast(msg2.toString());
    state.scores[winner] += winnings;
    if (state.scores[winner] >= 10) {
        endGame(server, winner);
    } else {
        state.phase = Phase::VOTE_ROUND1;
        state.dealer = (state.dealer + 1) % MAX_PLAYERS;
        state.turn = state.dealer;
        advanceTurn();
        dealCards(server);
    }
    sendUpdate(server, state);
}

void Game::play_card(const HandlerArgs &server, PlayCardMsg &msg) {
    if (state.phase != Phase::PLAYING)
        throw GameError({.error = "not just yet"});
    if (!cascading && turn_token != server.session)
        throw GameError({.error = "it's not your turn"});
    size_t total_cards = state.players[state.turn].cards.size() + state.players[state.turn].premoves.size();
    if (!cascading && total_cards == 1)
        throw GameError({.error = "nah nah the server can do that for you"});
    if (!cascading && !std::erase(state.players[state.turn].cards, msg.card))
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
        state.trick_leader = state.turn;
    }
    state.trick.push_back(msg.card);
    state.played_cards.push_back(TaggedCard{.card = server_copy, .id = state.turn});

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
    if (!cascading) {
        cascadePremoves(server);
    }
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
    ServerDiscardMsg new_msg;
    new_msg.id = state.turn;
    server.broadcast(new_msg.toString());
    state.played_cards.clear();
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
    if (state.players.size() < MAX_PLAYERS) // add bot players to fill up the game
        fillWithBots(server);
    if (state.phase != Phase::ENDED && state.phase != Phase::LOBBY)
        throw GameError({.error = "not in a start-able phase"});
    for (auto &player : state.players) {
        player.tricks = 0;
        player.sitting_out = false;
    }
    if (state.phase == Phase::ENDED) {
        state.scores = {0, 0};
    }
    deck.shuffle();
    state.phase = Phase::VOTE_ROUND1;
    state.dealer = (state.dealer + 1) % MAX_PLAYERS;
    state.turn = state.dealer;
    advanceTurn();

    this->dealCards(server);

    sendUpdate(server, state);
}

void Game::undo_premove(const HandlerArgs &server, UndoPremoveMsg &msg) {
    if (state.phase != Phase::PLAYING)
        throw GameError({.error = "you can't do that right now"});
    // undo all premoves
    ServerPlayer *player = nullptr;
    size_t i = 0;
    for (i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == server.session) {
            player = &state.players[i];
        }
    }
    if (player == nullptr)
        throw GameError({.error = "unknown player"});
    if (player->premoves.empty()) return;
    for (auto &card : player->premoves) {
        player->cards.push_back(card);
    }
    player->premoves.clear();
    PlayerUpdateMsg res;
    res.premoves = 0;
    res.id = i;
    server.broadcast(res.toString());
}

void Game::premove(const HandlerArgs &server, PremoveMsg &msg) {
    if (state.phase != Phase::PLAYING)
        throw GameError({.error = "you can't premove right now"});
    if (turn_token == server.session) {
        // just play the card
        PlayCardMsg play_msg;
        play_msg.card = msg.card;
        this->play_card(server, play_msg);
    } else {
        // premove
        ServerPlayer *player = nullptr;
        size_t i = 0;
        for (i = 0; i < state.players.size(); ++i) {
            if (state.players[i].session == server.session) {
                player = &state.players[i];
            }
        }
        if (player == nullptr)
            throw GameError({.error = "unknown player"});
        if (!std::erase(player->cards, msg.card))
            throw GameError({.error = "you don't have that card"});
        player->premoves.push_back(msg.card);
        PlayerUpdateMsg res;
        res.premoves = player->premoves.size();
        res.id = i;
        server.broadcast(res.toString());
    }
}

void Game::update_name(const HandlerArgs &server, UpdateNameMsg &msg) {
    std::string name = trimString(msg.name, MAX_PLAYER_NAME);
    for (uint i = 0; i < state.players.size(); ++i) {
        if (state.players[i].session == server.session) {
            state.players[i].name = std::optional<std::string>(name);
            PlayerUpdateMsg res;
            res.name = name;
            res.id = i;
            server.broadcast(res.toString());
            return;
        }
    }
}

void Game::fillWithBots(const HandlerArgs &server) {
    int bot_number = 1;
    while (state.players.size() < MAX_PLAYERS) {
        auto &player = state.players.emplace_back();
        int curr = bot_number++;
        player.connected = false;
        player.session = "bot:" + std::to_string(curr);
        player.name = std::optional("Bot " + std::to_string(curr));
        player.bot_enum = BotName::BOT_NONE;
    }
}

void Game::handleBotUpdates(const HandlerArgs &server) {
    while (state.players[state.turn].bot_enum == BotName::BOT_NONE) {
        auto &player = state.players[state.turn];

        BotDecisionState decision_state = {
            .id = state.turn,
            .name  = (BotName)player.bot_enum,
            .phase = state.phase,
            .trump = state.trump,
            .hand  = player.cards,
            .stack = state.played_cards,
            .top_card = state.top_card,
        };

        // NOTE Each handler funciton is designed to call `advanceTurn()` itself, so we don't need
        // to do that here.

        switch (state.phase) {
            case Phase::DISCARDING: {
                Card card = Bot::discard(decision_state);
                DiscardMsg msg = { .card = card, .id = std::nullopt };
                discard(server, msg);
                break;
            }

            case Phase::LOBBY:
            case Phase::ENDED: {
                assert(false); // WARN bots probably don't need to do anything here...
                break;
            }

            case Phase::PLAYING: {
                Card card = Bot::playCard(decision_state);
                PlayCardMsg msg = { .card = card, .id = std::nullopt };
                play_card(server, msg);
                break;
            }

            case Phase::VOTE_ROUND1:
            case Phase::VOTE_ROUND2: {
                bool go_alone = false;
                std::optional<Suit> suit = std::nullopt;
                if (orderTrump(decision_state, suit, go_alone)) {
                    OrderMsg msg = { .alone = go_alone, .id = std::nullopt, .suit = suit };
                    order(server, msg);
                } else {
                    PassMsg msg = { .id = std::nullopt };
                    pass(server, msg);
                }
                break;
            }
        }
    }
}
