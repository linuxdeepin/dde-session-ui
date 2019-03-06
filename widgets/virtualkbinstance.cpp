#include "virtualkbinstance.h"

#include <QProcess>
#include <QWindow>
#include <QWidget>
#include <QTimer>
#include <QDebug>
#include <QResizeEvent>

VirtualKBInstance &VirtualKBInstance::Instance()
{
    static VirtualKBInstance virtualKB;
    return virtualKB;
}

QWidget *VirtualKBInstance::virtualKBWidget() {
    return m_virtualKBWidget;
}

void VirtualKBInstance::init()
{
    if (m_virtualKBWidget) {
        emit initFinished();
        return;
    }

    QProcess * p = new QProcess(this);

    connect(p, &QProcess::readyReadStandardOutput, [this, p] {
        QByteArray output = p->readAllStandardOutput();

        if (output.isEmpty()) return;

        int xid = atoi(output.trimmed().toStdString().c_str());

        QWindow * w = QWindow::fromWinId(xid);
        m_virtualKBWidget = QWidget::createWindowContainer(w);
        m_virtualKBWidget->setFixedSize(600, 200);

        QTimer::singleShot(300, [=] {
            m_virtualKBWidget->hide();
            emit initFinished();
        });
    });

    p->start("onboard", QStringList() << "-e" << "--layout" << "Small");
}

bool VirtualKBInstance::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == m_virtualKBWidget && event->type() == QEvent::Resize) {
        QResizeEvent *e = static_cast<QResizeEvent*>(event);
        return e->size() != QSize(600, 200);
    }

    return QObject::eventFilter(watched, event);
}

VirtualKBInstance::VirtualKBInstance(QObject *parent)
    : QObject(parent)
    , m_virtualKBWidget(nullptr)
{
}
