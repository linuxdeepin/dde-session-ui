#ifndef DEFINE_H
#define DEFINE_H

#include <QSize>

class  OSD
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
