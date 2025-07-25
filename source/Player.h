#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include <QString>

class Player {
public:
    Player();
    Player(const QString& name);

    void resetScore();                     // Set score to 0
    void incrementScore();                 // Increase score by 1
    void incrementScore(int delta);        // Adjust score by a specific amount

    QString getName() const;               // Get player's name
    void setName(const QString& newName);  // Set player's name

    int getScore() const;                  // Get current score
    Hand& getHand();                       // Access player's hand

private:
    QString name;   // Player name
    int score;      // Player score
    Hand hand;      // Player's hand of cards
};

#endif // PLAYER_H
