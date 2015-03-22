#include "globals.h"
#include "mainwindow.h"
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

    //widget
    leftwidget = new QLabel();

    //dock
    dock = new QDockWidget(tr("Slideshow"));
    dock->setWidget(leftwidget);

    //scroll area
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(boximg);
    scrollArea->setWidgetResizable(true);

    //splitter
    QSplitter *splitter = new QSplitter();
    splitter->addWidget(dock);
    splitter->addWidget(scrollArea);
    splitter->setStretchFactor(0,0);
    splitter->setStretchFactor(1,1);

    setCentralWidget(splitter);

    //timer
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
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

    setTimerAct = new QAction(tr("&Set Timer"), this);
    setTimerAct->setStatusTip(tr("Set the timer frequency"));
    connect(setTimerAct, SIGNAL(triggered()), this, SLOT(setTimer()));

    playAct = new QAction(tr("&Play"), this);
    playAct->setStatusTip(tr("Start a slideshow!"));
    connect(playAct, SIGNAL(triggered()), this, SLOT(play()));
    playAct->setEnabled(false);

    playFromAct = new QAction(tr("&Play from Selection"), this);
    playFromAct->setStatusTip(tr("Start a slideshow from current selection!"));
    connect(playFromAct, SIGNAL(triggered()), this, SLOT(playFromSelection()));
    playFromAct->setEnabled(false);

    pauseAct = new QAction(tr("&Pause"), this);
    pauseAct->setStatusTip(tr("Pause the current show"));
    connect(pauseAct, SIGNAL(triggered()), this, SLOT(pause()));
    pauseAct->setEnabled(false);
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
    editMenu->addSeparator();
    editMenu->addAction(boximg->undoAct);
    editMenu->addAction(boximg->redoAct);
    editMenu->addSeparator();
    editMenu->addAction(boximg->cutAct);
    editMenu->addAction(boximg->copyAct);
    editMenu->addAction(boximg->pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(setTimerAct);
    editMenu->addAction(playAct);
    editMenu->addAction(playFromAct);
    editMenu->addAction(pauseAct);

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


/* update()
 *
 * this function is called periodically and is responsible for
 * managing the "slideshow" functionality.
 */
void MainWindow::update()
{
    cout << "MainWindow::update() called" << endl;
    if(currentDisplayIndex == -1)
    {
        cerr << "[CRITICAL ERROR] currentDisplayIndex is -1 in MainWindow::update()" << endl;
        exit(1);
    }
    else if(currentDisplayIndex >= (int) boximg->widgets.size()-1)
    {
        currentDisplayIndex = 0;
    }
    else
    {
        currentDisplayIndex++;
    }

    const QPixmap *pmap = boximg->getPixmapAt(currentDisplayIndex);
    leftwidget->setPixmap(*pmap);
}

/* setTimer()
 *
 * The timer should be settable from a dialog box using a QSlider
 * (not text entry). Range should be 1 to 5 seconds. Setting the
 * timer should have an undo.
 */
void MainWindow::setTimer()
{
    cout << "MainWindow::setTimer() called" << endl;
    stop_timer_if_running();

        //do stuff

    start_timer_if_show_in_progress();
}

//starts the show from a specific index
void MainWindow::startShowFromIndex(int startIndex)
{
    playAct->setEnabled(false);
    playFromAct->setEnabled(false);
    currentDisplayIndex = startIndex;
    update();
    timer->start(timeout);
    slideshowIsActive = true;
    slideshowIsPaused = false;
    pauseAct->setEnabled(true);
    return;
}

/* play() */
void MainWindow::play()
{
    cout << "MainWindow::play() called" << endl;
    if(boximg->widgets.size() > 0)
    {
        startShowFromIndex(boximg->widgets.size());
    }
    else
    {
        showPopUp("Error","Cannot play because there are no images");
    }
    return;
}

/* playFromSelection() */
void MainWindow::playFromSelection()
{
    cout << "MainWindow::playFromSelection() called" << endl;
    if(boximg->widgets.size() == 0)
    {
        showPopUp("Error","Cannot play because there are no images");
        return;
    }
    if(!currentlySelectedLabel)
    {
        showPopUp("Error","Cannot play because there is nothing selected");
        return;
    }
    int index_of_selection = boximg->getLabelIndex(currentlySelectedLabel);
    if(index_of_selection == -1)
    {
        cerr << "[CRITICAL ERROR] index_of_selection is -1 in MainWindow::playFromSelection()" << endl;
        exit(1);
    }

    //change index_of_selection to be one before itself
    if(index_of_selection == 0)
        index_of_selection = (int) boximg->widgets.size();
    else
        index_of_selection--;

    //start show
    startShowFromIndex(index_of_selection);
    return;
}

/* pause() */
void MainWindow::pause()
{
    cout << "MainWindow::pause() called" << endl;
    timer->stop();
    slideshowIsActive = false;
    slideshowIsPaused = true;
    boximg->updatePlayActionStatus();
    return;
}

