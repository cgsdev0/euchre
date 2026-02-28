
#pragma once

#include <optional>
#include <variant>

#include "Card.hpp"

namespace API {
    enum class ClientMsgType : int;
    enum class Suit : int;
}

namespace API {
    /**
     * Dealer chooses a card to discard.
     *
     * Choose a card to play for the current trick.
     *
     * Pass during the voting rounds.
     *
     * Order up a particular suit as trump.
     *
     * Use the quick-chat feature.
     *
     * Self-explanatory.
     *
     * Change your display name.
     *
     * Start a new game.
     */


    /**
     * Dealer chooses a card to discard.
     *
     * Choose a card to play for the current trick.
     *
     * Pass during the voting rounds.
     *
     * Order up a particular suit as trump.
     *
     * Use the quick-chat feature.
     *
     * Self-explanatory.
     *
     * Change your display name.
     *
     * Start a new game.
     */
    struct ClientMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<Card> card;
        int64_t id;
        ClientMsgType type = static_cast<ClientMsgType>(0);
        std::optional<Suit> suit;
        std::optional<int64_t> table_talk;
        std::optional<std::string> name;
    };
}
