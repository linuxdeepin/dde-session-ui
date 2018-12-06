#include "lockpasswordwidget.h"

#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <DHiDPIHelper>

DWIDGET_USE_NAMESPACE

LockPasswordWidget::LockPasswordWidget(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    QLabel *lockLbl = new QLabel(this);
    lockLbl->setPixmap(DHiDPIHelper::loadNxPixmap(":/img/action_icons/unlock_normal.svg"));
    layout->addSpacing(5);
    layout->addWidget(lockLbl);
    layout->addStretch();
    setLayout(layout);
}

void LockPasswordWidget::setMessage(const QString &message)
{
    m_message = message;

    update();
}

void LockPasswordWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::HighQualityAntialiasing);

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);
    painter.setOpacity(0.1);
    painter.drawRoundedRect(rect(), 5, 5);

    QTextOption option;
    option.setAlignment(Qt::AlignCenter);
    painter.setOpacity(1);
    painter.setPen(Qt::white);
    painter.drawText(rect(), m_message, option);
}
