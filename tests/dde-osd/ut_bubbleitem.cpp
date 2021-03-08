#include <gtest/gtest.h>

#include "notification-center/bubbleitem.h"

#include <QApplication>
#include <QMimeData>
#include <QDateTime>
#include <QtTest>

class UT_BubbleItem : public testing::Test
{
public:
    void SetUp() override
    {
        QString appName = "deepin-screen-recorder";
        uint id = 0;
        QString appIcon = "deepin-screenshot";
        QString summary = "截图完成";
        QString body = "图片已保存到/home/chenwei/Desktop/截图录屏_20200709104935.png";
        QStringList actions ;
        actions << "_open" << "查看";
        QVariantMap hints;
        int expireTimeout = -1;

        entity = std::make_shared<NotificationEntity>(appName, QString(), appIcon,
                                                                      summary, body, actions, hints,
                                                                      QString::number(QDateTime::currentMSecsSinceEpoch()),
                                                                      QString::number(id),
                                                                      QString::number(expireTimeout));
        obj = new BubbleItem(nullptr, entity);
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    BubbleItem *obj = nullptr;
    EntityPtr entity;
};

TEST_F(UT_BubbleItem, coverageTest)
{
    obj->setAlpha(10);
    obj->setHasFocus(false);
    obj->indexRow();
    obj->setOverlapWidget(false);
}

TEST_F(UT_BubbleItem, eventTest)
{
    BubbleItem item(obj, entity);

    // 可以连接到任何物体的任何信号并记录其发射。QSignalSpy本身就是一个QVariant列表的列表。
    // 信号的每次发射都会在列表中追加一项，其中包含信号的参数。
    QSignalSpy spy(&item, SIGNAL(havorStateChanged(bool)));

    // 触发一次该信号的事件，QEvent::Leave事件havorStateChanged信号携带的参数为false
    QEvent hoverEvent(QEvent::Leave);
    QApplication::sendEvent(&item, &hoverEvent);

    // 判断信号是否只发送了一次
    EXPECT_EQ(spy.count(), 1);

    QList<QVariant> arguments = spy.takeFirst();
    // 判断信号发送的数据类型
    QVERIFY(arguments.at(0).type() == QVariant::Bool);
    // 判断信号发送的结果
    EXPECT_EQ(arguments.at(0).toBool(), false);
}
