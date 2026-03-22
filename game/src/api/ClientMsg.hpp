#include <cstdint>

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
     * Alternative to sending your cookie as a header.
     *
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
     * Update an individual player's state.
     *
     * Start a new game.
     *
     * Select a card to pre-move.
     *
     * Undo a premove action.
     *
     * End the hand with your ridiculous cards.
     */


    /**
     * Alternative to sending your cookie as a header.
     *
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
     * Update an individual player's state.
     *
     * Start a new game.
     *
     * Select a card to pre-move.
     *
     * Undo a premove action.
     *
     * End the hand with your ridiculous cards.
     */
    struct ClientMsg {
std::string toString() const;
void fromString(const std::string &str);
        std::optional<std::string> session;
        ClientMsgType type = static_cast<ClientMsgType>(0);
        std::optional<Card> card;
        std::optional<int64_t> id;
        std::optional<bool> alone;
        std::optional<Suit> suit;
        std::optional<int64_t> table_talk;
        std::optional<std::string> name;
        std::optional<int64_t> premoves;
        std::optional<std::vector<Card>> cards;
    };
}
