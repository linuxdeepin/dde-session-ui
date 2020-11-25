#include <gtest/gtest.h>

#include "manager.h"

#include <QApplication>
#include <QMimeData>

class TstManager : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new Manager();
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    Manager *obj = nullptr;
};

TEST_F(TstManager, coverageTest)
{

}
