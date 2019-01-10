#ifndef SESSIONBASEWINDOW_H
#define SESSIONBASEWINDOW_H

#include <QFrame>
#include <QVBoxLayout>

class QScrollArea;
class SessionBaseWindow : public QFrame
{
    Q_OBJECT
public:
    explicit SessionBaseWindow(QWidget *parent = nullptr);

    virtual void setLeftBottomWidget(QWidget *const widget) final;
    virtual void setRightBottomWidget(QWidget *const widget) final;
    virtual void setCenterContent(QWidget *const widget) final;

private:
    void initUI();

private:
    QVBoxLayout *m_mainLayou;
    QHBoxLayout *m_leftBottomLayout;
    QHBoxLayout *m_centerLayout;
    QHBoxLayout *m_rightBottomLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_centerWidget;
    QWidget *m_leftBottomWidget;
    QWidget *m_rightBottomWidget;
};

#endif // SESSIONBASEWINDOW_H
