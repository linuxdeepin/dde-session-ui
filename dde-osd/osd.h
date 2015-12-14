#ifndef OSD_H
#define OSD_H

#include "switchmonitor.h"
#include "switchlayout.h"
#include "switchnormal.h"
#include <QWidget>
#include <QPaintEvent>
#include <QTimer>
#include <QList>
#include <QCommandLineParser>
#include <QCommandLineOption>

class Osd : public QWidget
{
    Q_OBJECT

public:
    Osd(QWidget *parent = 0);
    ~Osd();

    Q_SLOT void loadBasicNormal(QString whichImage);
    Q_SLOT void loadAdditionalNormal(QString imageName);
    Q_SLOT void loadSwitchLayout();
    Q_SLOT void highlightNextLayout();
    Q_SLOT void highlightCurrentLayout();
    Q_SLOT void loadSwitchMonitors();
    Q_SLOT void highlightNextMonitor();
    Q_SLOT void highlightCurrentMonitor();
    Q_SLOT void tailInWork();
    Q_SLOT int lastAppMode();
    Q_SLOT bool getLayoutStatus();
    Q_SLOT bool getMonitorStatus();
    Q_SLOT void cancelNormalText();
    Q_SLOT void showNormalText(QString text);

    void addCmdImage(QString cmdString, QString imageName);
    void addCmdImageWithText(QString cmdString, QString imageName, QString text);
    bool handleBasicCmd();
    bool handleAdditionalCmd();
    bool handleAdditionalCmdWithText();
    void processParser();

private:
    void initGlobalVars();
    void initBasicOperation();
    void initBasicCommandLine();
    void initConnect();

    void setTimer();
    void moveToCenter();

    void paintEvent(QPaintEvent *e);

    SwitchNormal* m_SwitchNormal;
    SwitchLayout* m_SwitchLayout;
    SwitchMonitor* m_SwitchMonitor;
    QCommandLineParser m_Parser;
    QLabel* m_BackImageLabel;
    QRect m_MouseInScreen;
    QTimer *m_Timer;

    QList<QCommandLineOption> cmdOptionList;
    QList<QString> imageNameList;

    QList<QCommandLineOption> cmdOptionWithTextList;
    QList<QString> imageNameWithTextList;
    QList<QString> textList;

    enum ActionMode {
        SwitchLayoutEnum,
        SwitchMonitorEnum,
        NoProgressBar,
        AudioProgressBar,
        BrightnessProgressBar
    } actionMode;

    bool layoutHasBeenInited = false;
    bool monitorHasBeenInited = false;

    const int BASE_SIZE = 140;
    const int DEADLINE_TIME = 2000;
    const QString BACK_IMAGE_STYLE = "QLabel{border-width: 5px 5px 5px 5px;border-image:url(:/OSD_bg.png)}";

    // basic commandlineoption
    const QCommandLineOption BrightnessUp = QCommandLineOption("BrightnessUp", "\tOSD BrightnessUp");
    const QCommandLineOption BrightnessDown = QCommandLineOption("BrightnessDown", "\tOSD BrightnessDown");
    const QCommandLineOption AudioMute = QCommandLineOption("AudioMute", "\tOSD AudioMute");
    const QCommandLineOption AudioDown = QCommandLineOption("AudioDown", "\tOSD AudioDown");
    const QCommandLineOption AudioUp = QCommandLineOption("AudioUp", "\tOSD AudioUp");
    const QCommandLineOption SwitchMonitors = QCommandLineOption("SwitchMonitors", "\tOSD SwitchMonitors");
    const QCommandLineOption SwitchLayouts = QCommandLineOption("SwitchLayout", "\tOSD SwitchLayout");

};

#endif // OSD_H
