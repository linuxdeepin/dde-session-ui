#ifndef USERBUTTON_H
#define USERBUTTON_H
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QtGui/QPaintEvent>
#include <QVBoxLayout>
#include <QObject>
#include <QLabel>

#include "useravatar.h"

class UserButton:public QPushButton
{
    Q_OBJECT
public:
    UserButton(QString iconUrl, QString idName, QWidget *parent=0);
    ~UserButton();

    enum AvatarSize {
        AvatarSmallSize = 90,
        AvatarLargerSize = 110,
    };

signals:
    void imageClicked(QString nam);
public slots:
    void hideIn(QString name);
    void showOut();
    void sendClicked();
    void setImageSize(const AvatarSize &avatarsize);
    void testing(bool is);

private:
    bool m_checked = false;
    QString m_iconUrl;
    QString m_buttonId;
    QLabel* m_iconLabel;
    QLabel* m_textLabel;
    QHBoxLayout* m_buttonLayout;
    QVBoxLayout* m_Layout;
    UserAvatar* m_userAvatar;
    AvatarSize m_avatarsize = AvatarLargerSize;
    int m_borderWidth = 4;

    void addTextShadow();
};
#endif // UserButton

