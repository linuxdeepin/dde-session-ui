#ifndef PUBLIC_FUNC_H
#define PUBLIC_FUNC_H

#include <QPixmap>
#include <QApplication>
#include <QIcon>

QPixmap loadPixmap(const QString &file) {
    qreal ratio = 1.0;
    qreal devicePixel = qApp->devicePixelRatio();

    QPixmap pixmap;

    if (!qFuzzyCompare(ratio, devicePixel)) {
        pixmap.load(qt_findAtNxFile(file, devicePixel, &ratio));
        pixmap = pixmap.scaled(devicePixel / ratio * pixmap.width(),
                               devicePixel / ratio * pixmap.height(),
                               Qt::IgnoreAspectRatio,
                               Qt::SmoothTransformation);
        pixmap.setDevicePixelRatio(devicePixel);
    } else {
        pixmap.load(file);
    }

    return pixmap;
}

#endif // PUBLIC_FUNC_H
