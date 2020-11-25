#include <gtest/gtest.h>

#include "notification/persistence.h"

#include <QApplication>
#include <QMimeData>

class TstPersistence : public testing::Test
{
public:
    void SetUp() override
    {
        widget = new Persistence();
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    Persistence *widget = nullptr;
};

TEST_F(TstPersistence, coverageTest)
{

}
