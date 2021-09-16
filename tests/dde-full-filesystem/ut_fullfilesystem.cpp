#define private public
#include "fullfilesystem.h"
#undef private

#include <QTest>

#include <gtest/gtest.h>
#include <unistd.h>


class UT_FullFileSystem : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new FullFilesystem();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    FullFilesystem *obj = nullptr;
};

TEST_F(UT_FullFileSystem, coverageTest)
{
    QTest::qWait(200);
    User user("com.deepin.daemon.Accounts", "/com/deepin/daemon/Accounts/User" + QString::number((getuid())), QDBusConnection::systemBus(), obj);
    obj->setCurrentUser(&user);
    obj->onRecoveryButtonClicked();
    obj->onPoweroffButtonClicked();
    obj->onBlurWallpaperFinished(user.greeterBackground(), user.greeterBackground(), true);
}
