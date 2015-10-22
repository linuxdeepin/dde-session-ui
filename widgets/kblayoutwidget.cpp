#include <QDebug>
#include "kblayoutwidget.h"

KbLayoutWidget::KbLayoutWidget(QStringList buttons, QWidget *parent)
    : QListWidget(parent)
{
    m_buttons = buttons;
    qDebug() << "m_buttons:" << m_buttons;


    initUI();
    initConnect();
    initData(buttons);
    updateUI();

}
void KbLayoutWidget::initConnect() {
}
void KbLayoutWidget::initUI() {

    for (int i = 0; i < m_buttons.length(); i++) {
        QPushButton* itemButton = new QPushButton;
        itemButton->setText(m_buttons[i]);
        itemButton->setFixedSize(400, 40);
        itemButton->setStyleSheet("background-color: rgba(255, 255, 255, 0);\
                                  text-align: left; \
                                   border: none;");
        qDebug() << "itemButton:" << itemButton->height() << this->height();

        QFrame* borderFrame = new QFrame;
        QVBoxLayout* borderLayout = new QVBoxLayout;
        borderLayout->setContentsMargins(20, 0, 0, 0);
        borderLayout->setSpacing(0);
        borderLayout->addWidget(itemButton);
        borderFrame->setLayout(borderLayout);
        borderFrame->setFixedSize(400, 40);

        QListWidgetItem* item = new QListWidgetItem(this);
        item->sizeHint();
        this->addItem(item);
        setItemWidget(item,  borderFrame);
        this->setGridSize(QSize(400, 40+4));
        this->setFixedWidth(400);
    }
    this->setFixedHeight(240);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background-color: rgba(255, 255, 255, 30);\
                   border: 1px solid white;\
                   border-radius: 4px;");

}
void KbLayoutWidget::initData(QStringList buttons) {
    for (int i = 0; i < buttons.length(); i++) {
        m_buttons << buttons[i];
    }
}
void KbLayoutWidget::updateUI() {

}
KbLayoutWidget::~KbLayoutWidget(){
}
