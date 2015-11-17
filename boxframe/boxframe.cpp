#include <QDebug>
#include <QPushButton>
#include <QScreen>
#include <QTimer>

#include "boxframe.h"
#include "backgroundlabel.h"

BoxFrame::BoxFrame(QWidget *parent)
    : QFrame(parent)
{
    this->move(0, 0);
    this->setFixedSize(qApp->desktop()->size());

    qDebug() << "this geometry:" << geometry();
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::BypassWindowManagerHint);
    QList<QScreen *> screenList = qApp->screens();
    for (int i = 0; i < screenList.length(); i++) {
        const QRect rect = screenList[i]->geometry();
        qDebug() << "boxframe rect:" << i<< rect;

        BackgroundLabel* background = new BackgroundLabel(true, this);
        background->setStyleSheet("background-color: transparent;");
        background->setFixedSize(rect.size());
        background->move(rect.x(), rect.y());

        background->show();
        qDebug() << background->geometry();
    }

    // Always receives mouseMoveEvent.
    this->setMouseTracking(true);
}

void BoxFrame::keyPressEvent(QKeyEvent *e) {

#ifndef QT_DEBUG
    Q_UNUSED(e)
#else
    if (e->key() == Qt::Key_Escape) {
        qApp->quit();
    }
#endif
}

void BoxFrame::resizeEvent(QResizeEvent *e) {
    QSize boxFrameSize = e->size();
    this->setFixedSize(boxFrameSize);
}

BoxFrame::~BoxFrame()
{

}
