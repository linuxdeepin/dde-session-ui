#ifndef LOGINFRAME_H
#define LOGINFRAME_H

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFrame>
#include <QLabel>

#include "../boxframe/boxframe.h"
#include "loginmanager.h"

class LoginFrame : public BoxFrame
{
    Q_OBJECT

public:
    explicit LoginFrame(const QString url, QWidget* parent = 0);
    ~LoginFrame();
public slots:
    void updateScreenPosition(QRect rect);
protected:
    void keyPressEvent(QKeyEvent *e);
private:
    QList<QLabel*> otherScreenBackgroundList;
    LoginManager* m_loginManager;
};

#endif // LOGINFRAME_H
