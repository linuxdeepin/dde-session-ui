#include "bubbleitem.h"
#include "notification/notificationentity.h"

BubbleItem::BubbleItem(std::shared_ptr<NotificationEntity> entity)
    : Bubble(entity, OSD::ShowStyle::BUBBLEWIDGET)
{
    setFixedSize(BubbleItemWidth, BubbleItemHeight);
}
