#ifndef ICONDATA_H
#define ICONDATA_H

#include <QDBusArgument>

class IconData {

public:
    friend QDebug operator<<(QDebug arg, const IconData &data);
    friend QDBusArgument &operator<<(QDBusArgument &arg, const IconData &data);
    friend const QDBusArgument &operator>>(const QDBusArgument &arg, IconData &data);

public:
    int width;
    int height;
    int rowstride;
    bool alpha;
    int bit;
    int cannel;
    QByteArray array;
};

#endif // ICONDATA_H
