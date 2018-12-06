#ifndef LOCKPASSWORDWIDGET_H
#define LOCKPASSWORDWIDGET_H

#include <QWidget>

class LockPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LockPasswordWidget(QWidget *parent = nullptr);

    void setMessage(const QString &message);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_message;
};

#endif // LOCKPASSWORDWIDGET_H
