#ifndef DMEMORYWARNINGDIALOG_H
#define DMEMORYWARNINGDIALOG_H

#include <DMainWindow>
#include <QPushButton>
#include <QLabel>

DWIDGET_USE_NAMESPACE

class ProcessInfoModel;

class DMemoryWarningDialog : public DMainWindow
{
    Q_OBJECT

public:
    explicit DMemoryWarningDialog(QWidget *parent = nullptr);

public slots:
    void updateAppInfo(const QString &appInfo);

private:
    void keyPressEvent(QKeyEvent *e);
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    ProcessInfoModel *m_infoModel;
    QLabel *m_icon;
    QLabel *m_appName;
    QLabel *m_tips;
    QPushButton *m_cancelButton;
    QPushButton *m_continueButton;
};

#endif // DMEMORYWARNINGDIALOG_H
