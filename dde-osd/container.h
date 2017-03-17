#ifndef CONTAINER_H
#define CONTAINER_H

#include <DBlurEffectWidget>

DWIDGET_USE_NAMESPACE

class Container : public DBlurEffectWidget
{
    Q_OBJECT
public:
    explicit Container(QWidget *parent = 0);
};

#endif // CONTAINER_H
