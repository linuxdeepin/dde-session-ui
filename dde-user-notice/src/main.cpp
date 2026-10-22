// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "fullscreen.h"
#include "fullscreenmanager.h"
#include "noticeconfig.h"
#include "preLoginWindow.h"

#include <DApplication>
#include <DGuiApplicationHelper>
#include <LogManager.h>

#include <QFile>
#include <QLocale>
#include <QScreen>
#include <QTranslator>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE
DCORE_USE_NAMESPACE

namespace {

const QString translationDir = QStringLiteral("/usr/share/dde-session-ui/translations");

void setupPalette()
{
    DPalette pa = DGuiApplicationHelper::instance()->standardPalette(DGuiApplicationHelper::LightType);
    pa.setColor(QPalette::Normal, DPalette::WindowText, QColor("#FFFFFF"));
    pa.setColor(QPalette::Normal, DPalette::Text, QColor("#FFFFFF"));
    pa.setColor(QPalette::Normal, DPalette::AlternateBase, QColor(0, 0, 0, 76));
    pa.setColor(QPalette::Normal, DPalette::Button, QColor(255, 255, 255, 76));
    pa.setColor(QPalette::Normal, DPalette::Light, QColor(255, 255, 255, 76));
    pa.setColor(QPalette::Normal, DPalette::Dark, QColor(255, 255, 255, 76));
    pa.setColor(QPalette::Normal, DPalette::ButtonText, QColor("#FFFFFF"));

    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::WindowText, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::Text, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::AlternateBase, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::Button, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::Light, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::Dark, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::generatePaletteColor(pa, DPalette::ButtonText, DGuiApplicationHelper::LightType);
    DGuiApplicationHelper::instance()->setApplicationPalette(pa);
}

} // namespace

int main(int argc, char *argv[])
{
    if (!QString(qgetenv("XDG_CURRENT_DESKTOP")).toLower().startsWith("deepin")) {
        setenv("XDG_CURRENT_DESKTOP", "Deepin", 1);
    }

    DApplication app(argc, argv);
    app.setOrganizationName("deepin");
    app.setApplicationName("dde-user-notice");
    app.setApplicationVersion("2015.1.0");

    // 多屏插拔时没有屏幕不应导致进程退出
    QApplication::setQuitOnLastWindowClosed(false);

#ifdef QT_DEBUG
    DLogManager::registerConsoleAppender();
#endif
    DLogManager::registerJournalAppender();

    QTranslator translator;
    if (translator.load(QLocale::system(), "dde-session-ui", "_", translationDir)) {
        app.installTranslator(&translator);
    }

    DGuiApplicationHelper::setAttribute(DGuiApplicationHelper::UseInactiveColorGroup, false);
    setupPalette();

    const QString userNoticePath = UserNoticeConfig::noticePath();
    QFile instructionFile(userNoticePath);
    if (!instructionFile.exists() || instructionFile.size() <= 0
            || !instructionFile.open(QIODevice::ReadOnly)) {
        qWarning() << "User notice file is not readable:" << userNoticePath;
        return 1;
    }
    instructionFile.close();

    auto createFrame = [](QPointer<QScreen> screen) -> QWidget * {
        Q_UNUSED(screen)
        // 所有屏幕共用同一块内容
        return new FullScreen(PreLoginWindow::instance());
    };

    FullScreenManager manager(createFrame);

    return app.exec();
}
