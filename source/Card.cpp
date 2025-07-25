#include "Card.h"

// Default constructor: 2 of Clubs
Card::Card() : value(2), suit(1) {}

// Constructor with specific value and suit
Card::Card(int value, int suit) : value(value), suit(suit) {}

// Get card value (2–14)
int Card::getValue() const {
    return value;
}

// Get suit as a string
QString Card::getSuit() const {
    switch (suit) {
    case 1: return "Clubs";
    case 2: return "Diamonds";
    case 3: return "Hearts";
    case 4: return "Spades";
    default: return "Unknown";
    }
}

// Get full card name, e.g., "King of Spades"
QString Card::getName() const {
    QString valueStr;
    switch (value) {
    case 11: valueStr = "Jack"; break;
    case 12: valueStr = "Queen"; break;
    case 13: valueStr = "King"; break;
    case 14: valueStr = "Ace"; break;
    default: valueStr = QString::number(value);
    }

    return valueStr + " of " + getSuit();
}

// Get encoded number, e.g., 209 = 9 of Diamonds
int Card::getNumber() const {
    return suit * 100 + value;
}

// Get image file path, e.g., "images/king_of_spades.png"
QString Card::getImagePath() const {
    QString valueStr;
    switch (value) {
    case 11: valueStr = "jack"; break;
    case 12: valueStr = "queen"; break;
    case 13: valueStr = "king"; break;
    case 14:
    case 1:  valueStr = "ace"; break;
    default: valueStr = QString::number(value);
    }

    QString suitStr = getSuit().toLower();  // Make sure this returns "hearts", "clubs", etc.

    // Full path becomes ":/images/cards/queen_of_clubs.png"
    return QString(":/cards/images/%1_of_%2.png").arg(valueStr, suitStr);
}
