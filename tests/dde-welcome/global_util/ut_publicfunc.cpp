#include <gtest/gtest.h>

#include "public_func.h"

#include <QFile>

class UtPublicFunc : public testing::Test
{
public:
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

TEST_F(UtPublicFunc, coverageTest)
{
    QString bgPath = "/usr/share/wallpapers/deepin/desktop.jpg";
    if (!QFile::exists(bgPath)) {
        bgPath = "/usr/share/backgrounds/default_background.jpg";
    }
    loadPixmap("bgPath");
}
