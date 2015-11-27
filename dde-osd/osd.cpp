#include <gtk/gtk.h>
#include "osd.h"
#include <X11/extensions/shape.h>
#include <QtX11Extras/QX11Info>
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
#include <QPen>
#include <QPainterPath>
#include <QBrush>
#include <QRectF>
#include <QRect>
#include <QFile>

Osd::Osd(QWidget *parent)
    : QWidget(parent)
{

    gtk_init(NULL, NULL);
    gdk_error_trap_push();

    initInterfaces();

    initGlobalVars();

    initBasicOperation();

    initConnects();
}

Osd::~Osd()
{

    // set the monitor mode when the app quits
    if (actionMode == SwitchMonitor) {
        if (m_CurrentIndexOfMonitorItem == 0) {
            // switch to duplicate mode
            m_DisplayInterface->SwitchMode(1, "");
        } else if (m_CurrentIndexOfMonitorItem == 1) {
            // switch to expanded mode
            m_DisplayInterface->SwitchMode(2, "");
        } else {
            // switch to one certain screen mode
            m_DisplayInterface->SwitchMode(3, m_ScreenList[m_CurrentIndexOfMonitorItem - 2]);
        }
    }

    // set the keyboard layout when the app quits
    if (actionMode == SwitchLayout && m_KeyboardList.length() > 0) {
        m_LayoutInterface->setCurrentLayout(m_KeyboardList[m_CurrentIndexOfKeyBoard]);
    }
}

void Osd::initInterfaces()
{
    m_VolumeInterface = new VolumeDbus("com.deepin.daemon.Audio",
                                       "/com/deepin/daemon/Audio/Sink0",
                                       QDBusConnection::sessionBus(), this);

    m_DisplayInterface = new DisplayDbus("com.deepin.daemon.Display",
                                         "/com/deepin/daemon/Display",
                                         QDBusConnection::sessionBus(), this);

    m_LayoutInterface = new LayoutDbus("com.deepin.daemon.InputDevices",
                                       "/com/deepin/daemon/InputDevice/Keyboard",
                                       QDBusConnection::sessionBus(), this);
}

void Osd::initGlobalVars()
{
    // m_BackImageLabel is used for the white border
    m_BackImageLabel = new QLabel(this);

    // image label
    m_ImageLabel = new QLabel(this);
    // m_Timer is used to record time , to quit the app properly
    m_Timer = new QTimer(this);

    // initial m_ListWidget
    m_ListWidget = new QListWidget(this);

    // m_Svg is used to display .svg files
    m_Svg = new QSvgWidget(this);

    // initial m_MonitersWrapper
    m_MonitersWrapper = new QWidget(this);

    // m_CanAudioMuteRun is used to record the mute state of sound
    m_CanAudioMuteRun = m_VolumeInterface->mute();
    // to record whether AudioMute has run before this time
    m_AudioMuteNotRunFromAudioMute = true;

    // init SwitchMonitors's displaymode
    if (m_DisplayInterface->displayMode() == 0) {
        displaymode = Custom;
    } else if (m_DisplayInterface->displayMode() == 1) {
        displaymode = Duplicate;
    } else if (m_DisplayInterface->displayMode() == 2) {
        displaymode = Expanded;
    } else if (m_DisplayInterface->displayMode() == 3) {
        displaymode = OneScreen;
    }
}

void Osd::initBasicOperation()
{
    this->resize(BASE_SIZE, BASE_SIZE);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    // set mouse penetration
    XShapeCombineRectangles(QX11Info::display(), winId(), ShapeInput, 0, 0, NULL, 0, ShapeSet, YXBanded);

    // set fixed size for image icon, and move it to app's center
    m_ImageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    m_ImageLabel->move((this->width() - IMAGE_SIZE) / 2, (this->height() - IMAGE_SIZE) / 2);

    // set fixed size for image icon, and move it to app's center
    m_Svg->setFixedSize(IMAGE_SIZE,IMAGE_SIZE);
    m_Svg->move((this->width() - IMAGE_SIZE) / 2, (this->height() - IMAGE_SIZE) / 2);
}

void Osd::initConnects()
{
    // when reaches deadline, we need to quit the app immediately
    connect(m_Timer, &QTimer::timeout, qApp, &QCoreApplication::quit);
}

QString Osd::getThemeIconPath(QString iconName)
{
    QByteArray bytes = iconName.toUtf8();
    const char *name = bytes.constData();

    GtkIconTheme* theme = gtk_icon_theme_get_default();

    GtkIconInfo* info = gtk_icon_theme_lookup_icon(theme, name, 240, GTK_ICON_LOOKUP_GENERIC_FALLBACK);

    if (info) {
        char* path = g_strdup(gtk_icon_info_get_filename(info));
#if GTK_MAJOR_VERSION >= 3
        g_object_unref(info);
#elif GTK_MAJOR_VERSION == 2
        gtk_icon_info_free(info);
#endif
        return QString(path);
    } else {
        return "";
    }
}

void Osd::showThemeImage(QString iconName, QSvgWidget* svgLoader, QLabel* notSvgLoader){
    if(iconName.endsWith(".svg")){
        svgLoader->load(iconName);
    }else if(iconName.isEmpty()){
        svgLoader->load(getThemeIconPath("application-default-icon"));
    }else{
        notSvgLoader->setPixmap(QPixmap(iconName).scaled(IMAGE_SIZE,IMAGE_SIZE,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    }
}

int Osd::latterAction()
{
    return actionMode;
}

void Osd::moveToCenter()
{
    // find out the screen that contains mouse.
    QDesktopWidget *desktop = QApplication::desktop();
    int primaryScreenKey = desktop->primaryScreen();
    for (int i = 0; i < desktop->screenCount(); i++) {
        QRect screen = desktop->screenGeometry(primaryScreenKey + i);
        if (screen.contains(QCursor::pos())) {
            m_MouseInScreen = screen;
            break;
        }
    }
    // move to corresponding screen
    this->move(m_MouseInScreen.x() + (m_MouseInScreen.width() - this->width()) / 2, m_MouseInScreen.y() + (m_MouseInScreen.height() - this->height()) / 2);
}

void Osd::setTimer()
{
    m_Timer->start(DEADLINE_TIME);
}

void Osd::tail_in_Work()
{
    moveToCenter();
    m_Timer->start(DEADLINE_TIME);
    this->repaint();
}

// this function is used to display normal osd'show, which is different from SwitchLayout and SwitchMonitors
void Osd::loadCorrespondingImage(QString whichImage)
{
    actionMode = Normal;
    m_ListWidget->setVisible(false);
    m_MonitersWrapper->setVisible(false);
    m_Svg->setVisible(true);
    this->resize(BASE_SIZE, BASE_SIZE);
    m_BackImageLabel->resize(this->size());
    m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
    if (whichImage == "NumLockOn") {
        showThemeImage(getThemeIconPath("numlock-enabled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "NumLockOff") {
        showThemeImage(getThemeIconPath("numlock-disabled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "CapsLockOn") {
        showThemeImage(getThemeIconPath("capslock-enabled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "CapsLockOff") {
        showThemeImage(getThemeIconPath("capslock-disabled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "TouchpadOn") {
        showThemeImage(getThemeIconPath("touchpad-enabled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "TouchpadOff") {
        showThemeImage(getThemeIconPath("touchpad-disabled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "TouchpadToggle") {
        showThemeImage(getThemeIconPath("touchpad-toggled-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "SwitchWM3D") {
        showThemeImage(getThemeIconPath("wm-effect-enabled"), m_Svg, m_ImageLabel);
    } else if (whichImage == "SwitchWM2D") {
        showThemeImage(getThemeIconPath("wm-effect-disabled"), m_Svg, m_ImageLabel);
    } else if (whichImage == "SwitchWMError") {
        showThemeImage(getThemeIconPath("wm-effect-error"), m_Svg, m_ImageLabel);
    } else if (whichImage == "Brightness") {
        actionMode = NormalBrightness;
        showThemeImage(getThemeIconPath("display-brightness-symbolic"), m_Svg, m_ImageLabel);
    } else if (whichImage == "AudioMute") {
        if (m_CanAudioMuteRun && m_AudioMuteNotRunFromAudioMute) {
            showThemeImage(getThemeIconPath("audio-volume-muted-symbolic-osd"), m_Svg, m_ImageLabel);
            m_AudioMuteNotRunFromAudioMute = false;
        } else {
            loadCorrespondingImage("Audio");
        }
    } else if (whichImage == "Audio") {
        actionMode = NormalAudio;
        m_CanAudioMuteRun = true;
        m_AudioMuteNotRunFromAudioMute = true;
        double volume = m_VolumeInterface->volume();
        // 0.7~1.0 means high volume range, 0.3~0.7 means medium volume range, and 0.0~0.3 means low volume range.
        if (volume > 0.7 && volume <= 1.0) {
            showThemeImage(getThemeIconPath("audio-volume-high-symbolic-osd"), m_Svg, m_ImageLabel);
        } else if (volume > 0.3 && volume <= 0.7) {
            showThemeImage(getThemeIconPath("audio-volume-medium-symbolic-osd"), m_Svg, m_ImageLabel);
        } else if (volume > 0.0) {
            showThemeImage(getThemeIconPath("audio-volume-low-symbolic-osd"), m_Svg, m_ImageLabel);
        } else if (volume == 0.0) {
            showThemeImage(getThemeIconPath("audio-volume-muted-symbolic-osd"), m_Svg, m_ImageLabel);
        }
    }
}

// --SwitchLayout
void Osd::loadSwitchLayout()
{
    actionMode = SwitchLayout;
    m_Svg->setVisible(false);
    m_MonitersWrapper->setVisible(false);
    m_ListWidget->setVisible(true);

    if (m_LayoutInterface->userLayoutList().length() > 1) {

        // give out the value of m_MaxTextWidth and m_KeyboradLayoutHeight, to help resize this app
        calculateKeyboardSize();
        this->resize(m_MaxTextWidth + LAYOUT_MARGIN * 4, m_KeyboradLayoutHeight + LAYOUT_MARGIN * 2);
        m_BackImageLabel->resize(this->size());
        m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);
        initKeyboard();
    } else {
        // if user's keyboard layout(s) just contain(s) 1 kind, quit the app immediately
        this->deleteLater();
    }
}

// the following 3 functions belong to SwitchLayout,which are calculateKeyboardSize(), initKeyboard() and reHiglightKeyboard()
void Osd::calculateKeyboardSize()
{
    m_MaxTextWidth = 0;

    // get the list of keyboard layout
    m_KeyboardList = m_LayoutInterface->userLayoutList();

    // set font-size to 14px, so that we can calculate the max width of all keyboardlayout texts
    QLabel *text = new QLabel;
    m_f = text->font();
    m_f.setPixelSize(14);
    text->setFont(m_f);

    int length = m_KeyboardList.length();
    // give the value of m_KeyboradLayoutHeight
    m_KeyboradLayoutHeight = (length > 5 ? KEYBOARD_ITEM_HEIGHT * 5 : KEYBOARD_ITEM_HEIGHT * length);

    // give the value of m_MaxTextWidth
    QFontMetrics metrics(text->font());
    for (int i = 0; i < length; i++) {
        text->setText(m_LayoutInterface->GetLayoutDesc(m_KeyboardList[i]));
        int textWidth = metrics.boundingRect(text->text()).width();
        m_MaxTextWidth = (textWidth > m_MaxTextWidth ? textWidth : m_MaxTextWidth);
    }
}

void Osd::initKeyboard()
{
    // variable "size" is the size of QListWidgetItem* item and QWidget* customItem
    QSize size(m_MaxTextWidth + LAYOUT_MARGIN * 2, KEYBOARD_ITEM_HEIGHT);

    // hLayout is used to wrap m_ListWidget
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    // make sure m_ListWidget's margins be 10px
    hLayout->setContentsMargins(10, 10, 10, 10);

    // set m_ListWidget's parameters
    m_ListWidget->setStyleSheet("background:transparent");
    m_ListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ListWidget->setFrameStyle(QFrame::NoFrame);
    m_ListWidget->resize(m_MaxTextWidth + LAYOUT_MARGIN * 2, m_KeyboradLayoutHeight);

    for (int i = 0, length = m_KeyboardList.length(); i < length; i++) {
        QListWidgetItem *item = new QListWidgetItem;
        // setFlags(Qt::NoItemFlags) can remove the default highlight
        item->setFlags(Qt::NoItemFlags);
        item->setSizeHint(size);

        // customItem and text within it
        QWidget *customItem = new QWidget;
        customItem->setFixedSize(size);
        QLabel *text = new QLabel(customItem);
        text->setFont(m_f);
        m_KeyboradTextList << text;
        // make sure left-margin and right-margin be 10px
        text->setContentsMargins(10, 0, 10, 0);
        text->setFixedSize(size);
        text->setText(m_LayoutInterface->GetLayoutDesc(m_KeyboardList[i]));

        // highlight the chosen customItem and text withint it, when app starts
        if (m_LayoutInterface->currentLayout() == m_KeyboardList[i]) {
            m_CurrentIndexOfKeyBoard = i;
            m_RecordInitialIndexOfKeyBoard = i;
            text->setStyleSheet(KEYBOARD_ITEM_HIGHLIGHT_STYLE);
        } else {
            text->setStyleSheet(KEYBOARD_ITEM_NORMAL_STYLE);
        }

        m_ListWidget->addItem(item);
        m_ListWidget->setItemWidget(item, customItem);
    }

    hLayout->addWidget(m_ListWidget);

    // make sure that the highlighted item can be displayed in view after app startss
    m_ListWidget->scrollToItem(m_ListWidget->item(m_CurrentIndexOfKeyBoard));

    // the following codes are about animation
    contentY = m_ListWidget->itemWidget(m_ListWidget->item(0))->y();
    m_animation = new QVariantAnimation(m_ListWidget);
    m_animation->setDuration(70);

    // when currentrow changes, check if new_contentY is different from contentY. If different,m_animation should start
    connect(m_ListWidget, &QListWidget::currentRowChanged,
            this, [this] {
        int new_contentY = m_ListWidget->itemWidget(m_ListWidget->item(0))->y();
        if (new_contentY != contentY  && m_KeyboardList.length() > 5)
        {
            m_animation->setStartValue(QVariant::fromValue(contentY));
            m_animation->setEndValue(QVariant::fromValue(new_contentY));

            contentY = new_contentY;

            m_animation->start();
        }
    });

    connect(m_animation, &QVariantAnimation::valueChanged, this, [this](const QVariant & value) {
        int contentY = m_ListWidget->itemWidget(m_ListWidget->item(0))->y();
        for (int i = 0; i < m_KeyboardList.length(); i++) {
            QWidget *w = m_ListWidget->itemWidget(m_ListWidget->item(i));
            w->move(0, w->y() - contentY + value.toInt());
        }
    });
}

void Osd::reHiglightKeyboard()
{
    if (m_CurrentIndexOfKeyBoard == 0) {
        // if m_CurrentIndexOfKeyBoard == 0,highlight the first one, and normalize the last one
        m_KeyboradTextList[0]->setStyleSheet(KEYBOARD_ITEM_HIGHLIGHT_STYLE);
        m_KeyboradTextList[m_KeyboradTextList.length() - 1]->setStyleSheet(KEYBOARD_ITEM_NORMAL_STYLE);
    } else {
        // highlight the current one, normalize the old one
        m_KeyboradTextList[m_CurrentIndexOfKeyBoard]->setStyleSheet(KEYBOARD_ITEM_HIGHLIGHT_STYLE);
        m_KeyboradTextList[m_CurrentIndexOfKeyBoard - 1]->setStyleSheet(KEYBOARD_ITEM_NORMAL_STYLE);
    }
    m_ListWidget->setCurrentRow(m_CurrentIndexOfKeyBoard);
}

void Osd::highlightNextLayout()
{
    if (m_CurrentIndexOfKeyBoard == m_KeyboardList.length() - 1) {
        m_CurrentIndexOfKeyBoard = 0;

        // make sure the listwidgetitem is positioned as initial after one-cycle selection
        if (m_KeyboardList.length() > 5) {
            for (int i = 0, length = m_KeyboardList.length(); i < length; i++) {
                QWidget *w = m_ListWidget->itemWidget(m_ListWidget->item(i));
                w->move(0, KEYBOARD_ITEM_HEIGHT * (i + 5 - length));
            }
        }
    } else {
        m_CurrentIndexOfKeyBoard++;
    }
    reHiglightKeyboard();
}

void Osd::loadSwitchMonitors()
{
    actionMode = SwitchMonitor;
    m_Svg->setVisible(false);
    m_ListWidget->setVisible(false);
    m_MonitersWrapper->setVisible(true);
    // get the list of all screens by using QString's method "split"
    QString screenNamesStr = (QString)m_DisplayInterface->QueryCurrentPlanName();
    m_ScreenList = screenNamesStr.split(",");

    if (m_ScreenList.length() > 1) {
        this->resize(BASE_SIZE * (m_ScreenList.length() + 2), BASE_SIZE);
        m_BackImageLabel->resize(this->size());
        m_BackImageLabel->setStyleSheet(BACK_IMAGE_STYLE);

        initMonitorItems();

        initCurrentScreenMode();
    } else {
        // if just 1 screen , quit the app immediately
        this->deleteLater();
    }
}

void Osd::initMonitorItems()
{
    m_MonitersWrapper->resize(this->size());
    m_HLayout = new QHBoxLayout(m_MonitersWrapper);

    // for duplicate mode
    QWidget *duplicateScreenItem = new QWidget(m_MonitersWrapper);
    QVBoxLayout *vLayoutOfDuplicateScreen = new QVBoxLayout(duplicateScreenItem);
    // image label for duplicate mode
    m_DuplicateScreenImageSvg = new QSvgWidget(duplicateScreenItem);
    m_DuplicateScreenImageSvg->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    m_DuplicateScreenImageLabel = new QLabel(duplicateScreenItem);
    m_DuplicateScreenImageLabel->setFixedSize(IMAGE_SIZE,IMAGE_SIZE);
    showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
    // text label for duplicate mode
    m_DuplicateScreenText = new QLabel(duplicateScreenItem);
    m_DuplicateScreenText->setText(tr("Duplicate"));
    m_DuplicateScreenText->setAlignment(Qt::AlignCenter);
    m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    // add above 2 widgets
    if(getThemeIconPath("project_screen-duplicate-symbolic").endsWith(".svg")){
        vLayoutOfDuplicateScreen->addWidget(m_DuplicateScreenImageSvg, 0, Qt::AlignHCenter);
    }else{
        vLayoutOfDuplicateScreen->addWidget(m_DuplicateScreenImageLabel, 0, Qt::AlignHCenter);
    }
    vLayoutOfDuplicateScreen->addWidget(m_DuplicateScreenText, 0, Qt::AlignHCenter);

    // for expanded mode
    QWidget *expandedScreenItem = new QWidget(m_MonitersWrapper);
    QVBoxLayout *vLayoutOfExpandedScreen = new QVBoxLayout(expandedScreenItem);
    // image label for expanded mode
    m_ExpandedScreenImageSvg = new QSvgWidget(expandedScreenItem);
    m_ExpandedScreenImageSvg->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    m_ExpandedScreenImageLabel = new QLabel(duplicateScreenItem);
    m_ExpandedScreenImageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
    showThemeImage(getThemeIconPath("project_screen-extend-symbolic"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
    // text label for expanded mode
    m_ExpandedScreenText = new QLabel(expandedScreenItem);
    m_ExpandedScreenText->setText(tr("Extend"));
    m_ExpandedScreenText->setAlignment(Qt::AlignCenter);    m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    // add above 2 widgets
    if(getThemeIconPath("project_screen-extend-symbolic").endsWith(".svg")){
        vLayoutOfExpandedScreen->addWidget(m_ExpandedScreenImageSvg, 0, Qt::AlignHCenter);
    }else{
        vLayoutOfExpandedScreen->addWidget(m_ExpandedScreenImageLabel, 0, Qt::AlignHCenter);
    }
    vLayoutOfExpandedScreen->addWidget(m_ExpandedScreenText, 0, Qt::AlignHCenter);

    // add duplicate and expanded items
    m_HLayout->addWidget(duplicateScreenItem);
    m_HLayout->addWidget(expandedScreenItem);

    // for one-screen mode
    for (int i = 0, length = m_ScreenList.length(); i < length; i++) {
        // one-screen mode item
        QWidget *item = new QWidget(m_MonitersWrapper);
        QVBoxLayout *vLayout = new QVBoxLayout(item);
        // image label for one-screen mode
        QSvgWidget *imageSvg = new QSvgWidget(item);
        imageSvg->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
        QLabel* imageLabel = new QLabel(item);
        imageLabel->setFixedSize(IMAGE_SIZE, IMAGE_SIZE);
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic"), imageSvg, imageLabel);
        // text label for one-screen mode
        QLabel *textLabel = new QLabel(item);
        textLabel->setText(m_ScreenList[i]);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
        // store imagelabel and textlabel into lists, so that we can change their style later
        m_ImageSvgList << imageSvg;
        m_ImageLabelList << imageLabel;
        m_TextLabelList << textLabel;
        // add above 2 widgets
        if(getThemeIconPath("project_screen-onlyone-symbolic").endsWith(".svg")){
            vLayout->addWidget(imageSvg, 0, Qt::AlignHCenter);
        }else{
            vLayout->addWidget(imageLabel, 0, Qt::AlignHCenter);
        }
        vLayout->addWidget(textLabel, 0, Qt::AlignHCenter);

        // add one-screen mode item
        m_HLayout->addWidget(item);
    }
}

void Osd::initCurrentScreenMode()
{
    // for each mode, we would do the same operations to change it's style when app starts
    switch (displaymode) {
    case Custom:
        //        m_CurrentIndexOfMonitorItem = -1;
        break;
    case Duplicate:
        showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic-focus"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
        m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        m_CurrentIndexOfMonitorItem = 0;
        break;
    case Expanded:
        showThemeImage(getThemeIconPath("project_screen-extend-symbolic-focus"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
        m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        m_CurrentIndexOfMonitorItem = 1;
        break;
    case OneScreen:
        QString primaryScreenName = m_DisplayInterface->primary();
        for (int i = 0, length = m_ScreenList.length(); i < length; i++) {
            if (m_ScreenList[i] == primaryScreenName) {
                showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic-focus"), m_ImageSvgList[i], m_ImageLabelList[i]);
                m_TextLabelList[i]->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
                // m_CurrentIndexOfMonitorItem is always 2 bigger than m_ScreenList's i
                m_CurrentIndexOfMonitorItem = i + 2;
            }
        }
        break;
    }
}

// when Meta-P is pressed , we need to refresh the highlight style
void Osd::reHighlightMonitor()
{
    if (m_CurrentIndexOfMonitorItem == 0) {
        showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic-focus"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
        m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic"), m_ImageSvgList[m_ScreenList.length() - 1], m_ImageLabelList[m_ScreenList.length() - 1]);
        m_TextLabelList[m_ScreenList.length() - 1]->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    } else if (m_CurrentIndexOfMonitorItem == 1) {
        showThemeImage(getThemeIconPath("project_screen-extend-symbolic-focus"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
        m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-duplicate-symbolic"), m_DuplicateScreenImageSvg, m_DuplicateScreenImageLabel);
        m_DuplicateScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    } else if (m_CurrentIndexOfMonitorItem == 2) {
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic-focus"), m_ImageSvgList[0], m_ImageLabelList[0]);
        m_TextLabelList[0]->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-extend-symbolic"), m_ExpandedScreenImageSvg, m_ExpandedScreenImageLabel);
        m_ExpandedScreenText->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    } else {
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic-focus"), m_ImageSvgList[m_CurrentIndexOfMonitorItem - 2], m_ImageLabelList[m_CurrentIndexOfMonitorItem - 2]);
        m_TextLabelList[m_CurrentIndexOfMonitorItem - 2]->setStyleSheet(MONITOR_TEXT_HIGHLIGHT_STYLE);
        showThemeImage(getThemeIconPath("project_screen-onlyone-symbolic"), m_ImageSvgList[m_CurrentIndexOfMonitorItem - 3], m_ImageLabelList[m_CurrentIndexOfMonitorItem - 3]);
        m_TextLabelList[m_CurrentIndexOfMonitorItem - 3]->setStyleSheet(MONITOR_TEXT_NORMAL_STYLE);
    }
}

void Osd::highlightNextMonitor()
{
    if (m_CurrentIndexOfMonitorItem < (m_ScreenList.length() + 1)) {
        ++m_CurrentIndexOfMonitorItem;
    } else {
        m_CurrentIndexOfMonitorItem = 0;
    }
    reHighlightMonitor();
}

void Osd::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    // paint app's background
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // for the backgound color
    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::transparent);
    pen.setWidth(1);
    QPainterPath path;
    path.addRoundedRect(QRectF(0.5, 0.5, this->width()-1, this->height()-1), 10, 10);
    painter.setPen(pen);
    painter.fillPath(path,QColor(0,0,0,127));

    if (actionMode == NormalAudio || actionMode == NormalBrightness) {
        // paint progressbar's background
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        QRect progressBarBackRect(30,106,80,4);
        brush.setColor(QColor(255,255,255,51));
        painter.setBrush(brush);
        painter.drawRoundedRect(progressBarBackRect,2,2);

        if (actionMode == NormalAudio) {
            // paint audio progressbar
            QRect progressBarRect(30,106,80*(m_VolumeInterface->volume()>=1.0 ? 1.0 : m_VolumeInterface->volume()),4);
            brush.setColor(Qt::white);
            painter.setBrush(brush);
            painter.drawRoundedRect(progressBarRect,2,2);
        } else if (actionMode == NormalBrightness) {
            // paint brightness progressbar
            QRect progressBarRect(30,106,80*m_DisplayInterface->brightness()[m_DisplayInterface->primary()],4);
            brush.setColor(Qt::white);
            painter.setBrush(brush);
            painter.drawRoundedRect(progressBarRect,2,2);
        }
    }
}

