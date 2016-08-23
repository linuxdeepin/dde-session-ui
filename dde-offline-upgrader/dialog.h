#ifndef DIALOG_H
#define DIALOG_H

#include <QDBusAbstractAdaptor>

#include <ddialog.h>

DWIDGET_USE_NAMESPACE

class Adaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.OfflineUpgrader")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.dde.OfflineUpgrader\">\n"
"    <method name=\"RaiseWindow\"/>\n"
"  </interface>\n"
        "")

public:
    Adaptor(QObject *parent);

public slots:
    void RaiseWindow();
};

class Dialog : public DDialog
{
	Q_OBJECT
public:
    Dialog();
};

#endif // DIALOG_H
