#include "bubbleitem.h"
#include "notification/notificationentity.h"
#include "notifycommon.h"

BubbleItem::BubbleItem(QWidget *parent, std::shared_ptr<NotificationEntity> entity)
    : Bubble(parent, entity, OSD::ShowStyle::BUBBLEWIDGET)
{
    setWindowFlags(Qt::Widget);
    setFixedSize(Notify::BubbleItemWidth, Notify::BubbleItemHeight);
}
