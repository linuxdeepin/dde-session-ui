/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     listenerri <listenerri@gmail.com>
 *
 * Maintainer: listenerri <listenerri@gmail.com>
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

#ifndef NETWORKSECRETDIALOG_H
#define NETWORKSECRETDIALOG_H

#include <QJsonDocument>
#include <QJsonObject>

#include <ddialog.h>
#include <dpasswordedit.h>

class NetworkSecretDialog : public Dtk::Widget::DDialog
{
    Q_OBJECT
public:
    enum WepKeyType {NotSpecified, Hex, Passphrase};

    explicit NetworkSecretDialog(QJsonDocument jsonDoc, QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private slots:
    void parseJsonData(const QJsonDocument &jsonDoc);
    void initUI();
    void onButtonsClicked(int index, const QString &text);
    void submit();
    void checkInputValid();
    bool passwordIsValid(const QString &text, const QString &secretKey);

    bool wepKeyIsValid(const QString &key, WepKeyType type);
    bool wpaPskIsValid(const QString &psk);


private:
    QJsonObject m_jsonObj;
    QMap<QString, QString> m_secretKeyStrMap;
    QString m_connName;
    QString m_connType;
    QString m_connSettingName;
    QStringList m_secretKeyList;

    QList<Dtk::Widget::DPasswordEdit *> m_lineEditList;

    bool m_allInputValid;
};

#endif // NETWORKSECRETDIALOG_H
