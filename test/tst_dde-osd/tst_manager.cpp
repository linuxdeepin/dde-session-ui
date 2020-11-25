#include <gtest/gtest.h>

#include "manager.h"

#include <QApplication>
#include <QMimeData>

class TstManager : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new Manager();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    Manager *widget = nullptr;
};

TEST_F(TstManager, coverageTest)
{

}
