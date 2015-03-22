#include "globals.h"
#include <QMessageBox>
#include <QString>
#include <iostream>

QTimer *timer;
QDockWidget *dock;
QAction *setTimerAct;
QAction *playAct;
QAction *playFromAct;
QAction *pauseAct;
int timeout;
int currentDisplayIndex;
bool slideshowIsActive;
bool slideshowIsPaused;

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
    cout <<"call globals.cpp/stop_timer_if_running()"<< endl;
    if(timer->isActive())
        timer->stop();
    return;
}

void start_timer_if_show_in_progress()
{
    cout <<"call globals.cpp/start_timer_if_show_in_progress()"<< endl;
    if(slideshowIsActive and !timer->isActive() and !slideshowIsPaused)
        timer->start(timeout);
    return;
}

void endShow()
{
    cout <<"call globals.cpp/endShow()"<< endl;
    stop_timer_if_running();
    currentDisplayIndex = -1;
    slideshowIsActive = false;
    slideshowIsPaused = false;
    playAct->setEnabled(false);
    playFromAct->setEnabled(false);
    pauseAct->setEnabled(false);
}
