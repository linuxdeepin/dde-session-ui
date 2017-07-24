#include "wmframe.h"

#include <QScreen>

WMFrame::WMFrame(QWidget *parent) : BoxFrame(parent)
{
    m_wmchooser = new WMChooser(this);

    updateScreenPosition();

    connect(m_wmchooser, &WMChooser::screenChanged, this, &WMFrame::updateScreenPosition);
}

void WMFrame::setConfigPath(const QString &path)
{
    m_wmchooser->setConfigPath(path);
}

void WMFrame::updateScreenPosition()
{
    QList<QScreen *> screenList = qApp->screens();
    QPoint mousePoint = QCursor::pos();
    for (const QScreen *screen : screenList)
    {
        if (screen->geometry().contains(mousePoint))
        {
            const QRect &geometry = screen->geometry();
            m_wmchooser->setFixedSize(geometry.size());
            m_wmchooser->move(geometry.x(), geometry.y());
            m_wmchooser->updateGeometry();

            return;
        }
    }
}
