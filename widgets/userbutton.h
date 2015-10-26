#ifndef USERBUTTON_H
#define USERBUTTON_H

#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QtGui/QPaintEvent>
#include <QVBoxLayout>
#include <QObject>
#include <QLabel>
#include <QPropertyAnimation>

#include "userbreathinglabel.h"
#include "useravatar.h"

class UserButton:public QPushButton
{
    Q_OBJECT
public:
    UserButton(QString iconUrl, QString idName, QWidget* parent=0);
    ~UserButton();

    enum AvatarSize {
        AvatarSmallSize = 90,
        AvatarLargerSize = 110,
    };

signals:
    void imageClicked(QString nam);

public slots:
    void sendClicked();
    void setImageSize(const AvatarSize &avatarsize);

    void hide(const int duration = 0);
    void move(const QPoint &position, const int duration = 0);
    void showButton();
    void hideButton();

private:
    void initUI();
    void initConnect();
    void addTextShadow();

    bool m_checked = false;
    QString m_iconUrl;
    QString m_buttonId;
    UserAvatar* m_userAvatar;
    UserBreathingLabel* m_textLabel;
    QHBoxLayout* m_buttonLayout;
    QVBoxLayout* m_Layout;

    AvatarSize m_avatarsize = AvatarLargerSize;
    QPropertyAnimation *m_moveAni;
    int m_borderWidth = 4;
};
#endif // UserButton

