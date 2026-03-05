#ifndef DECK_H
#define DECK_H
#include "api/API.hpp"
#include "api/Card.hpp"
#include <algorithm>
#include <cassert>
#include <random>

using namespace API;

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
        static std::mt19937 rng(std::random_device{}());
        std::shuffle(this->cards, this->cards + CARDS_IN_DECK, rng);
        this->deal_ptr = 0;
    }

    const std::vector<API::Card> deal(std::size_t n) {
        this->deal_ptr += n;
        assert(this->deal_ptr <= CARDS_IN_DECK);
        std::vector<API::Card> cards(
            this->cards + this->deal_ptr - n,
            this->cards + this->deal_ptr
        );
        return cards;
    }

  private:
    API::Card cards[CARDS_IN_DECK];
    std::size_t deal_ptr;
};

#endif
