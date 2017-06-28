/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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
#include "util_settings.h"

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
    UtilSettings *utilSettings = NULL;
    QTimer *getFocusTimer;

signals:
    void submit();
    void keybdLayoutButtonClicked();
    void updateKeyboardStatus();
    void alertChanged(const int alert);
    void focusIn();

    void leftKeyPressed();
    void rightKeyPressed();

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

    bool isReadOnly();
    void setReadOnly(bool value);

    void setAlert(bool alert, const QString &text = QString());
    inline bool alert() const {return m_alert;}

    void setMessage(const QString &message);

    void lineEditGrabKeyboard();
    void recordUserPassWd(bool isChoose, QString username);
    void updateCapslockStatus(bool on);

protected:
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    int m_timerCount = 0;

    void initUI();
    void initConnect();
    void initData();

    bool m_alert = false;
    bool m_alert_enter = false;

    QLineEdit *m_lineEdit;
#ifndef SHENWEI_PLATFORM
    QPushButton *m_keyboardButton;
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
