#include "warningdialog.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QIcon>

WarningDialog::WarningDialog(QWidget *parent)
    : DDialog(parent)
{
    const QString title = tr("Kindly Reminder");
    const QString body = tr("This application can not run without window effect");
    setTitle(title);
    setMessage(body);
    setIconPixmap(QIcon::fromTheme("dialog-warning").pixmap(48, 48));
    addButton(tr("OK"), true);

    const QDesktopWidget *desktop = QApplication::desktop();
    const QPoint cursorPoint = QCursor::pos();
    const QRect geom = desktop->screenGeometry(cursorPoint);

    move(geom.width() - width() / 2,
         geom.height() - height() / 2);

}

WarningDialog::~WarningDialog()
{
}
