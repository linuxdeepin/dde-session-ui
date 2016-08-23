/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef SWITCHFRAME
#define SWITCHFRAME

#include <QFrame>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPropertyAnimation>
#include <QDebug>

#include <dimagebutton.h>

class ItemButton: public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QString normalIcon MEMBER m_normalIcon DESIGNABLE true NOTIFY iconChanged)
    Q_PROPERTY(QString hoverIcon MEMBER m_hoverIcon DESIGNABLE true NOTIFY iconChanged)
    Q_PROPERTY(QString pressedIcon MEMBER m_pressedIcon DESIGNABLE true NOTIFY iconChanged)
    Q_PROPERTY(bool disabled READ isDisabled WRITE setDisabled NOTIFY stateChanged)

public:
    ItemButton(QWidget* parent=0);
    ~ItemButton();

    enum State {Default, Normal, Hover, Checked, Pressed, Disabled};

    void setDisabled(bool disabled);
    inline bool isDisabled() const {return m_state == Disabled;}
    void setChecked(bool checked);
    inline bool isChecked() const {return m_state == Checked;}
    inline State state() const {return m_state;}
    void updateState(const State state);
signals:
    void stateChanged(const State state);
    void clicked();
    void iconChanged();

private slots:
    void updateIcon();
    inline void setState(const State state) {updateState(state);}
protected:
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    void initUI();
    void initConnect();

    State m_state = Normal;

    QLabel *m_itemIcon;
    QString m_normalIcon;
    QString m_hoverIcon;
    QString m_pressedIcon;
};

class SwitchFrame:public QFrame
{
    Q_OBJECT
public:
    SwitchFrame(QWidget* parent=0);
    ~SwitchFrame();

signals:
    void triggerSwitchSession();
    void triggerSwitchUser();
    void triggerPower();

public slots:
    void chooseToSession(const QString &tips);
    inline void setUserSwitchEnable(bool enable) {m_switchUserButton->setVisible(enable);}
    inline void setSessionSwitchEnable(bool enable) {m_switchSessionButton->setVisible(enable);}

protected:
    bool eventFilter(QObject *o, QEvent *e);

private:
    void initUI();
    void initConnect();

private slots:
    void hideTips();
    void showTips();

private:
    QLabel *m_tipsLabel;
    QWidget *m_tipsWidget;
#ifndef SHENWEI_PLATFORM
    DTK_WIDGET_NAMESPACE::DImageButton* m_switchSessionButton;
    DTK_WIDGET_NAMESPACE::DImageButton* m_switchUserButton;
    DTK_WIDGET_NAMESPACE::DImageButton* m_powerButton;
#else
    ItemButton* m_switchSessionButton;
    ItemButton* m_switchUserButton;
    ItemButton* m_powerButton;
#endif
    QHBoxLayout* m_Layout;
#ifndef SHENWEI_PLATFORM
    QPropertyAnimation *m_tipsAni;
#endif
};
#endif // SWITCHFRAME

