#include "welcome.h"
#include <QPainter>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QScreen>
#include <QSettings>
#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>
#include "version.h"

DWIDGET_USE_NAMESPACE

Welcome::Welcome(QWidget *parent)
    : QWidget(parent),

      m_sizeAdjustTimer(new QTimer(this)),

      m_loadingSpinner(new DPictureSequenceView(this)),
      m_isUpgrade(false)
{
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    m_sizeAdjustTimer->setInterval(100);
    m_sizeAdjustTimer->setSingleShot(true);
    connect(m_sizeAdjustTimer, &QTimer::timeout, this, &Welcome::onScreenRectChanged, Qt::QueuedConnection);
    m_sizeAdjustTimer->start();

    m_isUpgrade = checkVersion();
    if (m_isUpgrade) {
        m_update = new Update(getSystemVersion(), this);
    } else {
        QStringList spinner;
        for (int i(0); i != 90; ++i)
            spinner << QString(":/loading_spinner/resources/loading_spinner/loading_spinner_%1.png").arg(QString::number(i), 3, '0');
        m_loadingSpinner->setPictureSequence(spinner);
        m_loadingSpinner->setFixedSize(32, 32);

        QTimer::singleShot(1, this, [this] () {
            raise();
            grabMouse();
            clearCursor();

            m_loadingSpinner->play();

            qDebug() << Q_FUNC_INFO;
        });
    }

#ifdef QT_DEBUG
    show();
#endif
}

Welcome::~Welcome()
{
    qApp->restoreOverrideCursor();
}

void Welcome::dbus_show()
{
    qDebug() << Q_FUNC_INFO;

    show();
}

void Welcome::dbus_exit()
{
    qDebug() << Q_FUNC_INFO;

    if (!m_isUpgrade)
        qApp->quit();
}

void Welcome::clearCursor()
{
    qApp->setOverrideCursor(Qt::BlankCursor);

    const auto disp = XOpenDisplay(nullptr);
    Q_ASSERT(disp);
    const auto window = DefaultRootWindow(disp);

    Cursor invisibleCursor;
    Pixmap bitmapNoData;
    XColor black;
    static char noData[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    black.red = black.green = black.blue = 0;

    bitmapNoData = XCreateBitmapFromData(disp, window, noData, 8, 8);
    invisibleCursor = XCreatePixmapCursor(disp, bitmapNoData, bitmapNoData,
                                          &black, &black, 0, 0);
    XDefineCursor(disp, window, invisibleCursor);
    XFixesChangeCursorByName(disp, invisibleCursor, "watch");
    XFreeCursor(disp, invisibleCursor);
    XFreePixmap(disp, bitmapNoData);
    XFlush(disp);
}

void Welcome::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
#ifdef  QT_DEBUG
    case Qt::Key_Escape:        qApp->quit();       break;
#endif
    default:;
    }

    QWidget::keyPressEvent(e);
}

void Welcome::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);

    QPainter painter(this);
    painter.fillRect(rect(), Qt::black);

    // debug
//    painter.fillRect(QRect(0, 0, 500, 500), Qt::red);
}

void Welcome::showEvent(QShowEvent *e)
{
    qDebug() << Q_FUNC_INFO;

    QWidget::showEvent(e);

    QTimer::singleShot(1, this, [=] {
        showFullScreen();
        activateWindow();
        grabKeyboard();

        qDebug() << Q_FUNC_INFO;
    });
}

void Welcome::onScreenRectChanged()
{
    setFixedSize(qApp->desktop()->size());

    const QPoint center = qApp->primaryScreen()->geometry().center();
    m_loadingSpinner->move(center.x() - m_loadingSpinner->width() / 2,
                           center.y() - m_loadingSpinner->height() / 2);
}

bool Welcome::checkVersion()
{
    QString currentVersion = getSystemVersion();

    if (currentVersion.isEmpty())
        return false;

    // check file exist
    QSettings welcomeSetting("deepin", "dde-welcome");
    QString version = welcomeSetting.value("Version").toString();
    if (version.isEmpty()) {
        welcomeSetting.setValue("Version", currentVersion);
        return false;
    }

    QRegExp re("(^\\d+\\.\\d+)");

    int currentVersionPos = currentVersion.indexOf(re);
    if (currentVersionPos >= 0)
        currentVersion = re.cap(0);
    else
        return false;

    int versionPos = version.indexOf(re);
    if (versionPos >= 0)
        version = re.cap(0);
    else
        return false;

    const int result = alpm_pkg_vercmp(currentVersion.toStdString().c_str(), version.toStdString().c_str());

    if (result > 0) {
        welcomeSetting.setValue("Version", currentVersion);
        return true;
    }

    welcomeSetting.setValue("Version", currentVersion);

    return false;
}

const QString Welcome::getSystemVersion()
{
    QSettings lsbSetting("/etc/lsb-release", QSettings::IniFormat);
    return lsbSetting.value("DISTRIB_RELEASE").toString();
}
