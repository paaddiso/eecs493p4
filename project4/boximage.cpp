#include "globals.h"
#include "boximage.h"
#include "commands.h"
#include "clickablelabel.h"

#include <QtGui>
#include <QDialog>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QtWidgets>
#include <QMessageBox>
#include <QGridLayout>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>

#include <iostream>
#include <cmath>

using namespace std;

bool checkPictureFile(QString &filename);
void removeRange(int index,int range);

BoxImage::BoxImage(QWidget *parent) : QWidget(parent)
{
    //ACTIONS
    createActions();

    //LAYOUT
    grid = new QGridLayout();
    grid->setVerticalSpacing(0);
    grid->setHorizontalSpacing(0);
    this->setLayout(grid);

}

BoxImage::~BoxImage()
{

}

void BoxImage::addImage(void)
{
    stop_timer_if_running();
    QString filename = QFileDialog::getOpenFileName(this,
                                    tr("Open File"),
                                    QDir::homePath());
    if(checkPictureFile(filename))
    {
        std::vector<QString> filenames;
        filenames.push_back(filename);
        QUndoCommand *addCommand = new AddCommand(filenames,widgets.size());
        undoStack->push(addCommand);
        updateUndoRedoActions();
    }
    start_timer_if_show_in_progress();
    return;
}

void BoxImage::addCollection(void)
{
    stop_timer_if_running();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::homePath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    QDir directory(dir);
    QStringList nameFilter;
    nameFilter << "*.jpg" << "*.png" << "*.xpm" << "*.gif" << "*.bmp";
    QStringList files = directory.entryList(nameFilter);
    std::vector<QString> filenames;

    //check all files in directory and push to a vector if valid
    for (int i = 0; i < files.size(); ++i)
    {
        //cout << files.at(i).toLocal8Bit().constData() << endl;
        QString str = directory.absoluteFilePath(files.at(i));
        if(checkPictureFile(str))
            filenames.push_back(str);
    }

    //if there are valid files, add them
    if(filenames.size() > 0)
    {
        QUndoCommand *addCommand = new AddCommand(filenames,widgets.size());
        undoStack->push(addCommand);
        updateUndoRedoActions();
    }
    start_timer_if_show_in_progress();
    return;
}

void BoxImage::deleteAll(void)
{
    endShow();
    //std::cout << "call BoxImage::deleteAll()" << std::endl;
    int size = (int) widgets.size();
    if(size > 0)
    {
        std::vector<QString> filenames;
        for(int i=0; i<size; i++)
        {
            filenames.push_back(widgets[i]->filename);
        }
        QUndoCommand *removeCommand = new RemoveCommand(filenames,0);
        undoStack->push(removeCommand);
        updateUndoRedoActions();
        clearWidgets();
        clearSelection();
    }
    return;
}

void BoxImage::clearWidgets(void)
{
    for(unsigned i=0; i<widgets.size(); i++)
        delete widgets[i];
    widgets.clear();
    return;
}

void BoxImage::deleteImage(void)
{
    //cout << "call BoxImage::deleteImage()" << endl;
    stop_timer_if_running();
    if(currentlySelectedLabel)
    {
            cout << "getting index of currentlySelectedLabel...";

        int index = getLabelIndex(currentlySelectedLabel);

            cout << "index = " << index << endl;

        if(index != -1)
        {
            std::vector<QString> filenames;
            QString filename = widgets[index]->filename;
            filenames.push_back(filename);
            QUndoCommand *removeCommand = new RemoveCommand(filenames,index);
            undoStack->push(removeCommand);
            updateUndoRedoActions();
        }
    }
    else
    {
       cout <<"cannot delete because nothing is selected"<<endl;
    }
    clearSelection();
    if(widgets.size()==0)
       endShow();
    start_timer_if_show_in_progress();
}

//delete single image
void BoxImage::cut(void)
{
    //cout << "call BoxImage::cut()" << endl;
    if(currentlySelectedLabel)
    {
        copy();//copy image to clipboard before cutting
        deleteImage();
    }
    else
    {
       showPopUp("Friendly Message","Cannot remove image because there is no image selected.");
    }
    clearSelection();
}

//returns the vector-index of the ClickableLabel with the id
//matching the parameter's id; used to find index of selected
//label; returns -1 if no match is found
int BoxImage::getLabelIndex(ClickableLabel* cLable_ptr)
{
    if(!cLable_ptr)
        return -1;
    int target_id = cLable_ptr->id;
    int size = (int) widgets.size();
    for(int i=0; i<size; i++)
    {
        int other_id = widgets[i]->id;
        if(target_id == other_id)
            return i;
    }
    return -1;
}

void BoxImage::createActions(void)
{
    imgAct = new QAction(tr("&Open an image"), this);
    //imgAct->setShortcut(tr("Ctrl+O"));
    imgAct->setStatusTip(tr("upload an image"));
    connect(imgAct, SIGNAL(triggered()), this, SLOT(addImage()));

    colAct = new QAction(tr("&Open a collection"), this);
    //colAct->setShortcut(tr("Ctrl+Q"));
    colAct->setStatusTip(tr("upload a collection"));
    connect(colAct, SIGNAL(triggered()), this, SLOT(addCollection()));

    deleteAct = new QAction(tr("&Remove all images"), this);
    //deleteAct->setShortcut(tr("Ctrl+D"));
    deleteAct->setEnabled(false);
    deleteAct->setStatusTip(tr("Remove all images"));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteAll()));

    deleteSingleAct = new QAction(tr("&Delete Image"), this);
    //deleteSingleAct->setShortcut(tr("Ctrl+X"));
    deleteSingleAct->setEnabled(false);
    deleteSingleAct->setStatusTip(tr("remove an image"));
    connect(deleteSingleAct, SIGNAL(triggered()), this, SLOT(deleteImage()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcut(tr("Ctrl+Z"));
    undoAct->setEnabled(false);
    undoAct->setStatusTip(tr("Undo previous change"));
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undo()));

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcut(tr("Ctrl+Y"));
    redoAct->setEnabled(false);
    redoAct->setStatusTip(tr("Redo previous change"));
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redo()));

    cutAct = new QAction(tr("&Cut"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setEnabled(false);
    cutAct->setStatusTip(tr("copy image to clipboard and remove it from grid"));
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setEnabled(false);
    copyAct->setStatusTip(tr("copy an image"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    pasteAct = new QAction(tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setEnabled(false);
    pasteAct->setStatusTip(tr("paste an image"));
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(paste()));
}

void BoxImage::addPictureFile(QString &filename)
{
    if (!filename.isEmpty())
    {
        //check image
        QImage image(filename);

        if (image.isNull())
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Warning", tr("Cannot load %1. Would you like to quit the application?").arg(filename),
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::Yes) {
              qDebug() << "Yes was clicked";
              endShow();
              exit(0);
            } else {
              qDebug() << "No was clicked";
              return;
            }
        }

        //add to grid
        addRawPictureFile(filename,widgets.size());
    }
    return;
}

//return true if picture is valid
bool BoxImage::checkPictureFile(QString &filename)
{
    if (filename.isEmpty())
    {
        return false;
    }

    QImage image(filename);
    if (image.isNull())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", tr("Cannot load %1. Would you like to quit the application?").arg(filename),
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
          qDebug() << "Yes was clicked";
          exit(0);
        } else {
          qDebug() << "No was clicked";
          return false;
        }
    }

    return true;

}

void BoxImage::addVectorOfCheckedFiles(std::vector<QString> filenames,int index)
{
    int size = (int) filenames.size();
    for (int i = 0; i < size; ++i)
    {
        QString filename = filenames[i];
        addRawPictureFile(filename,index);
        index++;
    }
    updatePlayActionStatus();
    return;
}

void BoxImage::addRawPictureFile(QString &filename,int index)
{
    QImage image1(filename);
    QImage image2 = image1.scaled(150,150,Qt::KeepAspectRatio);
    QPixmap pmap1 = QPixmap::fromImage(image1);
    QPixmap pmap2 = QPixmap::fromImage(image2);
    addWidgetToGrid(pmap1,pmap2,index,filename);
}

void BoxImage::addWidgetToGrid(QPixmap &raw_pixmap, QPixmap &pixmap,int index,QString &filename)
{
    //cout << "call BoxImage::addWidgetToGrid(QPixmap &pixmap,index=" << index << ",filename=" << filename.toLocal8Bit().constData() << ")" << endl;
    //always create new label and add to back of grid
    ClickableLabel *label = new ClickableLabel("");
    label->setAlignment((Qt::AlignCenter));
    int size = (int) widgets.size();
    int row = floor(size/5);
    int col = fmod(size,5);
    grid->addWidget(label,row,col);
    widgets.push_back(label);

    //handle pixmap
    if(index == size)
    {
        label->setPixmap(pixmap);
        label->internalPixmap = raw_pixmap;
        label->filename = filename;
    }
    else
    {
        shiftPixmapsUpAndInsert(raw_pixmap,index,filename);
    }

    //activate "delete all" action
    deleteAct->setEnabled(true);

    return;
}

void BoxImage::shiftPixmapsUpAndInsert(QPixmap &newpixmap,int index,QString &filename)
{
    //std::cout << "call BoxImage::shiftPixmapsUpAndInsert(index="<< index << ",filename=" << filename.toLocal8Bit().constData() << ")" << endl;
    for(int i=widgets.size()-1; i>index; i--)
    {
        const QPixmap *pm = widgets[i-1]->pixmap();
        widgets[i]->setPixmap(*pm);
        widgets[i]->internalPixmap = widgets[i-1]->internalPixmap;
        widgets[i]->filename = widgets[i-1]->filename;
    }
    widgets[index]->setPixmap(newpixmap.scaled(150,150,Qt::KeepAspectRatio));
    widgets[index]->internalPixmap = newpixmap;
    widgets[index]->filename = filename;
    return;
}

void BoxImage::removeRange(int index,int range)
{
    //std::cout << "call BoxImage::removeRange("<< index <<","<< range <<")" << std::endl;
    for(int i=0; i<range; i++)
    {
        removeWidgetFromGrid(index);
    }

    if(widgets.size()==0)
       deleteAct->setEnabled(false);

    if(getLabelIndex(currentlySelectedLabel) == -1)
        clearSelection();

    return;
}

void BoxImage::removeWidgetFromGrid(int index)
{
    //std::cout << "call BoxImage::removeWidgetFromGrid(" << index << ")" << endl;
    int size = (int) widgets.size();
    if(index != size-1)
        shiftPixmapsDown(index);
    if(size > 0)
    {
        delete widgets.back();//remove from grid
        widgets.pop_back();//remove from vector
    }
    return;
}

void BoxImage::shiftPixmapsDown(int index)
{
    //std::cout << "call BoxImage::shiftPixmapsDown(index=" << index << ")" << endl;
    int size = (int) widgets.size();
    for(int i=index; i<size-1; i++)
    {
        const QPixmap *pm = widgets[i+1]->pixmap();
        if(pm)
            widgets[i]->setPixmap(*pm);
        widgets[i]->internalPixmap = widgets[i+1]->internalPixmap;
        widgets[i]->filename = widgets[i+1]->filename;
    }
    return;
}

//deprecated
 void BoxImage::selectMultipleFiles(void)
 {
     QStringList files = QFileDialog::getOpenFileNames(this,
                                                       tr("select one or more files"),
                                                       QDir::homePath(),
                                                       "images (#.png *.xpm *.jpg)");
     QStringList::iterator it = files.begin();
     while(it != files.end())
     {
         addPictureFile(*it);
         ++it;
     }
     return;
 }

 void BoxImage::undo(void)
 {
    stop_timer_if_running();
    undoStack->undo();
    updateUndoRedoActions();
    start_timer_if_show_in_progress();
 }
 void BoxImage::redo(void)
 {
    stop_timer_if_running();
    undoStack->redo();
    updateUndoRedoActions();
    start_timer_if_show_in_progress();
 }

 void BoxImage::copy(void)
 {
     //cout << "call BoxImage::copy()" << endl;
     if(currentlySelectedLabel)
     {
         const QPixmap *pmap = currentlySelectedLabel->pixmap();
         clipboard->setPixmap(*pmap,QClipboard::Clipboard);
         copiedPictureFilename = currentlySelectedLabel->filename;
         somethingOnClipboard = true;
         pasteAct->setEnabled(true);
     }
     return;
 }

 void BoxImage::paste(void)
 {
    //cout << "call BoxImage::paste()" << endl;
    stop_timer_if_running();
    if(somethingOnClipboard)
    {
        if(getLabelIndex(currentlySelectedLabel) == -1)
            showPopUp("Warning","No image is selected. The picture will be pasted to the end of the grid.");
        int index = getPasteIndex();
        std::vector<QString> filenames;
        filenames.push_back(copiedPictureFilename);
        QUndoCommand *addCommand = new AddCommand(filenames,index);
        undoStack->push(addCommand);
        updateUndoRedoActions();
    }
    start_timer_if_show_in_progress();
 }

 int BoxImage::getPasteIndex(void)
 {
     //cout << "call BoxImage::getPasteIndex()" << endl;
     int index = -1;
     if(currentlySelectedLabel)
     {
         index = getLabelIndex(currentlySelectedLabel);
     }
     if(index == -1)
      index = (int) widgets.size();
     else
      index = index+1;
     return index;
 }

void BoxImage::clearSelection(void)
{
    //cout <<"call BoxImage::clearSelection()"<< endl;
    if(currentlySelectedLabel)
    {
      currentlySelectedLabel->setStyleSheet("");
    }
    currentlySelectedLabel = 0;
    copyAct->setEnabled(false);
    cutAct->setEnabled(false);
    deleteSingleAct->setEnabled(false);
    playFromAct->setEnabled(false);
}

void BoxImage::updateUndoRedoActions(void)
{
    //check undo
    if(undoStack->canUndo())
        undoAct->setEnabled(true);
    else
        undoAct->setEnabled(false);

    //check redo
    if(undoStack->canRedo())
        redoAct->setEnabled(true);
    else
        redoAct->setEnabled(false);
}

const QPixmap* BoxImage::getPixmapAt(int index)
{
    return &(widgets[index]->internalPixmap);
}

void BoxImage::updatePlayActionStatus()
{
    if(widgets.size()> 0 and !slideshowIsActive)
    {
        playAct->setEnabled(true);
        if(currentlySelectedLabel){playFromAct->setEnabled(true);}
        else{playFromAct->setEnabled(false);}
    }
    else if (widgets.size()> 0 and slideshowIsActive and slideshowIsPaused)
    {
        playAct->setEnabled(true);
        if(currentlySelectedLabel){playFromAct->setEnabled(true);}
        else{playFromAct->setEnabled(false);}
    }
    else
    {
        playAct->setEnabled(false);
        playFromAct->setEnabled(false);
    }
    return;
}
