#ifndef PASSWDEDIT
#define PASSWDEDIT

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

class PassWdEdit : public QFrame
{
    Q_OBJECT
public:
    PassWdEdit(QString iconId, QWidget* parent=0);
    ~PassWdEdit();

signals:
    void submit();

public slots:
    void setLineEditRightImage(QString imageUrl);
    QString getText();
    void setKeyBoardLayoutButtonStatus(bool show);
    void testing();

private:
    void initUI();
    void initConnect();
    void focusInEvent(QFocusEvent *);

private:
    QLineEdit* m_lineEdit;
    QString m_iconId;
    QPushButton* m_keyboardButton;
    QPushButton* m_iconButton;
    QHBoxLayout* m_Layout;
};
#endif // PASSWDEDIT
