// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
    void keyPressEvent(QKeyEvent *event) override;

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
    const QMap<QString, QString> m_secretKeyStrMap;
    const QMap<QString, QString> m_propMap;
    QString m_connName;
    QString m_connType;
    QString m_connSettingName;
    QStringList m_secretKeyList;
    QJsonObject m_propsObj;

    QList<Dtk::Widget::DLineEdit *> m_lineEditList;

    bool m_allInputValid;
};

#endif // NETWORKSECRETDIALOG_H
