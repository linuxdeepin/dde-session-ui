#include "osd.h"
#include <QApplication>
#include <QScreen>
#include <QTimer>
#include <QTranslator>
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    translator.load("/usr/share/dde-session-ui/translations/dde-session-ui_" + QLocale::system().name());
    a.installTranslator(&translator);

    // command line options
    QCommandLineOption NumLockOn("NumLockOn", "\tOSD NumLockOn");
    QCommandLineOption NumLockOff("NumLockOff", "\tOSD NumLockOff");
    QCommandLineOption CapsLockOn("CapsLockOn", "\tOSD CapsLockOn");
    QCommandLineOption CapsLockOff("CapsLockOff", "\tOSD CapsLockOff");
    QCommandLineOption TouchpadOn("TouchpadOn", "\tOSD TouchpadOn");
    QCommandLineOption TouchpadOff("TouchpadOff", "\tOSD TouchpadOff");
    QCommandLineOption TouchpadToggle("TouchpadToggle", "\tOSD TouchpadToggle");
    QCommandLineOption BrightnessUp("BrightnessUp", "\tOSD BrightnessUp");
    QCommandLineOption BrightnessDown("BrightnessDown", "\tOSD BrightnessDown");
    QCommandLineOption AudioMute("AudioMute", "\tOSD AudioMute");
    QCommandLineOption AudioDown("AudioDown", "\tOSD AudioDown");
    QCommandLineOption AudioUp("AudioUp", "\tOSD AudioUp");
    QCommandLineOption SwitchMonitors("SwitchMonitors", "\tOSD SwitchMonitors");
    QCommandLineOption SwitchLayout("SwitchLayout", "\tOSD SwitchLayout");

    QList<QCommandLineOption> cmdOptions;
    cmdOptions << NumLockOn << NumLockOff << CapsLockOn << CapsLockOff << TouchpadOn << TouchpadOff
               << TouchpadToggle << BrightnessDown << BrightnessUp << AudioMute << AudioDown << AudioUp << SwitchLayout << SwitchMonitors;

    // command line parser
    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    parser.addHelpOption();
    parser.addOptions(cmdOptions);
    parser.process(a);

    Osd osd;

    // if app is new, register it's service to dbus, so that we can make sure that there is only one app running no matter what happened
    if (QDBusConnection::sessionBus().registerService("com.deepin.dde.osd")) {
        QDBusConnection::sessionBus().registerObject("/com/deepin/dde/osd", &osd, QDBusConnection::ExportAllSlots);

        if (parser.isSet(NumLockOn)) {
            osd.loadCorrespondingImage("NumLockOn");
        } else if (parser.isSet(NumLockOff)) {
            osd.loadCorrespondingImage("NumLockOff");
        } else if (parser.isSet(CapsLockOn)) {
            osd.loadCorrespondingImage("CapsLockOn");
        } else if (parser.isSet(CapsLockOff)) {
            osd.loadCorrespondingImage("CapsLockOff");
        } else if (parser.isSet(TouchpadOn)) {
            osd.loadCorrespondingImage("TouchpadOn");
        } else if (parser.isSet(TouchpadOff)) {
            osd.loadCorrespondingImage("TouchpadOff");
        } else if (parser.isSet(TouchpadToggle)) {
            osd.loadCorrespondingImage("TouchpadToggle");
        } else if (parser.isSet(BrightnessUp) || parser.isSet(BrightnessDown)) {
            osd.loadCorrespondingImage("Brightness");
        } else if (parser.isSet(AudioUp) || parser.isSet(AudioDown)) {
            osd.loadCorrespondingImage("Audio");
        } else if (parser.isSet(AudioMute)) {
            osd.loadCorrespondingImage("AudioMute");
        } else if (parser.isSet(SwitchLayout)) {
            osd.loadSwitchLayout();
        } else if (parser.isSet(SwitchMonitors)) {
            osd.loadSwitchMonitors();
        } else {
            // if not all of above, for example, no parameter for commandline, quit the app immediately
            return 0;
        }

        // make sure that app is always positioned in the center primary screen
        osd.moveToCenter();
        osd.show();
        // kill app after 2000 miliseconds
        osd.setTimer();
    } else {
        // if we have our app run, do not start another one. To register its service is a better way.
        QDBusInterface iface("com.deepin.dde.osd", "/com/deepin/dde/osd", "", QDBusConnection::sessionBus());

        if (parser.isSet(NumLockOff)) {
            iface.call("loadCorrespondingImage", "NumLockOff");
        } else if (parser.isSet(NumLockOn)) {
            iface.call("loadCorrespondingImage", "NumLockOn");
        } else if (parser.isSet(CapsLockOn)) {
            iface.call("loadCorrespondingImage", "CapsLockOn");
        } else if (parser.isSet(CapsLockOff)) {
            iface.call("loadCorrespondingImage", "CapsLockOff");
        } else if (parser.isSet(TouchpadOn)) {
            iface.call("loadCorrespondingImage", "TouchpadOn");
        } else if (parser.isSet(TouchpadOff)) {
            iface.call("loadCorrespondingImage", "TouchpadOff");
        } else if (parser.isSet(TouchpadToggle)) {
            iface.call("loadCorrespondingImage", "TouchpadToggle");
        } else if (parser.isSet(BrightnessUp) || parser.isSet(BrightnessDown)) {
            iface.call("loadCorrespondingImage", "Brightness");
        } else if (parser.isSet(AudioUp) || parser.isSet(AudioDown)) {
            iface.call("loadCorrespondingImage", "Audio");
        } else if (parser.isSet(AudioMute)) {
            iface.call("loadCorrespondingImage", "AudioMute");
        } else if (parser.isSet(SwitchLayout)) {
            QDBusReply<int> latterActionResult = iface.call("latterAction");
            latterActionResult.value() == 0 ? iface.call("highlightNextLayout") : iface.call("loadSwitchLayout");
        } else if (parser.isSet(SwitchMonitors)) {
            QDBusReply<int> latterActionResult = iface.call("latterAction");
            latterActionResult.value() == 1 ? iface.call("highlightNextMonitor") : iface.call("loadSwitchMonitors");
        }

        // tail_in_Work contains 3 actions: setTimer ,  moveToCenter and repaint
        iface.call("tail_in_Work");

        // if iface.call(*) , after call, quit this app immediately
        return 0;
    }

    return a.exec();
}
