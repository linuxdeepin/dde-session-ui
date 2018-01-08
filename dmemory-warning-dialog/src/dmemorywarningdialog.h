#ifndef DMEMORYWARNINGDIALOG_H
#define DMEMORYWARNINGDIALOG_H

#include <QMainWindow>

class DMemoryWarningDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit DMemoryWarningDialog(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent *e);
};

#endif // DMEMORYWARNINGDIALOG_H
