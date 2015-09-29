#ifndef USERFRAME_H
#define USERFRAME_H
#include <QWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QObject>
#include <QtCore/QTimer>
#include <QtCore/QPropertyAnimation>
#include <QtGui/QKeyEvent>

#include "user/imagebutton.h"

class UserFrame: public QFrame {
    Q_OBJECT
public:
    UserFrame(QWidget *parent=0);
    ~UserFrame();
signals:
    void selectedUser(QString name);
public slots:
    void addUser(QString url, QString name);
private:
    ImageButton* m_userImageButton;
    QLabel* m_userName;
    QStringList userList;
    QHBoxLayout* m_userNameLayout;
    QVBoxLayout* m_Layout;
};
#endif // USERFRAME_H
