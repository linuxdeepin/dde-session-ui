#ifndef UPDATE_H
#define UPDATE_H

#include "fullscreenbackground.h"
#include <QWidget>

class Update : public FullscreenBackground
{
    Q_OBJECT
public:
    explicit Update(const QString &version, QWidget *parent = nullptr);
};

#endif // UPDATE_H
