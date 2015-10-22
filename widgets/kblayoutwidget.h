#ifndef KBLAYOUTWIDGET
#define KBLAYOUTWIDGET

#include <QPushButton>
#include <QListWidget>
#include <QHBoxLayout>

class KbLayoutWidget: public QListWidget
{
    Q_OBJECT
public:
    KbLayoutWidget(QStringList buttons, QWidget* parent = 0);
    ~KbLayoutWidget();

    void initData(QStringList buttons);
    void initUI();
    void initConnect();
    void updateUI();
private:
    QHBoxLayout* m_Layout;
    QStringList m_buttons;
};
#endif // KEYBOARDLAYOUTFRAME

