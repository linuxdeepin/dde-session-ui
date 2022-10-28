/*
 * Copyright (C) 2011 ~ 2021 Deepin Technology Co., Ltd.
 *
 * Author:     caixiangrong <caixiangrong@uniontech.com>
 *
 * Maintainer: caixiangrong <caixiangrong@uniontech.com>
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

#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QObject>

class QLocalSocket;

class NetworkDialog : public QObject
{
    Q_OBJECT

public:
    explicit NetworkDialog(QObject *parent = Q_NULLPTR);
    ~NetworkDialog();

    bool exec(const QJsonDocument &doc);
    void onPassword(QLocalSocket *socket, const QByteArray &data);

private Q_SLOTS:
    void connectedHandler();
    void disConnectedHandler();
    void readyReadHandler();
    bool ConnectToServer();

private:
    QString m_key;
    QByteArray m_lastData; // 用于数据拼接

    QByteArray m_data;
    QLocalSocket *m_clinet;
};

#endif // NETWORKDIALOG_H
