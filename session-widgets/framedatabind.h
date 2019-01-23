#ifndef FRAMEDATABIND_H
#define FRAMEDATABIND_H

#include <QObject>
#include <functional>
#include <QVariant>

class FrameDataBind : public QObject
{
    Q_OBJECT
public:
    static FrameDataBind *Instance();

    int registerFunction(const QString &flag, std::function<void (QVariant)> function);
    void unRegisterFunction(const QString &flag, int index);

    void updateValue(const QString &flag, const QVariant &value);

    void refreshData(const QString &flag);

private:
    FrameDataBind();
    ~FrameDataBind();
    FrameDataBind(const FrameDataBind &) = delete;

private:
    QMap<QString, QMap<int, std::function<void (QVariant)>>> m_registerList;
    QMap<QString, QVariant> m_datas;
};

#endif // FRAMEDATABIND_H
