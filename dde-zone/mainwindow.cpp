#include "mainwindow.h"
#include "pushbuttonlist.h"
#include <hotzone.h>
#include <QScreen>
#include <QGuiApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // let the app start without system animation
    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::WindowStaysOnTopHint);
    // let background be transparent
    setAttribute(Qt::WA_TranslucentBackground, true);

    // catch the screen that mouse is in
    QList<QScreen*> screenList = QGuiApplication::screens();
    for (int i=0;i<screenList.length();i++){
        QRect screen = screenList[i]->geometry();
        if(screen.contains(QCursor::pos())){
            // set the size and position of this app. Enlarge 30px to height to avoid fade-zone of mouseEvent.
            this->setGeometry(screen.x(), screen.y()-MAIN_ITEM_TOP_MARGIN, screen.width(), screen.height()+MAIN_ITEM_TOP_MARGIN);
            break;
        }
    }

    // set the background
    QWidget *back = new QWidget(this);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0, 127));
    back->setPalette(palette);
    back->setAutoFillBackground(true);
    back->setGeometry(0, MAIN_ITEM_TOP_MARGIN, this->width(), this->height()-MAIN_ITEM_TOP_MARGIN);

    // init corresponding QList for addButtons()
    m_ButtonNames << tr("Control Center") << tr("All Windows") << tr("Launcher") << tr("Desktop") << tr("None");
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
        this->deleteLater();
        qApp->quit();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape) {
        this->deleteLater();
        qApp->quit();
    }
}
