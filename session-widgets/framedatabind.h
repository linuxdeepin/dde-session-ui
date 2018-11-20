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

    void registerFunction(const QString &flag, std::function<void (QVariant)> function);

    void updateValue(const QString &flag, const QVariant &value);

private:
    FrameDataBind();
    ~FrameDataBind();
    FrameDataBind(const FrameDataBind &) = delete;

private:
    QMap<QString, QList<std::function<void (QVariant)>>> m_registerList;
};

#endif // FRAMEDATABIND_H
