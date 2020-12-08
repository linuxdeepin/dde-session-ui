#include <gtest/gtest.h>

#include "notification/persistence.h"

#include <QApplication>
#include <QMimeData>

class TstPersistence : public testing::Test
{
public:
    void SetUp() override
    {
//        obj = new Persistence();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    Persistence *obj = nullptr;
};

TEST_F(TstPersistence, coverageTest)
{

}
