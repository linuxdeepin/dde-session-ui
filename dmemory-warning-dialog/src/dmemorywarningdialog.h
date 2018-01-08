#ifndef DMEMORYWARNINGDIALOG_H
#define DMEMORYWARNINGDIALOG_H

#include <DMainWindow>

DWIDGET_USE_NAMESPACE
class DMemoryWarningDialog : public DMainWindow
{
    Q_OBJECT

public:
    explicit DMemoryWarningDialog(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // DMEMORYWARNINGDIALOG_H
