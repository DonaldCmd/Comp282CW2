#include "mainwindow.h"
#include <QApplication>

// Entry point of the application
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  // Initialize Qt application
    MainWindow w;                // Create main window instance
    w.show();                    // Show the main window
    return a.exec();             // Start the event loop
}
// Qt version: Qt 6.9.0 (MinGW 13.1.0 64-bit).
// Qt Creator version: Qt Creator 16.0.1 (Community).
