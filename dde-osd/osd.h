#ifndef OSD_H
#define OSD_H

#include "dbus/volumedbus.h"
#include "dbus/displaydbus.h"
#include "dbus/layoutdbus.h"
#include <QWidget>
#include <QLabel>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVariantAnimation>
#include <QList>
#include <QRect>
#include <QSvgWidget>


class Osd : public QWidget
{
    Q_OBJECT

public:
    Osd(QWidget *parent = 0);
    ~Osd();

    Q_SLOT void loadCorrespondingImage(QString whichImage);
    Q_SLOT void loadSwitchLayout();
    Q_SLOT void loadSwitchMonitors();
    Q_SLOT void highlightNextLayout();
    Q_SLOT void highlightNextMonitor();
    Q_SLOT void tail_in_Work();
    Q_SLOT void setTimer();
    Q_SLOT void moveToCenter();
    Q_SLOT int latterAction();

private:
    void initInterfaces();
    void initGlobalVars();
    void initBasicOperation();
    void initConnects();

    void paintEvent(QPaintEvent *e);

    QString getThemeIconPath(QString iconName);
    void showThemeImage(QString iconName, QSvgWidget* svgLoader, QLabel* notSvgLoader);

    // for keyboardlayout
    void calculateKeyboardSize();
    void initKeyboard();
    void reHiglightKeyboard();

    // for monitors
    void initMonitorItems();
    void initCurrentScreenMode();
    void reHighlightMonitor();

    QDBusInterface* m_AudioInterface;
    VolumeDbus *m_VolumeInterface;
    DisplayDbus *m_DisplayInterface;
    LayoutDbus *m_LayoutInterface;
    QLabel* m_BackImageLabel;
    QRect m_MouseInScreen;
    QLabel *m_ImageLabel;
    QTimer *m_Timer;
    QWidget *m_MonitersWrapper;
    QHBoxLayout *m_HLayout;
    QSvgWidget *m_DuplicateScreenImageSvg;
    QLabel* m_DuplicateScreenImageLabel;
    QLabel *m_DuplicateScreenText;
    QSvgWidget *m_ExpandedScreenImageSvg;
    QLabel* m_ExpandedScreenImageLabel;
    QLabel *m_ExpandedScreenText;
    QListWidget *m_ListWidget;
    QVariantAnimation *m_animation;
    QLabel* m_SwitchWMLabel;
    QFont m_f;
    QSvgWidget* m_Svg;

    int m_CurrentIndexOfMonitorItem;
    int m_CurrentIndexOfKeyBoard;
    int m_RecordInitialIndexOfKeyBoard;
    int m_MaxTextWidth;
    int m_KeyboradLayoutHeight;
    int contentY = 0;
    bool m_CanAudioMuteRun;
    bool m_AudioMuteNotRunFromAudioMute;

    QStringList m_ScreenList;
    QStringList m_KeyboardList;
    QList <QSvgWidget *> m_ImageSvgList;
    QList <QLabel *> m_ImageLabelList;
    QList <QLabel *> m_TextLabelList;
    QList <QLabel *> m_KeyboradTextList;

    enum DisPlayMode {
        Custom,
        Duplicate,
        Expanded,
        OneScreen
    } displaymode;

    enum ActionMode {
        SwitchLayout,
        SwitchMonitor,
        Normal,
        NormalAudio,
        NormalBrightness
    } actionMode;

    const int BASE_SIZE = 140;
    const int IMAGE_SIZE = 56;
    const int KEYBOARD_ITEM_HEIGHT = 36;
    const int LAYOUT_MARGIN = 10;
    const int DEADLINE_TIME = 2000;
    const QRect SWITCHWM_TEXT_GEOMETRY = QRect(16,96,108,34);
    const QPoint SWITCHWM_IMAGE_POINT = QPoint(42,30);
    const QString SWITCHWM_TEXT_STYLE = "QLabel{font-size:14px;color:#ffffff}";
    const QString KEYBOARD_ITEM_HIGHLIGHT_STYLE = "QLabel{color:#01bdff;font-size:14px;background:rgba(0,0,0,0.7);border:none;border-radius:5px}";
    const QString KEYBOARD_ITEM_NORMAL_STYLE = "QLabel{color:#ffffff;font-size:14px;}";
    const QString MONITOR_TEXT_HIGHLIGHT_STYLE = "QLabel{color:#01bdff;font-size:14px}";
    const QString MONITOR_TEXT_NORMAL_STYLE = "QLabel{color:#ffffff;font-size:14px}";
    const QString BACK_IMAGE_STYLE = "QLabel{border-width: 5px 5px 5px 5px;border-image:url(:/OSD_bg.png)}";
};

#endif // OSD_H
