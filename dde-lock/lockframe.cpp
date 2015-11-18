#include "lockframe.h"

LockFrame::LockFrame(QWidget* parent)
    : BoxFrame(parent)
{

    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry:" << geometry();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint);

    m_lockManager = new LockManager(this);
    QPoint mousePoint = QCursor::pos();

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        if (rect.contains(mousePoint)) {
            m_lockManager->setFixedSize(rect.size());
            m_lockManager->move(rect.x(), rect.y());
            qDebug() << "loginManager:" << m_lockManager->geometry();
            m_lockManager->updateWidgetsPosition();
            continue;
        }
    }

    connect(m_lockManager, &LockManager::screenChanged, this, &LockFrame::updateScreenPosition);
}

void LockFrame::updateScreenPosition(QRect rect) {
    qDebug() << "Move To the Other position:" << rect;
    m_lockManager->setFixedSize(rect.size());
    m_lockManager->move(rect.x(), rect.y());
    m_lockManager->updateGeometry();
    m_lockManager->updateWidgetsPosition();
    qDebug() << "m_loginManager:" << m_lockManager->geometry();
}

void LockFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

LockFrame::~LockFrame() {

}
