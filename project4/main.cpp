#include "globals.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    currentlySelectedLabel = 0;
    somethingOnClipboard = false;
    timeout = 3000;
    currentDisplayIndex = -1;
    slideshowIsActive = false;
    slideshowIsPaused = false;
    undoStack = new QUndoStack();
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
