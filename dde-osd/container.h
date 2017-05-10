#ifndef CONTAINER_H
#define CONTAINER_H

#include <DBlurEffectWidget>
#include <DWindowManagerHelper>

DWIDGET_USE_NAMESPACE

class QHBoxLayout;
class Container : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit Container(QWidget *parent = 0);

    void setContent(QWidget *content);
    void moveToCenter();

private slots:
    void windowManagerChanged();

private:
    QHBoxLayout *m_layout;

    DWindowManagerHelper *m_wmHelper;
    bool m_supportComposite;

    void updateWindowRadius();
    int getWindowRadius();
};

#endif // CONTAINER_H
