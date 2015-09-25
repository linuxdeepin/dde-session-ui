#ifndef SESSIONBUTTON
#define SESSIONBUTTON

#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets>
#include <QtGui>

class SessionButton: public QFrame
{
    Q_OBJECT
public:
    SessionButton(QString text, QString buttonId, QWidget *parent=0);
    ~SessionButton();

public slots:

private:
    QPushButton* m_contentButton;
    QLabel* m_contentTextLabel;
    QVBoxLayout* m_Layout;
};
#endif // SESSIONBUTTON

