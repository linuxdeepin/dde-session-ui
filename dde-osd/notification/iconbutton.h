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
    explicit IconButton(QWidget *parent = nullptr);
    explicit IconButton(const QString &text, QWidget *parent = nullptr);

    void setText(const QString &text);
    void setFocusState(bool has);
    void setBackOpacity(int opacity);
    void setRadius(int radius);

Q_SIGNALS:
    void clicked();

private:
    QString m_text;
    bool m_hasFocus;
    bool m_hover;
    int m_opacity;
    int m_radius;
    bool m_hasBackColor;

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
};

#endif // ICONBUTTON_
