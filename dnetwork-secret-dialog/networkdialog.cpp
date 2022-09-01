// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "networkdialog.h"

#include <QFile>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

static const QString NetworkDialogApp = "dde-network-dialog"; //网络列表执行文件

NetworkDialog::NetworkDialog(QObject *parent)
    : QObject(parent)
    , m_process(new QProcess(this))
{
    connect(m_process, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &NetworkDialog::finished);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &NetworkDialog::readProcessOutput);
}

NetworkDialog::~NetworkDialog()
{
    m_process->close();
}

void NetworkDialog::finished(int exitCode, QProcess::ExitStatus)
{
    readProcessOutput();
    this->deleteLater();
    qApp->exit(exitCode);
}

void NetworkDialog::readProcessOutput()
{
    QByteArray outData;
    QByteArray allData = m_process->readAllStandardOutput();

    allData = m_lastData + allData;
    QList<QByteArray> dataArray = allData.split('\n');
    m_lastData = dataArray.last();
    for (const QByteArray &data : dataArray) {
        if (data.startsWith("password:")) {
            QByteArray cmd = data.mid(9);
            QJsonDocument doc = QJsonDocument::fromJson(cmd);
            if (doc.isObject()) {
                QJsonObject obj = doc.object();
                QString key = obj.value("key").toString();
                QString passwd = obj.value("password").toString();
                bool input = obj.value("input").toBool();
                if (m_key == key) {
                    if (input) {
                        QJsonObject resultJsonObj;
                        QJsonArray secretsJsonArray;
                        secretsJsonArray.append(passwd);
                        resultJsonObj.insert("secrets", secretsJsonArray);

                        QFile file;
                        if (!file.open(stdout, QFile::WriteOnly)) {
                            qDebug() << "open STDOUT failed";
                            qApp->exit(-4);
                        }
                        file.write(QJsonDocument(resultJsonObj).toJson());
                        file.flush();
                        file.close();
                        qApp->exit(0);
                    } else {
                        qApp->exit(1);
                    }
                    break;
                }
            }
        }
    }
}

/**
 * @brief NetworkDialog::exec
 *        调dde-network-dialog处理密码输入
 *        参数为空或为WPA2企业版需要输入多个项时返回false，用原输入窗处理
 * @param doc json对象
 * @return true 已处理 false 未处理
 */
bool NetworkDialog::exec(const QJsonDocument &doc)
{
    QJsonObject obj = doc.object();
    // 只处理无线网密码
    if (obj.value("connType").toString() != "802-11-wireless") {
        return false;
    }
    QJsonArray array = obj.value("devices").toArray();
    QString device;
    if (!array.isEmpty()) {
        device = array.first().toString();
    }
    QString connName = obj.value("connId").toString();
    if (!connName.isEmpty() && (1 == obj.value("secrets").toArray().size())) {
        m_key = connName;
        m_process->close();
        QStringList argList;
        argList << "-w" << "-c" << m_key;
        if(!device.isEmpty()) {
            argList << "-n" << device;
        }
        m_process->start(NetworkDialogApp, argList);
        m_process->waitForStarted();
        return m_process->state() == QProcess::Running;
    }
    return false;
}
