#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "boximage.h"
#include <QLabel>
#include <QMainWindow>

class QAction;
class QMenu;
class QPlainTextEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void about();
    void invalidMessage();
    void update();
    void setTimer();
    void play();
    void playFromSelection();
    void stop();

private:

    //Left-widget pointer
    QLabel *leftwidget;

    //BoxImage pointer
    BoxImage *boximg;

    void loadImage(const QString &fileName);
    void loadCollection(const QString &directoryName);

    void createActions();
    void createMenus();
    void createStatusBar();
    QString strippedName(const QString &fullFileName);

    QPlainTextEdit *textEdit;
    QString curFile;

    //MENUS
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    //Actions
    QAction *exitAct;
    QAction *openAct;
    QAction *newAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *aboutAct;
    QAction *invAct;

    //helper functions
    void startShowFromIndex(int startIndex);

};

#endif // MAINWINDOW_H
