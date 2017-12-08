/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#ifndef PASSWDEDIT
#define PASSWDEDIT

#include <QFrame>
#include <QLineEdit>
#include <QKeyEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <dimagebutton.h>

#include "util_updateui.h"

#include "errortooltip.h"

class KeyboardMonitor;

class PassWdEdit : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool alert READ alert NOTIFY alertChanged DESIGNABLE true)

public:
    PassWdEdit(QWidget *parent = 0);
    ~PassWdEdit();

    QMap<QString, QString> keybdLayoutMap;
    QStringList keybdLayoutMainDescriptionList;
    QMap<QString, QString> keybdLayoutShortDescriptionMap;

    QStringList keyboardLayoutList;

signals:
    void submit();
    void keybdLayoutButtonClicked();
    void updateKeyboardStatus();
    void alertChanged(const int alert);

public slots:
    QString getText();
#ifdef LOCK_NO_QUIT
    void clearText();
#endif
    void setEnterBtnStyle(const QString &normalPic, const QString &hoverPic, const QString &pressedPic);

#ifndef SHENWEI_PLATFORM
    void updateKeybdLayoutUI(QStringList keybdList);
#endif
    void show();
    void hide();

    void setAlert(bool alert, const QString &text = QString());
    inline bool alert() const {return m_alert;}

    void setMessage(const QString &message);

    void recordUserPassWd(bool isChoose, QString username);
    void updateCapslockStatus(bool on);
    void selectAll() const;

protected:
    void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    int m_timerCount = 0;

    void initUI();
    void initConnect();
    void initData();

    bool m_alert = false;
    bool m_alert_enter = false;

    QLineEdit *m_lineEdit;
#ifndef SHENWEI_PLATFORM
    DImageButton *m_keyboardButton;
#endif
    DTK_WIDGET_NAMESPACE::DImageButton *m_iconButton;
    QHBoxLayout *m_Layout;

    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_showAni;
    QPropertyAnimation *m_hideAni;
    QMap<QString, QString> passwordMap;

    KeyboardMonitor *m_capslockMonitor;
    QLabel *m_capslockWarning;

    ErrorTooltip *m_errorTooltip;
};
#endif // PASSWDEDIT
