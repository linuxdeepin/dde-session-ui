/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
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

#include "pincodedialog.h"
#include "widgets/labels/largelabel.h"

#include <QDateTime>
#include <QTimer>

using namespace dcc::widgets;

namespace dcc {
namespace bluetooth {

PinCodeDialog::PinCodeDialog(const QString &pinCode,  const QString &devicepath, const QString &starttime, const bool &cancelable) :
    DDialog(),
    m_pinCodeLabel(new dcc::widgets::LargeLabel(this)),
    m_titileLabel(new dcc::widgets::LargeLabel(this)),
    m_bluetoothInter(new DBusBluetooth("com.deepin.daemon.Bluetooth", "/com/deepin/daemon/Bluetooth", QDBusConnection::sessionBus(), this))
{
    QString titilestr = tr("The PIN for connecting to the Bluetooth device is:");
    setIcon(QIcon::fromTheme("notification-bluetooth-connected"));
    setAttribute(Qt::WA_QuitOnClose);
    m_titileLabel->setObjectName("TitileText");
    addContent(m_titileLabel, Qt::AlignTop | Qt::AlignHCenter);

    QStringList btns;
    if (cancelable) {
        btns << tr("Cancel") ;
    }
    btns << tr("Confirm");
    addButtons(btns);

    m_pinCodeLabel->setObjectName("PinCodeText");
    addContent(m_pinCodeLabel, Qt::AlignBottom | Qt::AlignHCenter);
    QFont font = m_titileLabel->font();
    font.setBold(true);
    font.setPixelSize(16);
    m_titileLabel->setFont(font);
    m_titileLabel->setText(titilestr);
    m_pinCodeLabel->setText(pinCode);

    qint64 msec = 60 * 1000 - QDateTime::currentMSecsSinceEpoch() + starttime.toLongLong();
    if (msec < 0){
        qDebug() << "timeout";
        exit(-2);
    }
    QTimer::singleShot(msec, this, [ = ]() {
        close();
    });

    connect(m_bluetoothInter, &DBusBluetooth::AdapterPropertiesChanged, this, &PinCodeDialog::HandleBlutoothPower);
    connect(this, &PinCodeDialog::buttonClicked, this, [ = ](int index, const QString & text) {
        m_bluetoothInter->Confirm(QDBusObjectPath(devicepath), index == 1 ? true : false);
    });
    connect(this, &PinCodeDialog::closed, this, [ = ]() {
        m_bluetoothInter->Confirm(QDBusObjectPath(devicepath), false);
        qApp->quit();
    });
}

PinCodeDialog::~PinCodeDialog()
{

}

QString PinCodeDialog::pinCode() const
{
    return m_pinCodeLabel->text();
}

void PinCodeDialog::HandleBlutoothPower(const QString &message)
{
    QJsonParseError json_error;
    QString poweredbool;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(message.toLocal8Bit().data(), &json_error));

    if (json_error.error != QJsonParseError::NoError) {
        qDebug() << "json error! error type " << json_error.error;
        return;
    }

    QJsonObject rootObj = jsonDoc.object();

    if (rootObj.contains("Powered")) {
        bool test = rootObj.value("Powered").toBool();
        if (!test) {
            close();
        }
    }
}

PinCodeDialog::PinCodeDialog() :
    PinCodeDialog("", "", "", false)
{

}

} // namespace bluetooth
} // namespace dcc
