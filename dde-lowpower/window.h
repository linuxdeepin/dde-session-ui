#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QLabel;
class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = 0);
    ~Window();

private:
    QLabel * m_image;

    void setupSize();
    void setupImagePosition();
};

#endif // WINDOW_H
