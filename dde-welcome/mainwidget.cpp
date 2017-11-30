/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             kirigaya <kirigaya@mkacg.com>
 *             Hualet <mr.asianwang@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwidget.h"
#include "updatecontent.h"
#include "version.h"

#include <QGSettings/QGSettings>
#include <QVariant>
#include <QKeyEvent>

MainWidget::MainWidget(QWidget *parent)
    : FullscreenBackground(parent)
{
    if (QFile::exists(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first() + "/autostart/dde-first-run.desktop")) {
        m_isUpgrade = false;

        QString currentVersion = getSystemVersion();

        if (!currentVersion.isEmpty()) {
            QSettings welcomeSetting("deepin", "dde-welcome");
            QString version = welcomeSetting.value("Version").toString();
            if (version.isEmpty()) {
                welcomeSetting.setValue("Version", currentVersion);
            }
        }
    } else {
        m_isUpgrade = checkVersion();
    }

    if (m_isUpgrade) {
        UpdateContent *content = new UpdateContent(getSystemVersion(), this);
        setContent(content);

        QGSettings *gsettings = new QGSettings("com.deepin.dde.appearance", "", this);
        const QStringList list = gsettings->get("background-uris").toStringList();
        setBackground(list.first());
    }

#ifdef QT_DEBUG
    show();
    grabKeyboard();
#endif
}

bool MainWidget::checkVersion()
{
    QString currentVersion = getSystemVersion();

    if (currentVersion.isEmpty())
        return false;

    // check file exist
    QSettings welcomeSetting("deepin", "dde-welcome");
    QString version = welcomeSetting.value("Version").toString();
    if (version.isEmpty()) {
        welcomeSetting.setValue("Version", currentVersion);
        return true;
    }

    QRegExp re("(^\\d+(\\.\\d+)?)");

    int currentVersionPos = currentVersion.indexOf(re);
    if (currentVersionPos >= 0)
        currentVersion = re.cap(0);
    else
        return false;

    int versionPos = version.indexOf(re);
    if (versionPos >= 0)
        version = re.cap(0);
    else
        return false;

    const int result = alpm_pkg_vercmp(currentVersion.toStdString().c_str(), version.toStdString().c_str());

    if (result > 0) {
        welcomeSetting.setValue("Version", currentVersion);
        return true;
    }

    welcomeSetting.setValue("Version", currentVersion);

    return false;
}

const QString MainWidget::getSystemVersion()
{
    QSettings lsbSetting("/etc/lsb-release", QSettings::IniFormat);
    return lsbSetting.value("DISTRIB_RELEASE").toString();
}

void MainWidget::dbus_show()
{
    qDebug() << Q_FUNC_INFO;

    if (!m_isUpgrade)
        return;

    show();

    QTimer::singleShot(1, this, [=] {
        grabKeyboard();
    });
}

void MainWidget::dbus_exit()
{
    qDebug() << Q_FUNC_INFO;

    if (!m_isUpgrade)
        qApp->quit();
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    case Qt::Key_Enter:
    case Qt::Key_Return:
        if (m_isUpgrade)
            qApp->quit();
    default:;
    }

    FullscreenBackground::keyPressEvent(event);
}
