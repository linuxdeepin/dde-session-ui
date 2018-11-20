#include "framedatabind.h"

FrameDataBind::FrameDataBind()
    : QObject()
{

}

FrameDataBind::~FrameDataBind()
{

}

FrameDataBind *FrameDataBind::Instance()
{
    static FrameDataBind frameDataBind;
    return &frameDataBind;
}

void FrameDataBind::registerFunction(const QString &flag, std::function<void (QVariant)> function)
{
    m_registerList[flag] << function;
}

void FrameDataBind::updateValue(const QString &flag, const QVariant &value)
{
    QList<std::function<void (QVariant)>> functions = m_registerList[flag];

    for (auto it = functions.constBegin(); it != functions.constEnd(); ++it) {
        (*it)(value);
    }
}
