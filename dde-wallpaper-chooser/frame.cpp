#include "frame.h"
#include "wallpaperlistmodel.h"
#include "wallpaperlistview.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QDebug>

static const int ListHeight = 64;

Frame::Frame(QFrame *parent)
    : QFrame(parent),
      m_wallpaperList(new WallpaperListView(this))
{
    setFocusPolicy(Qt::StrongFocus);
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
//    setAttribute(Qt::WA_TranslucentBackground);

    initSize();
    initListView();
}

Frame::~Frame()
{

}

void Frame::showEvent(QShowEvent * event)
{
    activateWindow();

    QFrame::showEvent(event);
}

void Frame::keyPressEvent(QKeyEvent * event)
{
    if (event->key() == Qt::Key_Escape) {
        qApp->quit();
    }

    QFrame::keyPressEvent(event);
}

void Frame::initSize()
{
    QDesktopWidget * desktop = QApplication::desktop();
    QSize size = desktop->geometry().size();
    QRect primaryRect = desktop->screen(desktop->primaryScreen())->rect();

    setFixedSize(size);
    m_wallpaperList->setGeometry(QRect(primaryRect.x(),
                                       primaryRect.y() + primaryRect.height() - ListHeight,
                                       primaryRect.width(),
                                       ListHeight));
}

void Frame::initListView()
{
    WallpaperListModel * model = new WallpaperListModel(this);
    QStringList list; list << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg"
                           << "/usr/share/backgrounds/desktop.jpg";
    model->setStringList(list);

//    m_wallpaperList->setStyleSheet("QListView { outline: none; }");

    m_wallpaperList->setModel(model);
}
