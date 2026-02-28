#ifndef DECK_H
#define DECK_H
#include "api/API.hpp"
#include <cassert>

#define CARDS_IN_DECK 24
struct Deck {
    Deck() {
        for (std::size_t i = 0; i < CARDS_IN_DECK; ++i) {
            this->cards[i].suit = (API::Suit)(i / 6);
            this->cards[i].rank = (API::Rank)(i % 6);
        }
        shuffle();
    }

    void shuffle() {
        // TODO: actually shuffle
        this->deal_ptr = 0;
    }

    const std::span<API::Card> deal(std::size_t n) {
        this->deal_ptr += n;
        assert(this->deal_ptr <= CARDS_IN_DECK);
        return std::span<API::Card>(this->cards + this->deal_ptr - n, n);
    }

  private:
    API::Card cards[CARDS_IN_DECK];
    std::size_t deal_ptr;
};

#endif
