// SPDX-FileCopyrightText: 2022-2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#define private public
#include "content.h"
#undef private

#include <QPushButton>
#include <QCheckBox>
#include <QImage>
#include <QPainter>

#include <gtest/gtest.h>

class UT_Content : public testing::Test
{
public:
    void SetUp() override;


    void TearDown() override;

public:
    Content *widget = nullptr;
};

void UT_Content::SetUp()
{
    widget = new Content();
}

void UT_Content::TearDown()
{
    widget->deleteLater();
    widget = nullptr;
}

TEST_F(UT_Content, coverageTest)
{
    widget->setCnSource("/usr/share/deepin-deepinid-client/privacy/deepinid-CN-zh_CN.md");
    widget->setEnSource("/usr/share/deepin-deepinid-client/privacy/deepinid-US-en_US.md");
    widget->setHideBottom(false);
    Q_EMIT widget->m_languageBtn->buttonClicked(nullptr);
    EXPECT_TRUE(widget->calWidgetWidth() > 0);
    //发送关闭信号
    widget->m_cancelBtn->clicked();
}

TEST_F(UT_Content, consentControlsConfirmation)
{
    widget->setAllowCheckBoxText("Agree");
    widget->setEnAllowCheckBoxText("Agree");
    widget->updateContent();
    EXPECT_FALSE(widget->m_acceptBtn->isEnabled());
    widget->m_acceptCheck->setChecked(true);
    EXPECT_TRUE(widget->m_acceptBtn->isEnabled());
    widget->m_acceptCheck->setChecked(false);
    EXPECT_FALSE(widget->m_acceptBtn->isEnabled());
}

TEST_F(UT_Content, disabledConfirmationBackgroundIsDimmed)
{
    auto *button = widget->m_acceptBtn;
    button->resize(200, 36);
    auto render = [button] {
        QImage image(button->size(), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::white);
        QPainter painter(&image);
        button->render(&painter);
        return image.pixelColor(15, 10);
    };
    button->setEnabled(true);
    const QColor enabledColor = render();
    button->setEnabled(false);
    const QColor disabledColor = render();
    EXPECT_GT(disabledColor.lightness(), enabledColor.lightness());
}
