#include "globals.h"
#include <QMessageBox>
#include <QString>
#include <iostream>

class BoxImage;
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
