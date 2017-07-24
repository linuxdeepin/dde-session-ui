#ifndef VMFRAME_H
#define VMFRAME_H

#include "boxframe.h"
#include "wmchooser.h"
#include <QWidget>

class WMFrame : public BoxFrame
{
    Q_OBJECT
public:
    explicit WMFrame(QWidget *parent = 0);
    void setConfigPath(const QString &path);
    void updateScreenPosition();

private:
    WMChooser *m_wmchooser;
};

#endif // VMFRAME_H
