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

int FrameDataBind::registerFunction(const QString &flag, std::function<void (QVariant)> function)
{
    for (auto it = m_registerList.begin(); it != m_registerList.end(); ++it) {
        if (it.key() == flag) {
            QList<int> keys = it.value().keys();
            int index = 0;
            while (true) {
                if (!keys.contains(index)) {
                    it.value()[index] = function;
                    return index;
                }

                ++index;
            }
        }
    }

    m_registerList[flag][0] = function;
    return 0;
}

void FrameDataBind::unRegisterFunction(const QString &flag, int index)
{
    for (auto it = m_registerList.begin(); it != m_registerList.end(); ++it) {
        if (it.key() == flag) {
            it.value().remove(index);
        }
    }
}

void FrameDataBind::updateValue(const QString &flag, const QVariant &value)
{
    m_datas[flag] = value;

    refreshData(flag);
}

void FrameDataBind::refreshData(const QString &flag)
{
    QList<std::function<void (QVariant)>> functions = m_registerList[flag].values();

    for (auto it = functions.constBegin(); it != functions.constEnd(); ++it) {
        (*it)(m_datas[flag]);
    }
}
