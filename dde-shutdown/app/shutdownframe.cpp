#include <QDebug>

#include "../background/backgroundlabel.h"
#include "shutdownframe.h"

ShutdownFrame::ShutdownFrame(QWidget *parent)
    : BoxFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry:" << geometry();

    m_shutdownManager = new ShutdownManager(this);
    QPoint mousePoint = QCursor::pos();

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        if (rect.contains(mousePoint)) {
            m_shutdownManager->setFixedSize(rect.size());
            m_shutdownManager->move(rect.x(), rect.y());
            qDebug() << "shutdownManager:" << m_shutdownManager->geometry();
            updateScreenPosition();
            continue;
        }
    }
}

void ShutdownFrame::updateScreenPosition() {
    m_shutdownManager->show();
    m_shutdownManager->updateGeometry();
}

void ShutdownFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

ShutdownFrame::~ShutdownFrame()
{

}
