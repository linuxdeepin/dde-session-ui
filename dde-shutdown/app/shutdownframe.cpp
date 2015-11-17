#include <QDebug>

#include "backgroundlabel.h"
#include "shutdownframe.h"

ShutdownFrame::ShutdownFrame(QWidget *parent)
    : BoxFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry:" << geometry();
    setWindowFlags(Qt::FramelessWindowHint | Qt::BypassWindowManagerHint);

    m_shutdownManager = new ShutdownManager(this);
    updateScreenPosition();
    connect(m_displayInter, &DisplayInterface::PrimaryRectChanged, this, &ShutdownFrame::updateScreenPosition);
}

void ShutdownFrame::updateScreenPosition() {
    m_displayInter = new DisplayInterface(this);
    qDebug() << m_displayInter->lastError().type() << m_displayInter->isValid();
    DisplayRect rect = m_displayInter->primaryRect();
    m_shutdownManager->move(rect.x, rect.y);
    m_shutdownManager->show();
    m_shutdownManager->updateGeometry();
}

void ShutdownFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

ShutdownFrame::~ShutdownFrame()
{
    delete m_displayInter;
}
