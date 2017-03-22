#ifndef COMMON_H
#define COMMON_H

#include <QPainter>
#include <QStyleOptionViewItem>
#include <QDebug>

#define Pair(x, y) QPair<QString, QString>(x, y)

static const int ImageTextItemWidth = 140;
static const int ImageTextItemHeight = 140;
static const int TextItemWidth = 200;
static const int TextItemHeight = 36;

static const QColor ItemHighlightColor = "#01bdff";

class DrawHelper {
public:
    static void DrawImage(QPainter *painter, const QStyleOptionViewItem &option, const QPixmap &pix, bool withText = false, bool withProgress = false);
    static void DrawText(QPainter *painter, const QStyleOptionViewItem &option, const QString &text, QColor color = Qt::black, bool withImage = true);
    static void DrawProgressBar(QPainter *painter, const QStyleOptionViewItem &option, double progress);
};

#endif // COMMON_H
