#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlabel.h>
#include "Game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);  // Constructor
    ~MainWindow();                           // Destructor

private slots:
    void on_btnStart_clicked();      // Start Game button clicked
    void on_btnNext_clicked();       // Next Round button clicked
    void on_btnSwap_clicked();       // Swap button clicked

private:
    Ui::MainWindow *ui;
    Game game;

    void updateDisplay();            // Refresh all UI elements
    void updatePlayerHandImages();   // Show player's cards and names
    void updateComputerHandImages(); // Show computer's cards and names
};

#endif // MAINWINDOW_H
