#ifndef ACTIONBUTTON_H
#define ACTIONBUTTON_H

#include <QFrame>
#include <QPainter>

class ActionButton : public QFrame
{
    Q_OBJECT
    struct Button {
        QString id;
        QString text;
    };

public:
    ActionButton(QWidget * parent = 0);

    bool addButton(QString id, QString text);
    bool isEmpty();
    void clear();

signals:
    void buttonClicked(QString id);

protected:
    void paintEvent(QPaintEvent *) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *) Q_DECL_OVERRIDE;

private:
    int m_radius = 4;
    QList<Button> m_buttons;
    bool m_mouseInButtonOne = false;
    bool m_mouseInButtonTwo = false;
    bool m_mousePressed = false;
    bool m_mouseHover = false;
};

#endif // ACTIONBUTTON_H
