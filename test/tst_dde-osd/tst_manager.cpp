#include <gtest/gtest.h>

#include "manager.h"

#include <QApplication>
#include <QMimeData>

class TstManager : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new Manager();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    Manager *obj = nullptr;
};

TEST_F(TstManager, coverageTest)
{

    QStringList argList;
    argList << "AudioUp" << "AudioDown" << "AudioMute";
    argList << "BrightnessUp" << "BrightnessDown";
    argList << "CapsLockOn" << "CapsLockOff";
    argList << "NumLockOn" << "NumLockOff";
    argList << "TouchpadOn" << "TouchpadOff"<< "TouchpadToggle";
    argList << "SwitchWM3D" << "SwitchWM2D"<< "SwitchWMError";
    argList << "AudioMicMuteOn" << "AudioMicMuteOff";
    argList << "WLANOn" << "WLANOff";

    foreach(QString arg , argList) {
        obj->ShowOSD(arg);
    }
}
