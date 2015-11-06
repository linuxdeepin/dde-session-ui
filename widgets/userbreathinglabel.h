#ifndef USERBREATHINGLABEL
#define USERBREATHINGLABEL

#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QTimer>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QObject>

class UserBreathingLabel:public QLabel
{
    Q_OBJECT
    Q_PROPERTY(double opacity READ opacity WRITE setOpacity NOTIFY opacityChanged)
public:

    UserBreathingLabel(QWidget *parent=0);
    ~UserBreathingLabel();
    QGraphicsOpacityEffect* label_effect;
    QTimer* timer_fadeIn;
    QTimer* timer_fadeOut;
    double opacity() const;
signals:
    void opacityChanged();
    void showFinished();
    void hideFinished();
public slots:
    void showLabel();
    void hideLabel();

    void setOpacity(double opa);
    void setEffect();
private:
    double m_opacity;

    QPropertyAnimation* m_showOpacityAnimation;
    QPropertyAnimation* m_hideOpacityAnimation;
};
#endif // USERBREATHINGLABEL

