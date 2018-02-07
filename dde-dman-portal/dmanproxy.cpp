/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
 *
 * Maintainer: sbw <sbw@sbw.so>
 *             Hualet <mr.asianwang@gmail.com>
 *             kirigaya <kirigaya@mkacg.com>
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

#include "dmanproxy.h"

#include <QDir>
#include <QUrl>
#include <QLocale>
#include <QProcess>

#include <QDebug>

static QString getFlatpakPath(const QString &appid)
{
    QProcess flatpakProcess;

    flatpakProcess.setProgram("flatpak");
    flatpakProcess.setArguments(QStringList() << "info" << appid);

    flatpakProcess.start();
    flatpakProcess.waitForStarted(-1);
    flatpakProcess.waitForFinished(-1);
    auto out = flatpakProcess.readAllStandardOutput();
    auto err = flatpakProcess.readAllStandardError();
    auto lines = out.split('\n');
    for (auto line : lines) {
        if (line.startsWith("Location: ")) {
            QString path = QString::fromUtf8(line);
            return path.remove("Location: ");
        }
    }

    if (err.isEmpty()) {
        qWarning() << "get err:" << err;
    }
    return "";
}

static void runDMan(const QString &arg)
{
    QProcess *dmanProcess = new QProcess;

    dmanProcess->setProgram("dman");
    dmanProcess->setArguments(QStringList() << arg);

    dmanProcess->connect(dmanProcess, static_cast<void (QProcess::*)(int)>(&QProcess::finished),
    dmanProcess, [ = ](int /*exitCode*/) {
        dmanProcess->deleteLater();
    });
    dmanProcess->start();
}

DManProxy::DManProxy(QObject *parent) :
    QObject(parent)
{

}

void DManProxy::ShowManual(const QString appid)
{
    QDir dmanShare("/usr/share/dman");
    if (dmanShare.exists(appid)) {
        runDMan(appid);
        return;
    };

    // open flatpak dman
    auto path = getFlatpakPath(appid);
    if (!path.isEmpty()) {
        auto lang = QLocale().name();
        auto dmanRootPath = path + "/files/share/dman";
        QDir dmanRootDir(dmanRootPath);
        auto dmanAppDir = dmanRootDir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs).value(0);
        auto dmanLang = dmanRootPath + "/" + dmanAppDir + "/" + lang;
        auto fileUrl = QUrl::fromLocalFile(dmanLang);
        qDebug() << "open path" << fileUrl;
        runDMan(fileUrl.toString());
    }
}

