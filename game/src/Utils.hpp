#ifndef UTILS_H
#define UTILS_H

#include "api/API.hpp"

int color(const Suit suit);
bool isLeft(const Card &card, Suit trump);
bool isRight(const Card &card, Suit trump);
bool isTrumpAce(const Card &card, Suit trump);
int scoreCard(const Card &card, Suit trump, Suit led);

#ifdef UTILS_IMPLEMENTATION

int color(const Suit suit) {
    return suit == Suit::DIAMONDS || suit == Suit::HEARTS ? 0 : 1;
}

bool isLeft(const Card &card, Suit trump) {
    bool isBauer = card.rank == Rank::JACK && color(trump) == color(card.suit);
    return isBauer && trump != card.suit;
}

bool isRight(const Card &card, Suit trump) {
    return card.rank == Rank::JACK && trump == card.suit;
}

bool isTrumpAce(const Card &card, Suit trump) {
    return card.rank == Rank::ACE && trump == card.suit;
}

int scoreCard(const Card &card, Suit trump, Suit led) {
    bool isBauer = card.rank == Rank::JACK && color(trump) == color(card.suit);
    if (isBauer)
        return trump == card.suit ? 50 : 49; // right : left

    if (card.suit == trump) {
        return (((int)card.rank) + 1) + 25; // trump gets super-charged
    }

    if (card.suit == led) {
        return ((int)card.rank) + 1; // in-order
    }

    return 0; // actually worthless
}

#endif // UTILS_IMPLEMENTATION

#endif // UTILS_H
