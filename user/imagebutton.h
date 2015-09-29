#ifndef IMAGEBUTTON
#define IMAGEBUTTON
#include <QtWidgets/QPushButton>
#include <QtGui/QPaintEvent>

#include <QObject>
class ImageButton:public QPushButton
{
    Q_OBJECT
public:
    ImageButton(QString url, QString name, QWidget *parent=0);
    ~ImageButton();
    void paintEvent(QPaintEvent *);
    enum AvatarSize {
        AvatarSmallSize,
        AvatarBigSize,
    };

signals:
    void clicked(QString nam);
public slots:
    void hideIn(QString name);
    void showOut();
    void sendClicked();
    void setIconSize(const AvatarSize &avatarsize);
private:
    QString m_url;
    QString m_name;
    QString m_objectname;
    AvatarSize m_avatarsize = AvatarSmallSize;
    int m_borderWidth = 4;
    const int SMALL_ICON_SIZE = 80;
    const int LARGE_ICON_SIZE = 100;
};
#endif // IMAGEBUTTON

