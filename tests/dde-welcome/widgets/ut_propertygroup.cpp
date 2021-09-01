#include <gtest/gtest.h>

#define private public
#include "propertygroup.h"
#undef private

#include <QSignalMapper>
#include <QVariant>
#include <QMetaObject>
#include <QMetaProperty>

class TstPropertyGroup : public testing::Test
{
public:
    void SetUp() override
    {
        propertyGroup = new PropertyGroup();
    }

    void TearDown() override
    {
        delete propertyGroup;
        propertyGroup = nullptr;
    }

public:
    PropertyGroup *propertyGroup = nullptr;
};

TEST_F(TstPropertyGroup, coverageTest)
{
    QObject obj, obj1;
    obj.setProperty("hello", QVariant(true));
    obj1.setProperty("hello", QVariant(false));
    propertyGroup->addProperty("hello");
    propertyGroup->addObject(&obj);
    propertyGroup->addObject(&obj1);

    auto mapper = propertyGroup->m_signalMapperMap["hello"];
    if (mapper) {
        Q_EMIT mapper->mapped(&obj);
    }

    propertyGroup->removeProperty("hello");
    propertyGroup->removeObject(&obj);
}
