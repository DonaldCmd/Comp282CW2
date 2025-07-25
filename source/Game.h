#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Deck.h"

class Game {
public:
    Game();
    void startGame();                              // Start a new game
    bool dealNextRound();                          // Deal cards for the next round (false if <10 cards)
    const Player* winnerOfRound() const;           // Get the winner of the current round
    Player const& overallWinner() const;           // Get the overall winner so far
    int currentRound() const;                      // Get the current round number
    Player& getPlayer();                           // Get reference to the player
    Player& getComputer();                         // Get reference to the computer
    Deck& getDeck();                               // Get reference to the deck
    bool wasDraw() const;                          // Check if the round was a draw
    void playerSwapCards(const QVector<int>& indices); // Let player swap selected cards
    void evaluateHands();                          // Compare hands and decide the round winner

private:
    Player player;
    Player computer;
    Deck deck;
    int round;
    Player const* lastRoundWinner;
    bool isDraw = false;
    bool hasSwappedThisRound = false;              // Prevent multiple swaps in a round
    void computerSwapOneCardIfNeeded();            // Let computer swap one card if needed
};

#endif // GAME_H

