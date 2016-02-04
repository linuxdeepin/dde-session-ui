/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef ROUNDITEMBUTTON
#define ROUNDITEMBUTTON

#include <QObject>
#include <QFrame>
#include <QAbstractButton>
#include <QLabel>
#include <QtWidgets>
#include <QFocusEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QGraphicsOpacityEffect>

/* The RoundItemButton is used by shutdownButton, restartButton, and so on... */

class RoundItemButton: public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QString normalIcon MEMBER m_normalIcon DESIGNABLE true NOTIFY iconChanged)
    Q_PROPERTY(QString hoverIcon MEMBER m_hoverIcon DESIGNABLE true NOTIFY iconChanged)
    Q_PROPERTY(QString pressedIcon MEMBER m_pressedIcon DESIGNABLE true NOTIFY iconChanged)
    Q_PROPERTY(bool disabled READ isDisabled WRITE setDisabled NOTIFY stateChanged)

public:
    RoundItemButton(const QString &text, QWidget* parent=0);
    ~RoundItemButton();

    enum State {Default, Normal, Hover, Checked, Pressed, Disabled};

    void setDisabled(bool disabled);
    inline bool isDisabled() const {return m_state == Disabled;}
    void setChecked(bool checked);
    inline bool isChecked() const {return m_state == Checked;}
    inline State state() const {return m_state;}
    inline const QString text() const {return m_itemText->text();}
    inline void setText(const QString &text) {m_itemText->setText(text);}
    void updateState(const State state);
signals:
    void stateChanged(const State state);
    void clicked();
    void iconChanged();

private slots:
    void updateIcon();
    inline void setState(const State state) {updateState(state);}
protected:
    void paintEvent(QPaintEvent* event);
    void enterEvent(QEvent* event);
    void leaveEvent(QEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* e);

private:
    void initUI();
    void initConnect();

    State m_state = Normal;

    QLabel *m_itemIcon;
    QLabel *m_itemText;

    QGraphicsOpacityEffect *m_opacityEffect;

    QString m_normalIcon;
    QString m_hoverIcon;
    QString m_pressedIcon;
};
#endif // ROUNDITEMBUTTON
