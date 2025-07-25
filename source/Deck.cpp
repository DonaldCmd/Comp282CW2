#include "Deck.h"
#include <QRandomGenerator>

// Constructor: initialize and shuffle a full deck
Deck::Deck() {
    createDeck();
    shuffle();
}

// Create a full 52-card deck (values 2–14, suits 1–4)
void Deck::createDeck() {
    cards.clear();
    for (int value = 2; value <= 14; ++value) {
        for (int suit = 1; suit <= 4; ++suit) {
            cards.emplace_back(value, suit);
        }
    }
    currentIndex = 0;
}

// Shuffle the deck and reset the current index
void Deck::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cards.begin(), cards.end(), g);
    currentIndex = 0;
}

// Deal one card from the deck; return default card if empty
Card Deck::dealCard() {
    if (currentIndex < static_cast<int>(cards.size()))
        return cards[currentIndex++];
    else
        return Card(); // Return default card (2 of Clubs) if deck is empty
}

// Get number of undealt cards remaining
size_t Deck::cardsRemaining() const {
    return cards.size() - currentIndex;
}

// Reset the deck to a full shuffled set
void Deck::reset() {
    createDeck();
    shuffle();
}

// Insert a card at a random position after currentIndex
void Deck::insertCardRandomly(const Card& card) {
    if (cards.empty()) {
        cards.push_back(card);
        return;
    }

    // Insert between currentIndex and end (to avoid duplicates in dealt cards)
    int pos = QRandomGenerator::global()->bounded(currentIndex, static_cast<int>(cards.size()) + 1);
    cards.insert(cards.begin() + pos, card);
}
