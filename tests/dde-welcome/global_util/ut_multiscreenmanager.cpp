#include <gtest/gtest.h>

#define private public
#include "multiscreenmanager.h"
#include "mainwidget.h"
#undef private

#include <QApplication>

class Ut_MultiScreenManager : public testing::Test
{
public:
    void SetUp() override
    {
        manager = new MultiScreenManager();
    }

    void TearDown() override
    {
        delete manager;
        manager = nullptr;
    }

public:
    MultiScreenManager *manager = nullptr;
};

TEST_F(Ut_MultiScreenManager, coverageTest)
{
    auto createFrame = [&] (QScreen *screen) -> QWidget* {
        MainWidget *w = new MainWidget;
        w->setScreen(screen);
        return w;
    };
    manager->register_for_mutil_screen(createFrame);
    if (!qApp->screens().isEmpty()) {
        manager->onScreenRemoved(qApp->screens().first());
    }
}
