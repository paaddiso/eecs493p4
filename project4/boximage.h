#ifndef BOXIMAGE_H
#define BOXIMAGE_H

#include <QWidget>

#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QScrollArea>
#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QGridLayout>
#include <QClipboard>
#include <QString>
#include <QStringList>
#include <QPixmap>

#include <vector>

#include "clickablelabel.h"

class BoxImage : public QWidget
{
    Q_OBJECT

    public:
        explicit BoxImage(QWidget *parent = 0);
        ~BoxImage();

    signals:

    public slots:
        void addImage(void);
        void addCollection(void);
        void deleteAll(void);
        void deleteImage(void);
        void undo(void);
        void redo(void);
        void cut(void);
        void copy(void);
        void paste(void);

    public:
        QAction *imgAct;
        QAction *colAct;
        QAction *deleteAct;
        QAction *deleteSingleAct;
        QAction *undoAct;
        QAction *redoAct;
        QAction *cutAct;
        QAction *copyAct;
        QAction *pasteAct;

    private:
        QScrollArea *scrollArea;
        QGridLayout *grid;
        QLabel *imageLabel;

        void createActions(void);
        void shiftPixmapsUpAndInsert(QPixmap &pixmap,int index,QString &filename);
        void shiftPixmapsDown(int index);
        void removeWidgetFromGrid(int index);
        void addPictureFile(QString &filename);
        void addRawPictureFile(QString &filename,int index);
        void selectMultipleFiles(void);
        void clearSelection(void);
        void clearWidgets(void);
        void updateUndoRedoActions(void);
        bool checkPictureFile(QString &filename);
        int getPasteIndex(void);

    public:
        std::vector<ClickableLabel*> widgets;
        void addVectorOfCheckedFiles(std::vector<QString> filenames,int index);
        void addWidgetToGrid(QPixmap &raw_pixmap, QPixmap &pixmap,int index,QString &filename);
        void removeRange(int index,int range);
        int getLabelIndex(ClickableLabel*);
        const QPixmap* getPixmapAt(int index);
        void updatePlayActionStatus();
};

#endif // BOXIMAGE_H
