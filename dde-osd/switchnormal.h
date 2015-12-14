#ifndef SWITCHNORMAL_H
#define SWITCHNORMAL_H

#include "dbus/volumedbus.h"
#include <QWidget>
#include <QSvgWidget>
#include <QLabel>

class SwitchNormal : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchNormal(QWidget *parent = 0);

    double getVolume();
    void loadBasicImage(QString whichImage);
    void searchAddedImage(QString iconName);
    void hideNormal();
    void showNormal();
    void cancelText();
    void showText(QString text);
private:
    void initGlobalVars(QWidget *parent);
    void initBasicOperation();

    QDBusInterface* m_AudioInterface;
    VolumeDbus* m_VolumeInterface;
    QWidget* m_ParentItem;
    QSvgWidget* m_NormalImageSvg;
    QLabel* m_NormalImageLabel;
    QLabel* m_SwitchWMLabel;

    int m_CanAudioMuteRun;
    int m_AudioMuteNotRunFromAudioMute;

    const int BASE_SIZE = 140;
    const int IMAGE_SIZE = 56;
    const QRect SWITCHWM_TEXT_GEOMETRY = QRect(16,96,108,34);
    const QPoint SWITCHWM_IMAGE_POINT = QPoint(42,30);
    const QString SWITCHWM_TEXT_STYLE = "QLabel{font-size:14px;color:#ffffff}";
};

#endif // SWITCHNORMAL_H
