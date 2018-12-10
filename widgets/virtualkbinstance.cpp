#include "virtualkbinstance.h"

#include <QProcess>
#include <QWindow>
#include <QWidget>
#include <QTimer>
#include <QDebug>

VirtualKBInstance &VirtualKBInstance::Instance()
{
    static VirtualKBInstance virtualKB;
    return virtualKB;
}

QWidget *VirtualKBInstance::virtualKBWidget() {
    return m_virtualKBWidget;
}

bool VirtualKBInstance::eventFilter(QObject *watched, QEvent *event)
{

    return QObject::eventFilter(watched, event);
}

VirtualKBInstance::VirtualKBInstance(QObject *parent)
    : QObject(parent)
    , m_virtualKBWidget(nullptr)
{
    QProcess * p = new QProcess(this);

    connect(p, &QProcess::readyReadStandardOutput, [this, p] {
        QByteArray output = p->readAllStandardOutput();

        if (output.isEmpty()) return;

        int xid = atoi(output.trimmed().toStdString().c_str());

        QWindow * w = QWindow::fromWinId(xid);
        m_virtualKBWidget = QWidget::createWindowContainer(w);
        m_virtualKBWidget->setFixedSize(600, 200);

        QTimer::singleShot(500, [=] {
            m_virtualKBWidget->hide();
        });
    });

    p->start("onboard", QStringList() << "-e" << "--layout" << "Small");
}
