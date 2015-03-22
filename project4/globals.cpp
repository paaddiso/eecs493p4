#include "globals.h"
#include <QMessageBox>
#include <QString>
#include <iostream>

QTimer *timer;
QDockWidget *dock;
QAction *setTimerAct;
QAction *playAct;
QAction *playFromAct;
QAction *stopAct;
int timeout;
int currentDisplayIndex;
bool slideshowIsActive;

BoxImage* globalBoxImg_ptr;
QClipboard *clipboard = QApplication::clipboard();
QString copiedPictureFilename;
QUndoStack *undoStack;
ClickableLabel *currentlySelectedLabel;
bool somethingOnClipboard;
int next_label_id = 0;

using namespace std;

void showPopUp(const QString& title,const QString &text)
{
    cout << "showPopUp() called" << endl;
    QMessageBox msgBox;
    msgBox.setModal(true);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.exec();
    return;
}

void stop_timer_if_running()
{
    if(timer->isActive())
        timer->stop();
    return;
}

void start_timer_if_show_in_progress()
{
    if(slideshowIsActive and !timer->isActive())
        timer->start(timeout);
    return;
}

void endShow()
{
    cout <<"call globals.cpp/endShow()"<< endl;
    stop_timer_if_running();
    currentDisplayIndex = -1;
    slideshowIsActive = false;
}
