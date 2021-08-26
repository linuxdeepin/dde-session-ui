#include <gtest/gtest.h>

#define private public
#include "kblayoutindicator.h"
#undef private

class UT_Kblayoutindicator : public testing::Test
{
public:
    void SetUp() override
    {
        obj = new KBLayoutIndicator();
    }

    void TearDown() override
    {
        delete obj;
        obj = nullptr;
    }

public:
    KBLayoutIndicator *obj = nullptr;
};

TEST_F(UT_Kblayoutindicator, coverageTest) {
    Q_EMIT obj->m_data->m_keyboardInter->UserLayoutListChanged(QStringList() << "en");
    Q_EMIT obj->m_data->m_keyboardInter->CurrentLayoutChanged("en");
    Q_EMIT obj->m_data->m_keyboardInter->serviceValidChanged(true);
}
