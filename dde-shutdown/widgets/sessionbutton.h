#ifndef SESSIONBUTTON
#define SESSIONBUTTON
#include <QObject>
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets>
#include <QtGui/QFocusEvent>
#include <QtWidgets/QGraphicsDropShadowEffect>

#include "signalmanagement/signalmanager.h"

class SessionButton: public QFrame
{
    Q_OBJECT
public:
    SessionButton(QString text, QString buttonId, QWidget *parent=0);
    ~SessionButton();

    bool isChecked() const;
    QString buttonId();
public slots:
    void sendSignal();
    void setChecked(bool checked);
    void setButtonMutex(QString buttonName);
    void setButtonChecked();
protected:
    void paintEvent(QPaintEvent *event);
signals:
    void buttonAction(QString id);
private:
    bool m_checked = false;
    QString m_buttonId;
    QPushButton* m_contentButton;
    QLabel* m_contentTextLabel;
    QVBoxLayout* m_Layout;
    QHBoxLayout* m_ButtonLayout;
    void initConnect();
    void addNameShadow();
};
#endif // SESSIONBUTTON
