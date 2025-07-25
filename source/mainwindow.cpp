#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMap>
#include <QString>

// Convert hand type code to readable name
QString prettifyCategory(const QString& code) {
    static QMap<QString, QString> map = {
        {"ryfl", "Royal Flush"},
        {"stfl", "Straight Flush"},
        {"four", "Four of a Kind"},
        {"full", "Full House"},
        {"flsh", "Flush"},
        {"strt", "Straight"},
        {"trio", "Three of a Kind"},
        {"twop", "Two Pair"},
        {"pair", "One Pair"},
        {"high", "High Card"},
        {"invalid", "Invalid"}
    };
    return map.value(code, code);
}

// Constructor: setup UI and initial button state
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->btnNext->setEnabled(false);
}

// Destructor: cleanup
MainWindow::~MainWindow() {
    delete ui;
}

// Start Game button clicked
void MainWindow::on_btnStart_clicked() {
    game.startGame();

    if (!game.dealNextRound()) {
        ui->labelResult->setText("Deck has fewer than 10 cards — cannot deal.");
        ui->btnNext->setEnabled(false);
        return;
    }

    ui->btnNext->setEnabled(true);
    ui->btnNext->setText("NEXT ROUND");

    updateDisplay();
}

// Next Round / Finish Game button clicked
void MainWindow::on_btnNext_clicked() {
    if (ui->btnNext->text() == "FINISH GAME") {
        ui->labelResult->setText("Game Over. " + game.overallWinner().getName() + " wins! Click 'START' to play again.");
        ui->btnNext->setEnabled(false);
        return;
    }

    if (!game.dealNextRound()) {
        ui->labelResult->setText("Deck has fewer than 10 cards — press FINISH GAME.");
        ui->btnNext->setText("FINISH GAME");
        return;
    }

    if (game.wasDraw()) {
        ui->labelResult->setText("Draw!");
    } else {
        ui->labelResult->setText("Round Winner: " + game.winnerOfRound()->getName());
    }

    updateDisplay();
}

// Swap button clicked: player swaps selected cards
void MainWindow::on_btnSwap_clicked() {
    QVector<int> indices;
    if (ui->checkCard1->isChecked()) indices.append(0);
    if (ui->checkCard2->isChecked()) indices.append(1);
    if (ui->checkCard3->isChecked()) indices.append(2);
    if (ui->checkCard4->isChecked()) indices.append(3);
    if (ui->checkCard5->isChecked()) indices.append(4);

    if (indices.isEmpty()) return;

    game.playerSwapCards(indices);
    game.evaluateHands();
    updateDisplay();

    // Uncheck all boxes
    ui->checkCard1->setChecked(false);
    ui->checkCard2->setChecked(false);
    ui->checkCard3->setChecked(false);
    ui->checkCard4->setChecked(false);
    ui->checkCard5->setChecked(false);
}

// Update all UI displays
void MainWindow::updateDisplay() {
    QString playerCategory = prettifyCategory(game.getPlayer().getHand().getBest());
    QString computerCategory = prettifyCategory(game.getComputer().getHand().getBest());

    ui->labelPlayerCategory->setText("Your Hand: " + playerCategory);
    ui->labelComputerCategory->setText("Computer Hand: " + computerCategory);

    ui->labelScore->setText(
        QString("Round %1 | You: %2   Computer: %3")
            .arg(game.currentRound())
            .arg(game.getPlayer().getScore())
            .arg(game.getComputer().getScore())
        );

    if (game.wasDraw() || game.winnerOfRound() == nullptr) {
        ui->labelResult->setText("Round Result: Draw");
    } else {
        ui->labelResult->setText("Round Winner: " + game.winnerOfRound()->getName());
    }

    updatePlayerHandImages();
    updateComputerHandImages();
}

// Show player's cards and labels
void MainWindow::updatePlayerHandImages() {
    const auto& cards = game.getPlayer().getHand().getCards();

    QVector<QLabel*> imageLabels = {
        ui->labelCard1, ui->labelCard2, ui->labelCard3,
        ui->labelCard4, ui->labelCard5
    };
    QVector<QLabel*> nameLabels = {
        ui->labelCard1Name, ui->labelCard2Name, ui->labelCard3Name,
        ui->labelCard4Name, ui->labelCard5Name
    };

    for (int i = 0; i < cards.size(); ++i) {
        QPixmap pix(cards[i].getImagePath());
        imageLabels[i]->setPixmap(pix.scaled(100, 140, Qt::KeepAspectRatio));
        nameLabels[i]->setText(cards[i].getName());
    }
}

// Show computer's cards and labels
void MainWindow::updateComputerHandImages() {
    const auto& cards = game.getComputer().getHand().getCards();

    QVector<QLabel*> imageLabels = {
        ui->labelCompCard1, ui->labelCompCard2, ui->labelCompCard3,
        ui->labelCompCard4, ui->labelCompCard5
    };
    QVector<QLabel*> nameLabels = {
        ui->labelCompCard1Name, ui->labelCompCard2Name, ui->labelCompCard3Name,
        ui->labelCompCard4Name, ui->labelCompCard5Name
    };

    for (int i = 0; i < cards.size(); ++i) {
        QPixmap pix(cards[i].getImagePath());
        imageLabels[i]->setPixmap(pix.scaled(100, 140, Qt::KeepAspectRatio));
        nameLabels[i]->setText(cards[i].getName());
    }
}
