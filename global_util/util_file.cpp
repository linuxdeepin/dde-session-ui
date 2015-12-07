#include "util_file.h"

UtilFile::UtilFile(QObject *parent)
    : QObject(parent) {

}

UtilFile::~UtilFile() {
}

int UtilFile::getExpandState() {
    //expand UserWidget
    QFile file(DDESESSIONCC::LOCK_SHUTDOWN_FLAG);
    if (file.exists()) {
        qDebug() << "exist file open:" << file.open(QIODevice::ReadOnly);
        QTextStream out(&file);
        int stateIndex = out.readAll().trimmed().toInt();
        file.close();
        return stateIndex;
    } else {
        return 0;
    }
}

void UtilFile::setExpandState(int stateIndex) {
    QFile file(DDESESSIONCC::LOCK_SHUTDOWN_FLAG);
    qDebug() << "file open:" << file.open(QIODevice::ReadWrite|QIODevice::Truncate);
    file.setPermissions(DDESESSIONCC::LOCK_SHUTDOWN_FLAG, QFile::ReadOwner|QFile::WriteOwner
                        |QFile::ReadGroup|QFile::WriteGroup|QFile::ReadOther|QFile::WriteOther
                        |QFile::ReadUser|QFile::WriteUser);

    QTextStream in(&file);
    in << stateIndex;
    file.close();
}
