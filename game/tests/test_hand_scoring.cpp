#include <iostream>
#include <optional>
#include <vector>
#include <algorithm>
#include <cstdint>

#include "Bot.hpp"
#include "api/Rank.hpp"
#include "api/Suit.hpp"
#include "api/Phase.hpp"

using namespace Bot;
using namespace API;

static int score_card(Card card, Suit trump) {
    int color = (trump == Suit::DIAMONDS || trump == Suit::HEARTS) ? 0 : 1;
    int card_color = (card.suit == Suit::DIAMONDS || card.suit == Suit::HEARTS) ? 0 : 1;
    
    if (card_color == color) {
        if (card.rank == Rank::JACK) {
            return card.suit == trump ? 16 : 15;
        } else {
            return (int)card.rank + 9;
        }
    } else {
        return (int)card.rank;
    }
}

static int score_hand_for_suit(std::vector<Card> hand, Suit trump) {
    int score = 0;
    for (size_t i = 0; i < hand.size(); i++) {
        score += score_card(hand[i], trump);
    }
    return score;
}

std::string rank_str(Rank r) {
    switch(r) {
        case Rank::NINE: return "9";
        case Rank::TEN: return "T";
        case Rank::JACK: return "J";
        case Rank::QUEEN: return "Q";
        case Rank::KING: return "K";
        case Rank::ACE: return "A";
    }
    return "?";
}

std::string suit_str(Suit s) {
    switch(s) {
        case Suit::DIAMONDS: return "D";
        case Suit::CLUBS: return "C";
        case Suit::HEARTS: return "H";
        case Suit::SPADES: return "S";
    }
    return "?";
}

std::string hand_str(const std::vector<Card>& hand) {
    std::string s;
    for (const auto& c : hand) {
        s += rank_str(c.rank) + suit_str(c.suit) + " ";
    }
    return s;
}

Card make_card(Rank r, Suit s) {
    return Card{.illegal = std::nullopt, .rank = r, .suit = s};
}

struct Stats {
    int min_score = 999;
    int max_score = 0;
    int count_orderable = 0;
    int count_go_alone = 0;
};

int main() {
    std::vector<Card> deck;
    for (int si = 0; si < 4; si++) {
        Suit s = (Suit)si;
        for (int ri = 0; ri < 6; ri++) {
            Rank r = (Rank)ri;
            deck.push_back(make_card(r, s));
        }
    }
    
    std::vector<Card> hand(5);
    
    std::cout << "\n=== ALL EUCHRE HANDS SCORED FOR EACH TRUMP ===\n";
    std::cout << "NOTE: Scores > 20 would order trump. Scores > 35 would go alone.\n\n";
    
    auto print_hand = [&](Suit trump_suit) {
        std::cout << hand_str(hand) << "| "
                  << suit_str(trump_suit) << ": " 
                  << score_hand_for_suit(hand, trump_suit) << "\n";
    };
    
    int hand_count = 0;
    for (int i0 = 0; i0 < 24; i0++) {
    for (int i1 = i0 + 1; i1 < 24; i1++) {
    for (int i2 = i1 + 1; i2 < 24; i2++) {
    for (int i3 = i2 + 1; i3 < 24; i3++) {
    for (int i4 = i3 + 1; i4 < 24; i4++) {
        hand[0] = deck[i0];
        hand[1] = deck[i1];
        hand[2] = deck[i2];
        hand[3] = deck[i3];
        hand[4] = deck[i4];
        
        hand_count++;
        
        for (int ti = 0; ti < 4; ti++) {
            print_hand((Suit)ti);
        }
    }}}}}
    
    std::cout << "\nTotal hands enumerated: " << hand_count << "\n";
    
    std::cout << "\n\n=== SUMMARY STATS PER TRUMP SUIT ===\n\n";
    
    Stats stats_by_trump[4] = {};
    
    for (int i0 = 0; i0 < 24; i0++) {
    for (int i1 = i0 + 1; i1 < 24; i1++) {
    for (int i2 = i1 + 1; i2 < 24; i2++) {
    for (int i3 = i2 + 1; i3 < 24; i3++) {
    for (int i4 = i3 + 1; i4 < 24; i4++) {
        hand[0] = deck[i0];
        hand[1] = deck[i1];
        hand[2] = deck[i2];
        hand[3] = deck[i3];
        hand[4] = deck[i4];
        
        for (int ti = 0; ti < 4; ti++) {
            int score = score_hand_for_suit(hand, (Suit)ti);
            stats_by_trump[ti].min_score = std::min(stats_by_trump[ti].min_score, score);
            stats_by_trump[ti].max_score = std::max(stats_by_trump[ti].max_score, score);
            if (score > 20) stats_by_trump[ti].count_orderable++;
            if (score > 35) stats_by_trump[ti].count_go_alone++;
        }
    }}}}}
    
    for (int ti = 0; ti < 4; ti++) {
        std::cout << suit_str((Suit)ti) << ": "
                  << "min=" << stats_by_trump[ti].min_score << " "
                  << "max=" << stats_by_trump[ti].max_score << " "
                  << "orderable=" << stats_by_trump[ti].count_orderable << " "
                  << "go_alone=" << stats_by_trump[ti].count_go_alone << "\n";
    }
    
    std::cout << "\nTotal possible hands (24 choose 5) = 42504\n";
    std::cout << "Each hand scored for 4 trump suits = 170016 scores\n";
    
    return 0;
}
