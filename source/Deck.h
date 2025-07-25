#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <random>
#include <algorithm>

class Deck {
public:
    Deck();

    void createDeck();                           // Initialize a standard 52-card deck
    void shuffle();                              // Shuffle the deck randomly
    Card dealCard();                             // Deal one card from the top
    size_t cardsRemaining() const;               // Get the number of undealt cards
    void reset();                                // Reset and reshuffle the deck
    void insertCardRandomly(const Card& card);   // Insert a card at a random position

private:
    std::vector<Card> cards;     // Active deck
    std::vector<Card> discard;   // Discarded cards (optional use)
    int currentIndex;            // Index of the next card to deal
};

#endif // DECK_H

