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

#ifndef DCC_BLUETOOTH_PINCODEDIALOG_H
#define DCC_BLUETOOTH_PINCODEDIALOG_H

#include <ddialog.h>
#include <com_deepin_daemon_bluetooth.h>

#include <QJsonDocument>
#include <QJsonParseError>

DWIDGET_USE_NAMESPACE

using  DBusBluetooth = com::deepin::daemon::Bluetooth;

namespace dcc {
namespace widgets {
class LargeLabel;
}

namespace bluetooth {
class PinCodeDialog : public DDialog
{
    Q_OBJECT
public:
    explicit PinCodeDialog();
    explicit PinCodeDialog(const QString &pinCode,  const QString &devicepath, const QString &starttime,  const bool &cancelable = true);
    ~PinCodeDialog();

    QString pinCode() const;
private Q_SLOTS:
    void HandleBlutoothPower(const QString &message);
private:
    dcc::widgets::LargeLabel *m_pinCodeLabel;
    dcc::widgets::LargeLabel *m_titileLabel;
    DBusBluetooth *m_bluetoothInter;
};
} // namespace bluetooth
} // namespace dcc

#endif // DCC_BLUETOOTH_PINCODEDIALOG_H
