#include "welcome.h"

#include <QPainter>
#include <QKeyEvent>
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QTimer>
#include <QVBoxLayout>

#include <X11/Xlib.h>
#include <X11/extensions/Xfixes.h>

DWIDGET_USE_NAMESPACE

Welcome::Welcome(QWidget *parent)
    : QWidget(parent),

      m_sizeAdjustTimer(new QTimer(this)),

      m_loadingSpinner(new DPictureSequenceView(this))
{
    m_sizeAdjustTimer->setInterval(100);
    m_sizeAdjustTimer->setSingleShot(true);

    QStringList spinner;
    for (int i(0); i != 90; ++i)
        spinner << QString(":/loading_spinner/resources/loading_spinner/loading_spinner_%1.png").arg(QString::number(i), 3, '0');
    m_loadingSpinner->setPictureSequence(spinner);
    m_loadingSpinner->play();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_loadingSpinner);
    centralLayout->setAlignment(m_loadingSpinner, Qt::AlignCenter);
    centralLayout->setSpacing(0);
    centralLayout->setMargin(0);

    setLayout(centralLayout);
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    connect(m_sizeAdjustTimer, &QTimer::timeout, this, &Welcome::onScreenRectChanged, Qt::QueuedConnection);

    m_sizeAdjustTimer->start();
    QTimer::singleShot(1, this, &Welcome::clearCursor);
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

    QTimer::singleShot(1, this, [this] () {
        raise();
        activateWindow();
        grabMouse();
        grabKeyboard();
        clearCursor();

        qDebug() << Q_FUNC_INFO;
    });
}

void Welcome::onScreenRectChanged()
{
    setFixedSize(qApp->desktop()->size());
}
