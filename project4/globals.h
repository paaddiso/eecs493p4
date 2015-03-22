#ifndef GLOBALS_H
#define GLOBALS_H

#include <QApplication>
#include <QDockWidget>
#include <QUndoStack>
#include <QClipboard>
#include <QPixmap>
#include <QString>
#include <QAction>
#include <QLabel>
#include <QTimer>
#include <vector>


class ClickableLabel;
class BoxImage;

extern QTimer *timer;
extern QDockWidget *dock;
extern QAction *setTimerAct;
extern QAction *playAct;
extern QAction *playFromAct;
extern QAction *pauseAct;
extern int timeout;
extern int currentDisplayIndex;
extern bool slideshowIsActive;
extern bool slideshowIsPaused;
extern void stop_timer_if_running();
extern void start_timer_if_show_in_progress();
extern void endShow();


extern QClipboard *clipboard;// = QApplication::clipboard();
extern QString copiedPictureFilename;//work-around since there is only one clipboard
extern QUndoStack *undoStack;
extern ClickableLabel *currentlySelectedLabel;
extern bool somethingOnClipboard;
extern void showPopUp(const QString& title,const QString &text);
extern int next_label_id;
extern BoxImage* globalBoxImg_ptr;

#endif // GLOBALS_H

