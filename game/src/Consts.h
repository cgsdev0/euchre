#ifndef INCLUDE_CONSTS_H
#define INCLUDE_CONSTS_H

#include <chrono>
#include <set>
#include <string>

using string = std::string;
typedef unsigned int uint;

static const uint MAX_PLAYERS = 4;
static const uint MAX_CHAT_LOG = 100;
static const uint SESSION_BYTES = 16;
static const uint ROOM_LEN = 6;
static const uint DEFAULT_PORT = 3001;
static const auto EVICT_AFTER = std::chrono::minutes(10);
static const uint EVICTION_LIMIT = 10;
static const uint MAX_PLAYER_NAME = 25;
static const uint MAX_CHAT_LEN = 402 + MAX_PLAYER_NAME;

/* ws->getUserData returns one of these */
struct PerSocketData {
    string session;
    string room;
    string display_name;
    bool dedupe_conns;
};

#endif
