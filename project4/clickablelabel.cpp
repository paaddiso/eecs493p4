#include "clickablelabel.h"
#include "boximage.h"
#include <iostream>

ClickableLabel::ClickableLabel(const QString& text,QWidget * parent):QLabel(parent)
{
    id = next_label_id++;
    this->setText(text);
    labelSize.setWidth(150);
    labelSize.setHeight(150);
    this->setFixedSize(labelSize);
}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    event = event;//prevents warnings
    emit clicked();
    std::cout << "select label " << id <<std::endl;

    if(currentlySelectedLabel)
    {
        currentlySelectedLabel->setStyleSheet("");
    }
    currentlySelectedLabel = this;
    this->setStyleSheet("border: 5px solid yellow");
    globalBoxImg_ptr->copyAct->setEnabled(true);
    globalBoxImg_ptr->cutAct->setEnabled(true);
    globalBoxImg_ptr->deleteSingleAct->setEnabled(true);
}
