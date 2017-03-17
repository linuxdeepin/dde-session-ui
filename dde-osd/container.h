#ifndef CONTAINER_H
#define CONTAINER_H

#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

class QHBoxLayout;
class Container : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit Container(QWidget *parent = 0);

    void setContent(QWidget *content);

private:
    QHBoxLayout *m_layout;
};

#endif // CONTAINER_H
