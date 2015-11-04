#include <QDebug>
#include "kblayoutwidget.h"

LayoutButton::LayoutButton(QString text, QWidget *parent)
    : QPushButton(parent)
{
    setObjectName("LayoutButton");
    setCheckable(true);

    m_text = text;
    m_iconLabel = new QLabel;
    QPixmap pix(":img/tick_hover.png");
    m_iconLabel->setPixmap(pix);
    m_iconLabel->setMinimumSize(pix.size());
    m_textLabel = new QLabel;
    m_textLabel->setText(text);

    m_iconLabel->hide();

    QHBoxLayout* m_Layout = new QHBoxLayout;
    m_Layout->setContentsMargins(2, 0, 0, 0);
    m_Layout->setSpacing(0);
    m_Layout->addWidget(m_iconLabel);
    m_Layout->addSpacing(5);
    m_Layout->addWidget(m_textLabel);
    m_Layout->addStretch();
    setLayout(m_Layout);

}
void LayoutButton::enterEvent(QEvent *event) {
    emit mouseEnter(m_text);
    qDebug() << "enterEvent;";
    updateStyle();
}

void LayoutButton::leaveEvent(QEvent *event) {
    emit mouseLeave(m_text);
    qDebug() << "leaveEvent;";
     updateStyle();
}

void LayoutButton::updateStyle() {
    style()->unpolish(this);
    style()->polish(this);
    this->update();
}

void LayoutButton::setButtonChecked(bool checked) {
    if (checked) {
        this->setChecked(true);
        m_iconLabel->show();
    } else {
        this->setChecked(false);
        m_iconLabel->hide();
    }
    updateStyle();
}

void LayoutButton::OnlyMeChecked(bool checked) {
    setButtonChecked(checked);
    emit onlyOneChecked(m_text);
}

LayoutButton::~LayoutButton()
{
}

KbLayoutWidget::KbLayoutWidget(QStringList buttons, QWidget *parent)
    : QListWidget(parent)
{
    if (!buttons.isEmpty()) {
        m_buttons.clear();
        m_buttons = buttons;
    }
    qDebug() << "m_buttons:" << m_buttons;
    initUI();
    initConnect();
    initData(buttons);
    updateUI();

}

void KbLayoutWidget::initConnect() {
    const int count = m_layoutButtons.length();
    for (int i = 0; i != count; ++i)
    {
        connect(m_layoutButtons.at(i), SIGNAL(clicked(bool)), m_layoutButtons.at(i), SLOT(OnlyMeChecked(bool)));
        connect(m_layoutButtons.at(i), SIGNAL(onlyOneChecked(QString)), this, SLOT(setButtonsChecked(QString)));
    }
}

void KbLayoutWidget::initUI() {
    setObjectName("KeyboardLayoutFrame");
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::NoSelection);

    for (int i = 0; i < m_buttons.length(); i++) {
        LayoutButton* itemButton = new LayoutButton(m_buttons[i]);
        m_layoutButtons.append(itemButton);
        itemButton->setFixedSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, DDESESSIONCC::LAYOUTBUTTON_HEIGHT);
//        qDebug() << "itemButton:" << itemButton->height() << this->height();

        QFrame* borderFrame = new QFrame;
        borderFrame->setObjectName("LayoutBorderFrame");
        QVBoxLayout* borderLayout = new QVBoxLayout;
        borderLayout->setContentsMargins(20, 0, 0, 0);
        borderLayout->setSpacing(0);
        borderLayout->addWidget(itemButton);
        borderFrame->setLayout(borderLayout);
        borderFrame->setFixedSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, DDESESSIONCC::LAYOUTBUTTON_HEIGHT);

        QListWidgetItem* item = new QListWidgetItem(this);
        item->sizeHint();
        this->addItem(item);
        setItemWidget(item,  borderFrame);
        this->setGridSize(QSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, DDESESSIONCC::LAYOUTBUTTON_HEIGHT));
        this->setFixedWidth(DDESESSIONCC::PASSWDLINEEIDT_WIDTH);
    }
    this->setFixedHeight(DDESESSIONCC::LAYOUTBUTTON_HEIGHT*3);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

}

void KbLayoutWidget::setButtonsChecked(QString text) {
    qDebug() << "setButtonsChecked:" << text;
    for (int i = 0; i < m_layoutButtons.length(); i++) {
        if (m_layoutButtons.at(i)->m_text != text) {
            m_layoutButtons.at(i)->setButtonChecked(false);
        }
    }
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
