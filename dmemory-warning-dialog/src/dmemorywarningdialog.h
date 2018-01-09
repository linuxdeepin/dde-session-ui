#ifndef DMEMORYWARNINGDIALOG_H
#define DMEMORYWARNINGDIALOG_H

#include <DMainWindow>

DWIDGET_USE_NAMESPACE

class ProcessInfoModel;

class DMemoryWarningDialog : public DMainWindow
{
    Q_OBJECT

public:
    explicit DMemoryWarningDialog(QWidget *parent = nullptr);

private:
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    ProcessInfoModel *m_infoModel;
};

#endif // DMEMORYWARNINGDIALOG_H
