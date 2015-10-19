#include "userbreathinglabel.h"

#include <QtDebug>

UserBreathingLabel::UserBreathingLabel(QWidget *parent)
    : QLabel(parent), m_opacity(1.0)
{
    m_showAnimation = new QPropertyAnimation(this, "opacity");
    m_hideAnimation = new QPropertyAnimation(this, "opacity");
    label_effect = new QGraphicsOpacityEffect(this);

    connect(this, SIGNAL(opacityChanged()), this, SLOT(setEffect()));
    connect(this, SIGNAL(hideFinished()), this, SLOT(hide()));
}
UserBreathingLabel::~UserBreathingLabel()
{
    delete m_showAnimation;
    delete m_hideAnimation;
}

//////////////////////////////////Test New Function
///
double UserBreathingLabel::opacity() const {
    return m_opacity;
}
void UserBreathingLabel::setOpacity(double opa) {
    if (opa != m_opacity) {
        m_opacity = opa;
        emit opacityChanged();
    }

}

void UserBreathingLabel::showLabel() {
    m_showAnimation->setStartValue(0);
    m_showAnimation->setEndValue(1.0);
    m_showAnimation->start();
    m_showAnimation->setDuration(500);
}

void UserBreathingLabel::hideLabel() {
    m_hideAnimation->setStartValue(1.0);
    m_hideAnimation->setEndValue(0);
    m_hideAnimation->start();
    m_hideAnimation->setDuration(300);
}

void UserBreathingLabel::setEffect() {
    label_effect->setOpacity(m_opacity);
    this->setGraphicsEffect(label_effect);

}
