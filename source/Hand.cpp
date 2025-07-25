#include "Hand.h"
#include <map>
#include <algorithm>

// Default constructor
Hand::Hand() {}

// Deal 5 cards from the deck
void Hand::dealHand(Deck& deck) {
    cards.clear();
    for (int i = 0; i < 5 && deck.cardsRemaining() > 0; ++i) {
        cards.push_back(deck.dealCard());
    }
}

// Sort cards by value (ascending)
void Hand::sortValue() {
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        return a.getValue() < b.getValue();
    });
}

// Sort cards by group frequency (higher first), then by value (higher first)
void Hand::sortGroup() {
    std::map<int, std::vector<Card>> freqMap;
    for (const Card& c : cards) {
        freqMap[c.getValue()].push_back(c);
    }

    std::vector<std::vector<Card>> grouped;
    for (auto& pair : freqMap) {
        grouped.push_back(pair.second);
    }

    std::sort(grouped.begin(), grouped.end(), [](const std::vector<Card>& a, const std::vector<Card>& b) {
        if (a.size() != b.size())
            return a.size() > b.size();
        return a[0].getValue() > b[0].getValue();
    });

    cards.clear();
    for (const auto& group : grouped) {
        for (const auto& c : group)
            cards.push_back(c);
    }
}

// Get frequency counts of card values (sorted descending)
std::vector<int> Hand::getValueCounts() const {
    std::map<int, int> counts;
    for (const Card& c : cards)
        counts[c.getValue()]++;

    std::vector<int> values;
    for (auto& entry : counts)
        values.push_back(entry.second);

    std::sort(values.rbegin(), values.rend());
    return values;
}

// Check if all cards have the same suit
bool Hand::isFlush() const {
    if (cards.empty()) return false;
    QString suit = cards[0].getSuit();
    for (const Card& c : cards) {
        if (c.getSuit() != suit)
            return false;
    }
    return true;
}

// Check if cards form a straight
bool Hand::isStraight() const {
    std::vector<int> vals;
    for (const Card& c : cards)
        vals.push_back(c.getValue());

    std::sort(vals.begin(), vals.end());
    if (vals == std::vector<int>{2, 3, 4, 5, 14}) // Special low-A straight
        return true;

    for (int i = 0; i < 4; ++i) {
        if (vals[i + 1] != vals[i] + 1)
            return false;
    }
    return true;
}

// Return hand type code as string
QString Hand::getBest() const {
    if (cards.size() != 5) return "invalid";

    bool flush = isFlush();
    bool straight = isStraight();
    std::vector<int> counts = getValueCounts();

    int high = 0;
    for (const auto& c : cards)
        high = std::max(high, c.getValue());

    if (straight && flush && high == 14) return "ryfl";  // Royal Flush
    if (straight && flush) return "stfl";                // Straight Flush
    if (counts[0] == 4) return "four";                   // Four of a Kind
    if (counts[0] == 3 && counts[1] == 2) return "full"; // Full House
    if (flush) return "flsh";                            // Flush
    if (straight) return "strt";                         // Straight
    if (counts[0] == 3) return "trio";                   // Three of a Kind
    if (counts[0] == 2 && counts[1] == 2) return "twop"; // Two Pair
    if (counts[0] == 2) return "pair";                   // One Pair
    return "high";                                       // High Card
}

// Set hand from encoded values
void Hand::setHand(const QVector<int>& cardValues) {
    cards.clear();
    for (int code : cardValues) {
        int suit = code / 100;
        int value = code % 100;
        cards.push_back(Card(value, suit));
    }
}

// Swap selected cards and return old cards to the deck
void Hand::swapCard(const QVector<int>& cardIndices, Deck& deck) {
    QVector<int> unique;
    for (int i : cardIndices) {
        if (!unique.contains(i) && i >= 0 && i < 5)
            unique.push_back(i);
    }

    std::vector<Card> toReturn;

    for (int i = 0; i < unique.size() && deck.cardsRemaining() > 0; ++i) {
        toReturn.push_back(cards[unique[i]]);
        cards[unique[i]] = deck.dealCard();
    }

    for (const Card& card : toReturn) {
        deck.insertCardRandomly(card);
    }
}

// Get current hand (const reference)
const std::vector<Card>& Hand::getCards() const {
    return cards;
}

// Get primary hand value used in comparison
int Hand::getPrimaryValue() const {
    QString best = getBest();

    std::map<int, int> valueCount;
    for (const Card& c : cards)
        valueCount[c.getValue()]++;

    std::multimap<int, int, std::greater<int>> freqToValue;
    for (const auto& [val, freq] : valueCount)
        freqToValue.insert({freq, val});

    if (best == "four" || best == "trio" || best == "pair") {
        return freqToValue.begin()->second;
    } else if (best == "twop") {
        int highestPair = 0;
        for (const auto& [freq, val] : freqToValue) {
            if (freq == 2 && val > highestPair)
                highestPair = val;
        }
        return highestPair;
    } else if (best == "full") {
        for (const auto& [freq, val] : freqToValue) {
            if (freq == 3)
                return val;
        }
    } else if (best == "strt" || best == "stfl" || best == "ryfl") {
        std::vector<int> vals;
        for (const Card& c : cards)
            vals.push_back(c.getValue());
        std::sort(vals.begin(), vals.end());
        if (vals == std::vector<int>{2, 3, 4, 5, 14})
            return 5;
        return vals.back();
    } else if (best == "flsh" || best == "high") {
        int maxVal = 0;
        for (const auto& c : cards)
            maxVal = std::max(maxVal, c.getValue());
        return maxVal;
    }

    return 0;
}

// Get secondary values for tie-breaking
std::vector<int> Hand::getSecondaryValues() const {
    std::map<int, int> valueCount;
    for (const Card& c : cards)
        valueCount[c.getValue()]++;

    int primary = getPrimaryValue();

    std::multimap<int, int, std::greater<int>> freqToValue;
    for (const auto& [val, freq] : valueCount) {
        if (val != primary) {
            freqToValue.insert({freq, val});
        }
    }

    std::vector<int> secondaryValues;
    for (const auto& [freq, val] : freqToValue) {
        for (int i = 0; i < freq; ++i)
            secondaryValues.push_back(val);
    }

    std::sort(secondaryValues.begin(), secondaryValues.end(), std::greater<>());
    return secondaryValues;
}

// Get hand rank index (lower = stronger)
int Hand::getRankIndex() const {
    QStringList ranks = {
        "ryfl", "stfl", "four", "full", "flsh", "strt",
        "trio", "twop", "pair", "high"
    };
    return ranks.indexOf(getBest());
}
