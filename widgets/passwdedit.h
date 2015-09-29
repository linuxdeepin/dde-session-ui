#ifndef PASSWDEDIT
#define PASSWDEDIT
#include <QtWidgets/QFrame>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QHBoxLayout>

class PassWdEdit : public QFrame
{
    Q_OBJECT
public:
    PassWdEdit(QWidget *parent=0);
    ~PassWdEdit();
public slots:
    void switchStatus(bool isLogin);
private:
    QLineEdit* m_lineEdit;
    QLabel* m_lockLabel;
    QHBoxLayout* m_Layout;
};
#endif // PASSWDEDIT

