//
// Created by hualet on 11/22/16.
//

#include "errortooltip.h"

ErrorTooltip::ErrorTooltip(QString errorMessage, QWidget *parent) :
    DArrowRectangle(DArrowRectangle::ArrowTop, parent),
    m_label(new QLabel(errorMessage))
{
    m_label->setStyleSheet("padding: 5px 10px; color: #f9704f");
    m_label->adjustSize();
    setContent(m_label);
    setBackgroundColor(Qt::white);
    setBorderColor(Qt::white);
}

void ErrorTooltip::setMessage(QString message) {
    m_label->setText(message);
    m_label->adjustSize();
    setContent(m_label);
}
