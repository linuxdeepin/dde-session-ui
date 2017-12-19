#ifndef OTHERUSERINPUT_H
#define OTHERUSERINPUT_H

#include <QFrame>
#include <QLineEdit>
#include <dimagebutton.h>

DWIDGET_USE_NAMESPACE

class OtherUserInput : public QFrame
{
    Q_OBJECT
public:
    explicit OtherUserInput(QWidget *parent = nullptr);

    inline const QString account() const { return m_accountEdit->text(); }
    inline const QString passwd() const { return m_passwdEdit->text(); }

signals:
    void submit();

private:
    void initUI();
    void initConnect();

private:
    QLineEdit *m_accountEdit;
    QLineEdit *m_passwdEdit;
    DImageButton *m_submitBtn;
};

#endif // OTHERUSERINPUT_H
