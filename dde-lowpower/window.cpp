#include <QLabel>
#include <QPixmap>
#include <QApplication>
#include <QDesktopWidget>
#include <QCursor>
#include <QDebug>

#include "window.h"

Window::Window(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::BypassWindowManagerHint | Qt::WindowStaysOnTopHint);

    setupSize();
    setStyleSheet("Window { background: black }");

    m_image = new QLabel(this);
    m_image->setPixmap(QPixmap(":/images/lowpower.png"));

    setupImagePosition();
}

Window::~Window()
{

}

void Window::setupSize()
{
    QDesktopWidget * desktop = QApplication::desktop();
    int screenCount = desktop->screenCount();

    int totalWidth = 0;
    int totalHeight = 0;
    for (int i = 0; i < screenCount; i++) {
        QWidget * screen = desktop->screen(i);
        totalWidth = qMax(totalWidth, screen->x() + screen->width());
        totalHeight = qMax(totalHeight, screen->y() + screen->height());
    }

    setFixedSize(totalWidth, totalHeight);
}

void Window::setupImagePosition()
{
    QPoint cursorPos = QCursor::pos();

    QDesktopWidget * desktop = QApplication::desktop();
    int screenNum = desktop->screenNumber(cursorPos);
    QWidget * screen = desktop->screen(screenNum);

    m_image->move(screen->geometry().center() - m_image->rect().center());
}
