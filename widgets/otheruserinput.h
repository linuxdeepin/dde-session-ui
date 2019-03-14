#ifndef OTHERUSERINPUT_H
#define OTHERUSERINPUT_H

#include "errortooltip.h"

#include <QFrame>
#include <QLineEdit>
#include <dimagebutton.h>

DWIDGET_USE_NAMESPACE

class KeyboardMonitor;
class OtherUserInput : public QFrame
{
    Q_OBJECT
public:
    explicit OtherUserInput(QWidget *parent = nullptr);

    inline const QString account() const { return m_accountEdit->text(); }
    inline const QString passwd() const { return m_passwdEdit->text(); }

signals:
    void submit();

public slots:
    void setAlert(const QString &alert);
    void setAccount(const QString &username);
    void clearAlert() const;

protected:
    void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    void initUI();
    void initConnect();

private:
    QLineEdit *m_accountEdit;
    QLineEdit *m_passwdEdit;
    DImageButton *m_submitBtn;
    ErrorTooltip *m_errorTip;
    QLabel *m_capslockWarning;
    KeyboardMonitor *m_capslockMonitor;
};

#endif // OTHERUSERINPUT_H
