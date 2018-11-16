#ifndef SESSIONBASEWINDOW_H
#define SESSIONBASEWINDOW_H

#include <QFrame>
#include <QVBoxLayout>

class SessionBaseWindow : public QFrame
{
    Q_OBJECT
public:
    explicit SessionBaseWindow(QWidget *parent = nullptr);

    void setLeftBottomWidget(QWidget *const widget);
    void setRightBottomWidget(QWidget *const widget);
    void setCenterContent(QWidget *const widget);

private:
    void initUI();

private:
    QVBoxLayout *m_mainLayou;
    QHBoxLayout *m_centerLayout;
    QHBoxLayout *m_leftBottomLayout;
    QHBoxLayout *m_rightBottomLayout;

    QWidget *m_centerContent;

    QWidget *m_leftBottomWidget;
    QWidget *m_rightBottomWidget;
};

#endif // SESSIONBASEWINDOW_H
