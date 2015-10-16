#ifndef PASSWDEDIT
#define PASSWDEDIT
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>
#include <QPushButton>
class PassWdEdit : public QFrame
{
    Q_OBJECT
public:
    PassWdEdit(QString iconId, QWidget *parent=0);
    ~PassWdEdit();
public slots:
    void setLineEditRightImage(QString imageUrl);
    QString getText();
    void setKeyBoardLayoutButtonStatus(bool show);
    void testing();
signals:
    void jumpTo();
private:
    QLineEdit* m_lineEdit;
    QPushButton* m_keyboardButton;
    QPushButton* m_iconButton;
    QHBoxLayout* m_Layout;
};
#endif // PASSWDEDIT
