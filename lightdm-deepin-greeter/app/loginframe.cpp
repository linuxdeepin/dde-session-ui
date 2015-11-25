#include <QDebug>
#include <QPushButton>
#include <QScreen>
#include <QTimer>
#include "loginframe.h"

LoginFrame::LoginFrame(QWidget *parent)
    : BoxFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry:" << geometry();
    setWindowFlags(Qt::FramelessWindowHint | Qt::BypassWindowManagerHint);

    m_loginManager = new LoginManager(this);
    QPoint mousePoint = QCursor::pos();

    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        if (rect.contains(mousePoint)) {
            m_loginManager->setFixedSize(rect.size());
            m_loginManager->move(rect.x(), rect.y());
            qDebug() << "loginManager:" << m_loginManager->geometry();
            m_loginManager->updateWidgetsPosition();
            continue;
        }
    }

    connect(m_loginManager, &LoginManager::screenChanged, this, &LoginFrame::updateScreenPosition);
}

void LoginFrame::updateScreenPosition(QRect rect) {
    qDebug() << "Move To the Other position:" << rect;
    m_loginManager->setFixedSize(rect.size());
    m_loginManager->move(rect.x(), rect.y());
    m_loginManager->updateGeometry();
    m_loginManager->updateWidgetsPosition();
    qDebug() << "m_loginManager:" << m_loginManager->geometry();
}

void LoginFrame::keyPressEvent(QKeyEvent *e) {
    Q_UNUSED(e);
}

LoginFrame::~LoginFrame()
{

}
