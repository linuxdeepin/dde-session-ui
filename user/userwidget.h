#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtWidgets/QHBoxLayout>
#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QKeyEvent>

#include "user/imagebutton.h"
class UserWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)
public:
    UserWidget(QWidget *parent = 0);
    ~UserWidget();

public slots:
    void addUser(QString url, QString name);
    void loginInUser(QString nam);
    void initConnect();
    void setGeometry(const QRect &rect);
    void showWidget();
    void hideWidget();
//    void keyPressEvent(QKeyEvent *e);
signals:
    void selectedUser(QString name);
private:
    bool isBigAvatar = false;
    QString m_currentUser = "administrator";
    int m_finalX = 0;
    int m_finalY = 0;
    QHBoxLayout* m_Layout;
    QStringList userList;
    QPropertyAnimation* m_showAnimation;
    QPropertyAnimation* m_hideAnimation;
    const int USER_ICON_SIZE = 120;
};

#endif // WIDGET_H
