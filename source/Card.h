#ifndef CARD_H
#define CARD_H

#include <QString>

class Card {
public:
    Card();
    Card(int value, int suit);  // suit: 1=Clubs, 2=Diamonds, 3=Hearts, 4=Spades

    int getValue() const;            // Returns value: 2–14 (J=11, Q=12, K=13, A=14)
    QString getSuit() const;         // Returns suit as string, e.g., "Hearts"
    QString getName() const;         // Returns card name, e.g., "King of Spades"
    int getNumber() const;           // Returns encoded number, e.g., 209 = 9 of Diamonds
    QString getImagePath() const;    // Returns image file path

private:
    int value; // Card value: 2–14
    int suit;  // Card suit: 1=Clubs, 2=Diamonds, 3=Hearts, 4=Spades
};

#endif // CARD_H






