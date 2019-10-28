#ifndef DEFINE_H
#define DEFINE_H

#include <QSize>
#include <QStringList>
#include <QStandardPaths>

static const int Padding = 46;
static const int BubbleMargin = 20;
static const int BubbleTimeout = 5000;//msec
static const QStringList Directory = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
static const QString CachePath = Directory.first() + "/.cache/deepin/deepin-notifications/";

class OSD
{
public:
    typedef enum {
        BUBBLEWINDOW,
        BUBBLEWIDGET
    } ShowStyle;

    static QSize BubbleSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(600, 80);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(380, 90);
        }
        return size;
    }

    static int BubbleWidth(ShowStyle style)
    {
        return BubbleSize(style).width();
    }

    static int BubbleHeight(ShowStyle style)
    {
        return BubbleSize(style).height();
    }

    static QSize ButtonSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(70, 60);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(60, 36);
        }
        return size;
    }

    static QSize IconSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(64, 64);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(20, 20);
        }
        return size;
    }

    static QSize CloseButtonSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(30, 30);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(20, 20);
        }
        return size;
    }
};
#endif // DEFINE_H
