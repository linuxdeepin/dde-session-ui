#ifndef DEFINE_H
#define DEFINE_H

#include <QSize>
#include <QStringList>
#include <QStandardPaths>
#include <QXmlStreamReader>

#define MIN(a,b) ((a)>(b)?(b):(a))
#define ABS(a) (a)>0?(a):(-(a))

static const int ScreenPadding = 20;    //最上方通知距屏幕上方间隔
static const int BubbleMargin = 12;     //桌面消息通知间隔
static const int BubblePadding = 10;    //消息通知内部Padding
static const int BubbleSpacing = 10;    //消息通知内部Space
static const int BubbleTimeout = 5000;  //通知默认超时时间(毫秒)
static const int BubbleEntities = 3;
static const int BubbleOverLap = 2;     //层叠的气泡数量
static const int BubbleOverLapHeight = 12;  //通知中心层叠层高度
static const QString NoReplaceId = "0";      //为0 返回一个计数值给程序
static const int AnimationTime = 300;       //ms
static const QStringList Directory = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
static const QString CachePath = Directory.first() + "/.cache/deepin/deepin-notifications/";

namespace Notify {
static const int CenterWidth = 400;
static const int CenterMargin = 10;
static const int CenterTitleHeight = 32;

static const int GroupTitleHeight = 32;
static const int GroupButtonSize = 24;
static const int GroupMargin = 30;
static const int BubbleDefaultAlpha = 20;       //bubble控件透明度
static const qreal BubbleOpacity = 0.6;    //bubble内容的透明度
}

class OSD
{
public:
    typedef enum {
        BUBBLEWINDOW,
        BUBBLEWIDGET
    } ShowStyle;

    enum DockPosition {
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3
    };

    static QSize BubbleSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(600, 60);
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
            size = QSize(70, 40);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(60, 36);
        }
        return size;
    }

    static QSize IconButtonSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(36, 36);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(36, 36);
        }
        return size;
    }

    static QSize IconSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(40, 40);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(24, 24);
        }
        return size;
    }

    static QSize CloseButtonSize(ShowStyle style)
    {
        QSize size;
        if (style == BUBBLEWINDOW) {
            size = QSize(30, 30);
        } else if (style == BUBBLEWIDGET) {
            size = QSize(24, 24);
        }
        return size;
    }

    static QString removeHTML(const QString &source)
    {
        QXmlStreamReader xml(source);
        QString textString;
        while (!xml.atEnd()) {
            if (xml.readNext() == QXmlStreamReader::Characters) {
                textString += xml.text();
            }
        }

        return textString.isEmpty() ? source : textString;
    }
};
#endif // DEFINE_H
