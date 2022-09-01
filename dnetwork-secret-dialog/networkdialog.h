// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QProcess>

class NetworkDialog : public QObject
{
    Q_OBJECT

public:
    explicit NetworkDialog(QObject *parent = Q_NULLPTR);
    ~NetworkDialog();

    bool exec(const QJsonDocument &doc);

private Q_SLOTS:
    void finished(int exitCode, QProcess::ExitStatus);
    void readProcessOutput();

private:
    QProcess *m_process;
    QString m_key;
    QByteArray m_lastData; // 用于数据拼接
};

#endif // NETWORKDIALOG_H
