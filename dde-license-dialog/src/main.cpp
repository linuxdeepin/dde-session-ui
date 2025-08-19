// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "mainwindow.h"
#if (defined QT_DEBUG) && (defined CHECK_ACCESSIBLENAME)
#include "../common/accessibilitycheckerex.h"
#endif

#include <DApplication>
#include <DGuiApplicationHelper>

#include <QTranslator>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QFile>
#include <QDebug>

DWIDGET_USE_NAMESPACE
DGUI_USE_NAMESPACE

int main(int argc, char *argv[])
{
    qputenv("DSG_APP_ID", "org.deepin.dde.license-dialog");
    DApplication a(argc, argv);

    a.loadTranslator();

    QTranslator translator;
    if (translator.load(QLocale::system(), "dde-session-ui", "_", "/usr/share/dde-session-ui/translations")) {
        a.installTranslator(&translator);
    }

    QCommandLineOption title(QStringList() << "t" << "title", "Parm:string type  \n Action:Show Chinese title name");
    QCommandLineOption entitle(QStringList() << "u" << "english title", "Parm:string type   \n Action:Show English title name");
    QCommandLineOption content(QStringList() << "c" << "content", "Parm:string type   \n Action:Show English title name  \n Notice:Set absolute path of document you want to show");
    QCommandLineOption checkbox(QStringList() << "a" << "allow", "Parm:string type \n Action:Show checkbox and allow content in Chinese");
    QCommandLineOption encheckbox(QStringList() << "b" << "english allow", "Parm:string type \n Action:Show checkbox and allow content in English");
    QCommandLineOption encontent(QStringList() << "e" << "english content", "Parm:string type \n Action:Show content in English  \n Notice:Set absolute path of document you want to show");
    QCommandLineOption hidebottom(QStringList() << "d" << "hide bottom", "Parm:string type  \n Action:Show bottom if param set \"yes\"  \n Notice:Bottom include allow checkbox,confirm button,cancel bottom");
    QCommandLineOption theme(QStringList() << "p" << "palette", "Parm:string type \n Action:Set application theme \n Values: light, dark, auto (default: follow system)");

    title.setValueName("TitleName");
    entitle.setValueName("EnTitleName");
    content.setValueName("Content");
    checkbox.setValueName("Check");
    encheckbox.setValueName("EnCheck");
    encontent.setValueName("EnContent");
    hidebottom.setValueName("hidebottom");
    theme.setValueName("PaletteType");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addOption(title);
    parser.addOption(entitle);
    parser.addOption(content);
    parser.addOption(checkbox);
    parser.addOption(encheckbox);
    parser.addOption(encontent);
    parser.addOption(hidebottom);
    parser.addOption(theme);
    parser.process(a);

    if (parser.isSet(theme)) {
        QString themeValue = parser.value(theme).toLower();
        if (themeValue == "light") {
            DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::LightType);
        } else if (themeValue == "dark") {
            DGuiApplicationHelper::instance()->setPaletteType(DGuiApplicationHelper::DarkType);
        }
    }

    MainWindow w;

    if (parser.isSet(title)) {
        w.setTitle(parser.value(title));
    }

    if (parser.isSet(entitle)) {
        w.setEnTitle(parser.value(entitle));
    } else {
        w.setEnTitle(parser.value(title));// 照顾以前的版本,英文未设置使用中文
    }

    if (parser.isSet(content)) {
        w.setCnSource(parser.value(content));
    }

    if (parser.isSet(encontent)) {
        w.setEnSource(parser.value(encontent));
    }

    if (parser.isSet(checkbox)) {
        w.setAllowCheckBoxText(parser.value(checkbox));
    }

    if (parser.isSet(encheckbox)) {
        w.setEnAllowCheckBoxText(parser.value(encheckbox));
    } else {
        w.setEnAllowCheckBoxText(parser.value(checkbox));// 照顾以前的版本,英文未设置使用中文
    }

    if (parser.isSet(hidebottom)) {
        w.setHideBottom(parser.value(hidebottom));
    }

    w.updateLocaleSource();
#if (defined QT_DEBUG) && (defined CHECK_ACCESSIBLENAME)
    AccessibilityCheckerEx checker;
    checker.setOutputFormat(DAccessibilityChecker::FullFormat);
    checker.start();
#endif
    w.moveToCenter();
    w.show();


    return a.exec();
}
