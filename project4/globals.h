#ifndef GLOBALS_H
#define GLOBALS_H

#include <QApplication>
#include <QUndoStack>
#include <QClipboard>
#include <QPixmap>
#include <QString>
#include <QLabel>
#include <vector>

class ClickableLabel;

extern QClipboard *clipboard;// = QApplication::clipboard();
extern QString copiedPictureFilename;//work-around since there is only one clipboard
extern QUndoStack *undoStack;
extern ClickableLabel *currentlySelectedLabel;
extern bool somethingOnClipboard;
extern void showPopUp(const QString& title,const QString &text);
extern int next_label_id;

class BoxImage;
extern BoxImage* globalBoxImg_ptr;

#endif // GLOBALS_H

