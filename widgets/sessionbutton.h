#ifndef SESSIONBUTTON
#define SESSIONBUTTON
#include <QObject>
#include <QtWidgets/QFrame>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLabel>
#include <QtWidgets>
#include <QtGui/QFocusEvent>
#include <QtWidgets/QGraphicsDropShadowEffect>

#include "util_signalmanager.h"

class SessionButton: public QPushButton
{
    Q_OBJECT
public:
    SessionButton(QString text, QString buttonId, QWidget *parent=0);
    ~SessionButton();

    bool isChecked() const;
    void setHover(bool isHover);
public slots:
    void setChecked(bool checked);

    void setButtonMutex(QString buttonName);
    void setButtonHoverMutex(QString buttonName);
    inline QString buttonId() {
        return m_iconLabel->objectName();
    }

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
signals:
    void buttonAction(QString id);

    void updateStyle();
private:
    bool m_checked = false;
    QString m_buttonId;
    QLabel* m_iconLabel;
    QLabel* m_contentTextLabel;
    QVBoxLayout* m_Layout;
    QHBoxLayout* m_ButtonLayout;
    void initConnect();
    void addTextShadow();
};
#endif // SESSIONBUTTON
