#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include "globals.h"
#include <QString>
#include <QPixmap>
#include <QLabel>
#include <QSize>

class ClickableLabel : public QLabel
{
    public:
        ClickableLabel();
        ~ClickableLabel();
        QSize labelSize;

    Q_OBJECT

    public:
       explicit ClickableLabel(const QString& text="", QWidget* parent=0);
       int id;
       QString filename;
       QPixmap internalPixmap;

    signals:
        void clicked();

    protected:
        void mousePressEvent(QMouseEvent *event);

};

#endif // CLICKABLELABEL_H
