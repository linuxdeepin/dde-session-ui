#ifndef LOGOWIDGET
#define LOGOWIDGET

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class LogoWidget: public QFrame
{
    Q_OBJECT
public:
    LogoWidget(QWidget* parent=0);
    ~LogoWidget();
private:
    void initUI();
    QString getVersion();
    QLabel* m_logoLabel;
    QLabel* m_logoVersionLabel;
    QVBoxLayout* m_logoLeftSideLayout;
    QVBoxLayout* m_logoRightSideLayout;
    QHBoxLayout* m_logoLayout;
};
#endif // LOGOFRAME

