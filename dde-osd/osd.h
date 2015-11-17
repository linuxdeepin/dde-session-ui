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

    void paintEvent(QPaintEvent *);

    // for keyboardlayout
    void calculateKeyboardSize();
    void initKeyboard();
    void reHiglightKeyboard();

    // for monitors
    void initMonitorItems();
    void initCurrentScreenMode();
    void reHighlightMonitor();


    VolumeDbus *m_VolumeInterface;
    DisplayDbus *m_DisplayInterface;
    LayoutDbus *m_LayoutInterface;
    QRect m_MouseInScreen;
    QLabel *m_ImageLabel;
    QPixmap m_Pixmap;
    QTimer *m_Timer;
    QWidget *m_MonitersWrapper;
    QHBoxLayout *m_HLayout;
    QLabel *m_DuplicateScreenImage;
    QLabel *m_DuplaicateScreenText;
    QLabel *m_ExpandedScreenImage;
    QLabel *m_ExpandedScreenText;
    QListWidget *m_ListWidget;
    QVariantAnimation *m_animation;
    QFont m_f;

    int m_CurrentIndexOfMonitorItem;
    int m_CurrentIndexOfKeyBoard;
    int m_RecordInitialIndexOfKeyBoard;
    int m_MaxTextWidth;
    int m_KeyboradLayoutHeight;
    int contentY = 0;
    bool m_AudioMuteNotRun;

    QStringList m_ScreenList;
    QStringList m_KeyboardList;
    QList < QLabel * > m_ImageLabelList;
    QList < QLabel * > m_TextLabelList;
    QList < QLabel * > m_KeyboradTextList;

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
    const QString KEYBOARD_ITEM_HIGHLIGHT_STYLE = "QLabel{color:#01bdff;font-size:14px;padding-left:10px;background:rgba(0,0,0,0.7);border-radius:5px}";
    const QString KEYBOARD_ITEM_NORMAL_STYLE = "QLabel{color:#ffffff;font-size:14px;padding-left:10px}";
    const QString MONITOR_TEXT_HIGHLIGHT_STYLE = "QLabel{color:#01bdff;font-size:14px}";
    const QString MONITOR_TEXT_NORMAL_STYLE = "QLabel{color:#ffffff;font-size:14px}";

};

#endif // OSD_H
