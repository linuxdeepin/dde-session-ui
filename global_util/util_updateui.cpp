#include "util_updateui.h"

void updateStyle(QString styleFiles, QWidget* widget){
    QFile qssFile(QString("%1").arg(styleFiles));

    QString qss;
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen()) {
        qss = QLatin1String(qssFile.readAll());
        widget->setStyleSheet(qss);
        qssFile.close();
    }
}

