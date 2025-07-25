#include "Player.h"

// Default constructor: name = "Player", score = 0
Player::Player() : name("Player"), score(0) {}

// Constructor with specified name
Player::Player(const QString& name) : name(name), score(0) {}

// Reset score to 0
void Player::resetScore() {
    score = 0;
}

// Increase score by 1
void Player::incrementScore() {
    ++score;
}

// Get player name
QString Player::getName() const {
    return name;
}

// Get current score
int Player::getScore() const {
    return score;
}

// Get reference to player's hand
Hand& Player::getHand() {
    return hand;
}

// Set player name
void Player::setName(const QString& newName) {
    name = newName;
}

// Adjust score by given value (score cannot go below 0)
void Player::incrementScore(int delta) {
    score += delta;
    if (score < 0) score = 0;
}
