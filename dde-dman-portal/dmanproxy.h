#pragma once

#include <QObject>

#define SERVICE_NAME "com.deepin.dman"
#define SERVICE_PATH "/com/deepin/dman"

class DManProxy : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dman")
public:
    explicit DManProxy(QObject *parent = 0);

Q_SIGNALS:

public Q_SLOTS:
    Q_INVOKABLE void ShowManual(const QString appid);
};

