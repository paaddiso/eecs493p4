#include "globals.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    currentlySelectedLabel = 0;
    somethingOnClipboard = false;
    undoStack = new QUndoStack();
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
