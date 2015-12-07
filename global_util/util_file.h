#ifndef UTIL_FILE
#define UTIL_FILE
#include <QFile>
#include <QObject>
#include <QDebug>

#include "constants.h"

class UtilFile:public QObject {
    Q_OBJECT
public:
    UtilFile(QObject* parent=0);
    ~UtilFile();

    int getExpandState();
    void setExpandState(int stateIndex);
};
#endif // UTIL_FILE

