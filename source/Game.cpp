#include "Game.h"
#include <QStringList>
#include <QMap>

// Constructor: initialize players, round counter, and state
Game::Game() : player("You"), computer("Computer"), round(0), lastRoundWinner(nullptr) {}

// Start a new game session
void Game::startGame() {
    player.resetScore();
    computer.resetScore();
    deck.reset();
    round = 0;
    lastRoundWinner = nullptr;
}

// Deal cards for the next round and determine the winner
bool Game::dealNextRound() {
    if (deck.cardsRemaining() < 10)
        return false;

    player.getHand().dealHand(deck);
    computer.getHand().dealHand(deck);
    ++round;

    isDraw = false;
    hasSwappedThisRound = false;

    // Evaluate round result
    int pRank = player.getHand().getRankIndex();
    int cRank = computer.getHand().getRankIndex();

    if (pRank < cRank) {
        player.incrementScore();
        lastRoundWinner = &player;
    } else if (pRank > cRank) {
        computer.incrementScore();
        lastRoundWinner = &computer;
    } else {
        int pPrimary = player.getHand().getPrimaryValue();
        int cPrimary = computer.getHand().getPrimaryValue();

        if (pPrimary > cPrimary) {
            player.incrementScore();
            lastRoundWinner = &player;
        } else if (cPrimary > pPrimary) {
            computer.incrementScore();
            lastRoundWinner = &computer;
        } else {
            auto pSec = player.getHand().getSecondaryValues();
            auto cSec = computer.getHand().getSecondaryValues();

            bool determined = false;
            for (int i = 0; i < std::min(pSec.size(), cSec.size()); ++i) {
                if (pSec[i] > cSec[i]) {
                    player.incrementScore();
                    lastRoundWinner = &player;
                    determined = true;
                    break;
                } else if (pSec[i] < cSec[i]) {
                    computer.incrementScore();
                    lastRoundWinner = &computer;
                    determined = true;
                    break;
                }
            }

            if (!determined) {
                isDraw = true;
                lastRoundWinner = nullptr;
            }
        }
    }

    return true;
}

// Get winner of the last round
const Player* Game::winnerOfRound() const {
    return lastRoundWinner;
}

// Get overall winner (by score)
Player const& Game::overallWinner() const {
    if (player.getScore() > computer.getScore())
        return player;
    else if (computer.getScore() > player.getScore())
        return computer;
    else
        return player; // Tie: return either
}

// Get current round number
int Game::currentRound() const {
    return round;
}

// Get player object
Player& Game::getPlayer() {
    return player;
}

// Get computer object
Player& Game::getComputer() {
    return computer;
}

// Get deck object
Deck& Game::getDeck() {
    return deck;
}

// Check if last round was a draw
bool Game::wasDraw() const {
    return isDraw;
}

// Player swaps up to 3 cards (only once per round in first 4 rounds)
void Game::playerSwapCards(const QVector<int>& indices) {
    if (round < 5 && !hasSwappedThisRound && indices.size() <= 3) {
        player.getHand().swapCard(indices, deck);
        hasSwappedThisRound = true;

        computerSwapOneCardIfNeeded();
        evaluateHands();
    }
}

// Re-evaluate both hands and update scores (after swap)
void Game::evaluateHands() {
    if (lastRoundWinner == &player)
        player.incrementScore(-1);
    else if (lastRoundWinner == &computer)
        computer.incrementScore(-1);

    int pRank = player.getHand().getRankIndex();
    int cRank = computer.getHand().getRankIndex();
    isDraw = false;

    if (pRank < cRank) {
        player.incrementScore();
        lastRoundWinner = &player;
    } else if (pRank > cRank) {
        computer.incrementScore();
        lastRoundWinner = &computer;
    } else {
        int pPrimary = player.getHand().getPrimaryValue();
        int cPrimary = computer.getHand().getPrimaryValue();

        if (pPrimary > cPrimary) {
            player.incrementScore();
            lastRoundWinner = &player;
        } else if (cPrimary > pPrimary) {
            computer.incrementScore();
            lastRoundWinner = &computer;
        } else {
            auto pSec = player.getHand().getSecondaryValues();
            auto cSec = computer.getHand().getSecondaryValues();

            bool determined = false;
            for (int i = 0; i < std::min(pSec.size(), cSec.size()); ++i) {
                if (pSec[i] > cSec[i]) {
                    player.incrementScore();
                    lastRoundWinner = &player;
                    determined = true;
                    break;
                } else if (pSec[i] < cSec[i]) {
                    computer.incrementScore();
                    lastRoundWinner = &computer;
                    determined = true;
                    break;
                }
            }

            if (!determined) {
                isDraw = true;
                lastRoundWinner = nullptr;
            }
        }
    }
}

// Enhanced computer swap logic with strategy to form flush, straight, full house, or four of a kind
void Game::computerSwapOneCardIfNeeded() {
    QString type = computer.getHand().getBest();
    const auto& cards = computer.getHand().getCards();
    int keyVal = computer.getHand().getPrimaryValue();
    int swapIndex = -1;

    QMap<int, int> valCount;
    QMap<QString, int> suitCount;
    QVector<int> values;

    for (const auto& c : cards) {
        valCount[c.getValue()]++;
        suitCount[c.getSuit()]++;
        values.push_back(c.getValue());
    }

    // Try to form a flush if one suit has 4 cards
    for (const auto& suit : suitCount.keys()) {
        if (suitCount[suit] == 4) {
            for (int i = 0; i < cards.size(); ++i) {
                if (cards[i].getSuit() != suit) {
                    swapIndex = i;
                    break;
                }
            }
            break;
        }
    }

    // Try to form a straight if we already have 4 close consecutive values
    if (swapIndex == -1) {
        std::sort(values.begin(), values.end());
        int consecutive = 1;
        for (int i = 1; i < values.size(); ++i) {
            if (values[i] == values[i - 1] + 1) {
                consecutive++;
            } else if (values[i] != values[i - 1]) {
                consecutive = 1;
            }
            if (consecutive >= 4) {
                for (int j = 0; j < cards.size(); ++j) {
                    bool partOfStraight = false;
                    for (int k = 0; k < values.size(); ++k) {
                        if (std::abs(cards[j].getValue() - values[k]) <= 1) {
                            partOfStraight = true;
                            break;
                        }
                    }
                    if (!partOfStraight) {
                        swapIndex = j;
                        break;
                    }
                }
                break;
            }
        }
    }

    // Try to form a full house if we already have a trio
    if (swapIndex == -1 && type == "trio") {
        for (int i = 0; i < cards.size(); ++i) {
            int v = cards[i].getValue();
            if (valCount[v] != 3 && valCount[v] != 2) {
                swapIndex = i;
                break;
            }
        }
    }

    // Try to form four of a kind from a trio
    if (swapIndex == -1 && type == "trio") {
        for (int i = 0; i < cards.size(); ++i) {
            if (cards[i].getValue() != keyVal) {
                swapIndex = i;
                break;
            }
        }
    }

    // Fallback to basic strategy
    if (swapIndex == -1) {
        if (type == "pair" || type == "trio") {
            for (int i = 0; i < cards.size(); ++i) {
                if (cards[i].getValue() != keyVal &&
                    (swapIndex == -1 || cards[i].getValue() < cards[swapIndex].getValue())) {
                    swapIndex = i;
                }
            }
        } else if (type == "twop") {
            for (int i = 0; i < cards.size(); ++i) {
                if (valCount[cards[i].getValue()] == 1) {
                    swapIndex = i;
                    break;
                }
            }
        } else if (type == "high") {
            for (int i = 0; i < cards.size(); ++i) {
                if (swapIndex == -1 || cards[i].getValue() < cards[swapIndex].getValue()) {
                    swapIndex = i;
                }
            }
        }
    }

    if (swapIndex != -1) {
        computer.getHand().swapCard({swapIndex}, deck);
    }
}

