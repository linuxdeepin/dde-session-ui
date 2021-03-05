#include <gtest/gtest.h>

#include "updatecontent.h"

#include <QApplication>
#include <QMimeData>

class TstUpdateContent : public testing::Test
{
public:
    void SetUp() override
    {
//        std::pair<QString, QString> pair;
//        obj = new UpdateContent(pair);
    }

    void TearDown() override
    {
//        delete obj;
//        obj = nullptr;
    }

public:
    UpdateContent *obj = nullptr;
};

TEST_F(TstUpdateContent, coverageTest)
{

}
