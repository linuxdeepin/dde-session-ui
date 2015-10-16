#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QKeyEvent>
#include <QFrame>

#include "userbutton.h"
class UserWidget : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)
public:
    UserWidget(QWidget* parent = 0);
    ~UserWidget();
signals:
    void selectedUser(QString name);
    void sizeUpdate();
public slots:
    void addUser(QString url, QString name);
    void loginInUser(QString nam);

    void setGeometry(QRect rect);
    void expandWidget();
    void packUpWidget();

    void setCurrentUser();
    void setUserAvatarSamll();
    void testing(QString em);
//    void keyPressEvent(QKeyEvent *e);

private:
    void initUI();
    void initConnect();

    bool isLargerAvatar = false;
    QString m_currentUser = "administrator";
    int m_finalX = 0;
    int m_finalY = 0;
    QHBoxLayout* m_Layout;
    QStringList userList;

    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;
    const int USER_ICON_WIDTH = 140;
    const int USER_ICON_HEIGHT = 150;
};

#endif // WIDGET_H
