#include <gtest/gtest.h>

#include "updatecontent.h"

#include <QApplication>
#include <QMimeData>

class TstUpdateContent : public testing::Test
{
public:
    void SetUp() override
    {
        std::pair<QString, QString> pair;
        widget = new UpdateContent(pair);
    }

    void TearDown() override
    {
        delete widget;
        widget = nullptr;
    }

public:
    UpdateContent *widget = nullptr;
};

TEST_F(TstUpdateContent, coverageTest)
{

}
