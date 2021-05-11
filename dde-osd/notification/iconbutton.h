/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Maintainer: zorowk <near.kingzero@gmail.com>
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

#ifndef ICONBUTTON_H
#define ICONBUTTON_H
#include <DWidget>

DWIDGET_USE_NAMESPACE
/*!
 * \~chinese \class IconButton
 * \~chinese \brief 自定义按钮类
 */
class IconButton : public DWidget
{
    Q_OBJECT
public:
    enum ButtonState {
        HOVER,
        PRESS,
        RELEASE,
    };

    explicit IconButton(QWidget *parent = nullptr);
    explicit IconButton(const QString &text, QWidget *parent = nullptr);

    void setText(const QString &text);
    void setIcon(const QString &iconPath);
    void setFocusState(bool has);
    void setOpacity(const ButtonState state, int opacity);
    void setRadius(int radius);

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

Q_SIGNALS:
    void clicked();

private:
    QString m_text;
    bool m_hasFocus;
    bool m_hover;
    int m_hoverOpacity;
    int m_pressOpacity;
    int m_releaseOpacity;
    int m_radius;
    ButtonState m_currentState;
    QPoint m_pressPos;
    QString m_iconPath;
};

#endif // ICONBUTTON_
