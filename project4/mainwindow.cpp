#include "globals.h"
#include "mainwindow.h"
#include "actionbutton.h"
#include "boximage.h"

#include <QtGui>
#include <QAction>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QSplitter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include <iostream>

/* http://stackoverflow.com/questions/13111669/yes-no-message-box-using-qmessagebox
 * Online Qt Image Viewer Example
 * Online Qt Menu Example
*/

using namespace std;

//ctor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    //new BoxImage instance
    boximg = new BoxImage();
    globalBoxImg_ptr = boximg;

    //set up
    createActions();
    createMenus();
    createStatusBar();

    //Buttons
    ActionButton *addCollectionButton = new ActionButton();
    addCollectionButton->setAction(boximg->colAct);
    addCollectionButton->setText("Add Collection");
    addCollectionButton->setMinimumWidth(150);
    addCollectionButton->setMinimumHeight(70);

    ActionButton *addImageButton = new ActionButton();
    addImageButton->setAction(boximg->imgAct);
    addImageButton->setText("Add Image");
    addImageButton->setMinimumWidth(150);
    addImageButton->setMinimumHeight(70);

    ActionButton *deleteAllButton = new ActionButton();
    deleteAllButton->setAction(boximg->deleteAct);
    deleteAllButton->setText("Delete All Images");
    deleteAllButton->setMinimumWidth(150);
    deleteAllButton->setMinimumHeight(70);

    ActionButton *deleteButton = new ActionButton();
    deleteButton->setAction(boximg->deleteSingleAct);
    deleteButton->setText("Delete Image");
    deleteButton->setMinimumWidth(150);
    deleteButton->setMinimumHeight(70);

    //layout
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setAlignment(Qt::AlignTop);
    vLayout->setSpacing(5);
    vLayout->addWidget(addCollectionButton);
    vLayout->addWidget(addImageButton);
    vLayout->addWidget(deleteAllButton);
    vLayout->addWidget(deleteButton);

    //widget
    QWidget *controls = new QWidget();
    controls->setLayout(vLayout);

    //scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(boximg);
    scrollArea->setWidgetResizable(true);

    //splitter
    QSplitter *splitter = new QSplitter();
    splitter->addWidget(controls);
    splitter->addWidget(scrollArea);
    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);

    setCentralWidget(splitter);

 //   setUnifiedTitleAndToolBarOnMac(true);
}

//dtor
MainWindow::~MainWindow()
{

}

void MainWindow::about()
{
   QMessageBox::about(this, tr("About Application"),
            tr("This is an image viewer. It will <b>display</b> images"
               " and not do much else."));
}

void MainWindow::invalidMessage()
{
    cout << "MainWindow::invalidMessage() called" << endl;
    showPopUp("Invalid Command","This feature has not been implemented");
    return;
}

void MainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(invalidMessage()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(invalidMessage()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(invalidMessage()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(invalidMessage()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    invAct = new QAction(tr("Invalid &Action"), this);
    connect(invAct, SIGNAL(triggered()), this, SLOT(invalidMessage()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(boximg->imgAct);
    editMenu->addAction(boximg->colAct);
    editMenu->addAction(boximg->deleteAct);
    editMenu->addAction(boximg->deleteSingleAct);
    editMenu->addAction(boximg->undoAct);
    editMenu->addAction(boximg->redoAct);
    editMenu->addAction(boximg->cutAct);
    editMenu->addAction(boximg->copyAct);
    editMenu->addAction(boximg->pasteAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
}


void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
