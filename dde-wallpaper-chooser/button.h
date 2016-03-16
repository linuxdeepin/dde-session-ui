#ifndef BUTTON_H
#define BUTTON_H

#include <QPushButton>

class Button : public QPushButton
{
public:
    Button(QWidget * parent = 0);

    void setAttract(bool);
};

#endif // BUTTON_H
