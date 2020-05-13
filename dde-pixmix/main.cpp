/*
 * Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     fanpengcheng_cm <fanpengcheng_cm@deepin.com>
 *
 * Maintainer: fanpengcheng_cm <fanpengcheng_cm@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QImageReader>

#include <DApplication>
#include <DGuiApplicationHelper>
#include <DLog>

#define MATRIX              16          //图片大小
#define OPACITY             90          //颜色透明度
#define SATUARATION         50          //饱和度
#define BRIGHTNESS          -60         //亮度

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE
DCORE_USE_NAMESPACE

QPixmap calcPix(const QString &pixPath)
{
    QPixmap pix;
    if (!pix.load(pixPath)) {
        QImage image(pixPath);
        pix = QPixmap::fromImage(image);
    }
    if (pix.isNull())
        exit(-1);

    QPixmap pix2 = pix.scaled(MATRIX, MATRIX, Qt::IgnoreAspectRatio, Qt::FastTransformation);

    int r = 0;
    int g = 0;
    int b = 0;

    for (int i = 0; i < MATRIX; ++i) {
        for (int j = 0; j < MATRIX; ++j) {
            QPoint p(i, j);
            QRgb rgb = pix2.toImage().pixel(p);
            QString str = QString("%1").arg(rgb & RGB_MASK, 8, 16, QChar('0'));

            QString red = str.mid(2, 2);
            QString green = str.mid(4, 2);
            QString blue = str.mid(6, 2);

            bool ok = false;
            r += red.toInt(&ok, 16);
            g += green.toInt(&ok, 16);
            b += blue.toInt(&ok, 16);
        }
    }

    QColor color(int(r * 1.0 / (MATRIX * MATRIX)), int(g * 1.0 / (MATRIX * MATRIX)), int(b * 1.0 / (MATRIX * MATRIX)));
    color = DGuiApplicationHelper::adjustColor(color, 0, SATUARATION, BRIGHTNESS, 0, 0, 0, 0);

    QPainter painter(&pix);
    painter.setRenderHints(painter.renderHints() | QPainter::SmoothPixmapTransform);
    color.setAlpha(int(OPACITY * 1.0 / 100 * 255));
    painter.fillRect(pix.rect(), color);

    return pix;
}

int main(int argc, char *argv[])
{
    DApplication a(argc, argv);
    a.setApplicationVersion("0.0.1");

    a.setOrganizationName("deepin");
    a.setApplicationName("dde-pixmix");

    DLogManager::registerConsoleAppender();
    DLogManager::registerFileAppender();

    QCommandLineOption option(QStringList() << "o" << "output", "Output suitable background image.");
    option.setValueName("path");

    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription("DDE");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(option);

    cmdParser.process(a);

    if (cmdParser.isSet(option)) {
        QString outFile = cmdParser.value(option);
        if (outFile == "-") {
            QString inFile = a.arguments().last();
            QPixmap pix = calcPix(inFile);

            if (!pix.isNull()) {
                QFile *out = new QFile();
                out->open(stdout, QIODevice::WriteOnly);
                if (!pix.save(out, QImageReader::imageFormat(inFile), 100))
                    return -2;
                out->flush();
                out->close();
            } else {
                qDebug() << "pix is null";
                return -3;
            }
        } else {
            if (!outFile.isEmpty()) {
                QString inFile = a.arguments().last();
                QPixmap pix = calcPix(inFile);
                if (!pix.isNull()) {
                    if (!pix.save(outFile, QImageReader::imageFormat(inFile), 100))
                        return -4;
                } else {
                    qDebug() << "pix is null";
                    return -5;
                }
            }
        }
    }
    return 0;
}
