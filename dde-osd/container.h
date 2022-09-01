// SPDX-FileCopyrightText: 2011 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef CONTAINER_H
#define CONTAINER_H

#include <DBlurEffectWidget>
#include <DWindowManagerHelper>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

class QHBoxLayout;
/*!
 * \~chinese \class Container
 * \~chinese \brief 用来显示osd listview的窗口
 */
class Container : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit Container(QWidget *parent = nullptr);

    void setContent(QWidget *content);
    void moveToCenter();

protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private slots:
    void windowManagerChanged();
    void updateWindowRadius();
    int getWindowRadius();
    void onDelayQuit();

private:
    QHBoxLayout *m_layout;
    DWindowManagerHelper *m_wmHelper;
    QTimer *m_quitTimer;
    bool m_supportComposite;
};

#endif // CONTAINER_H
