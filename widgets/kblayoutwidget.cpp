#include <QDebug>
#include "kblayoutwidget.h"

LayoutButton::LayoutButton(QString text, QWidget *parent)
    : QPushButton(parent)
{
    setObjectName("LayoutButton");
    setCheckable(true);

    m_text = text;
    m_iconLabel = new QLabel(this);
    QPixmap pix(":img/tick_hover.png");
    m_iconLabel->setPixmap(pix);
    m_iconLabel->setMinimumSize(pix.size());
    m_textLabel = new QLabel;
    m_textLabel->setObjectName("LayoutTextLabel");
    m_textLabel->setText(text);
    m_textLabel->setStyleSheet("QLabel{ color: white;}");
    m_iconLabel->hide();

    QHBoxLayout* m_Layout = new QHBoxLayout;
    m_Layout->setContentsMargins(20, 0, 0, 0);
    m_Layout->setSpacing(0);

    m_Layout->addSpacing(5);
    m_Layout->addWidget(m_textLabel);
    m_Layout->addStretch();
    setLayout(m_Layout);

    m_iconLabel->move(this->x() + 5, this->y() + 1);
    this->setStyleSheet("QPushButton { background-color: transparent; border: none;} \
                         QPushButton:hover { background-color: rgba(0, 0, 0, 0.4); border: none;}");
}
void LayoutButton::enterEvent(QEvent *event) {
    emit mouseEnter(m_text);
    Q_UNUSED(event);
//    qDebug() << "enterEvent;";
    updateStyleSelf();
}

void LayoutButton::leaveEvent(QEvent *event) {
    emit mouseLeave(m_text);
    Q_UNUSED(event);
//    qDebug() << "leaveEvent;";
     updateStyleSelf();
}

void LayoutButton::updateStyleSelf() {
    style()->unpolish(this);
    style()->polish(this);
    this->update();
}

void LayoutButton::setButtonChecked(bool checked) {
    if (checked) {
        this->setChecked(true);
        m_textLabel->setProperty("Checked", true);
        m_textLabel->setStyleSheet("QLabel[Checked = \"true\"] { color: #01BDFF;}");
        m_iconLabel->show();
    } else {
        this->setChecked(false);
        m_textLabel->setProperty("Checked", false);
        m_textLabel->setStyleSheet("QLabel { color: white;}");
        m_iconLabel->hide();
    }

    updateStyleSelf();

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
        connect(m_layoutButtons.at(i), SIGNAL(onlyOneChecked(QString)), this, SIGNAL(setButtonClicked(QString)));
    }

}

void KbLayoutWidget::initUI() {
    setObjectName("KeyboardLayoutFrame");
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSelectionMode(QAbstractItemView::NoSelection);
    setResizeMode(Adjust);

    for (int i = 0; i < m_buttons.length(); i++) {
        LayoutButton* itemButton = new LayoutButton(m_buttons[i]);
        m_layoutButtons.append(itemButton);
        itemButton->setFixedSize(DDESESSIONCC::PASSWDLINEEIDT_WIDTH, DDESESSIONCC::LAYOUTBUTTON_HEIGHT);

        QFrame* borderFrame = new QFrame;
        borderFrame->setObjectName("LayoutBorderFrame");
        QVBoxLayout* borderLayout = new QVBoxLayout;
        borderLayout->setContentsMargins(0, 0, 0, 0);
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

    updateStyle(":/skin/keybdlayoutwidget.qss", this);
}

void KbLayoutWidget::setButtonsChecked(QString text) {
    qDebug() << "XsetButtonsChecked:" << text;
    for (int i = 0; i < m_layoutButtons.length(); i++) {
        if (m_layoutButtons.at(i)->m_text != text) {
            m_layoutButtons.at(i)->setButtonChecked(false);
        }
    }

    emit setButtonClicked(text);
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
