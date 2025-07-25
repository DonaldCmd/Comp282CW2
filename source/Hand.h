#ifndef HAND_H
#define HAND_H

#include "Card.h"
#include "Deck.h"
#include <QVector>
#include <QString>
#include <vector>

class Hand {
public:
    Hand();

    void dealHand(Deck& deck);                     // Deal 5 cards from the deck
    void sortValue();                              // Sort cards by value (ascending)
    void sortGroup();                              // Sort cards by value frequency (e.g., pairs first)

    QString getBest() const;                       // Get hand type (e.g., "four", "flsh")
    int getPrimaryValue() const;                   // Get main value for comparison (e.g., pair/triple)
    std::vector<int> getSecondaryValues() const;   // Get kicker values for tie-breaking
    int getRankIndex() const;                      // Get rank index (lower = stronger hand)

    void setHand(const QVector<int>& cardValues);  // Set hand using encoded card values (e.g., 412 = 12 of Spades)

    void swapCard(const QVector<int>& cardIndices, Deck& deck); // Swap selected cards from deck
    const std::vector<Card>& getCards() const;     // Get all cards in hand

private:
    std::vector<Card> cards;

    bool isFlush() const;                          // Check if all cards share the same suit
    bool isStraight() const;                       // Check if card values are consecutive
    std::vector<int> getValueCounts() const;       // Get count of each card value in hand
};

#endif // HAND_H
