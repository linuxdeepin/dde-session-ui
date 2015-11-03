#include "mainwindow.h"
#include <hotzone.h>

MainWindow::MainWindow(int width, int height, QWidget *parent) : QMainWindow(parent)
{
    // let the app start without system animation
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    //let background be transparent
    setAttribute(Qt::WA_TranslucentBackground, true);
    // set the size of this app
    this->resize(width, height);

    // set the background
    QWidget *back = new QWidget(this);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0, 127));
    back->setPalette(palette);
    back->setAutoFillBackground(true);
    back->resize(width, height);

    // init
    m_ButtonNames << tr("Control Center") << tr("All Windows") << tr("Launcher") << tr("Show Desktop") << tr("None");
    m_ActionStrs << CONTROL_CENTER_FROM_LEFT_STR << ALL_WINDOWS_STR << LAUNCHER_STR << SHOW_DESKTOP_STR << NONE_STR;
    m_ActionStrs2 << CONTROL_CENTER_FROM_RIGHT_STR << ALL_WINDOWS_STR << LAUNCHER_STR << SHOW_DESKTOP_STR << NONE_STR;

    // load 4 corners
    HotZone* hotzone1 = new HotZone(this, false, false);
    hotzone1->addButtons(m_ButtonNames,m_ActionStrs);

    HotZone* hotzone2 = new HotZone(this, true, false);
    hotzone2->addButtons(m_ButtonNames,m_ActionStrs2);

    HotZone* hotzone3 = new HotZone(this, false, true);
    hotzone3->addButtons(m_ButtonNames,m_ActionStrs);

    HotZone* hotzone4 = new HotZone(this, true, true);
    hotzone4->addButtons(m_ButtonNames,m_ActionStrs2);

}

MainWindow::~MainWindow()
{
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton || e->button() == Qt::MiddleButton) {
        MainWindow::close();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        MainWindow::close();
    }
}
