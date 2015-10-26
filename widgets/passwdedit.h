#ifndef PASSWDEDIT
#define PASSWDEDIT

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include <libdui/dbuttonlist.h>

#include "util_parsexml.h"

class PassWdEdit : public QFrame
{
    Q_OBJECT
public:
    PassWdEdit(QString iconId, QWidget* parent=0);
    ~PassWdEdit();

    QMap<QString, QString> keybdLayoutMap;
    QMap<QString, QString> keybdLayoutMainDescriptionMap;
    QMap<QString, QString> keybdLayoutShortDescriptionMap;
signals:
    void submit();
    void keybdLayoutButtonClicked();
    void updateKeyboardStatus();
public slots:
    void setLineEditRightImage(QString imageUrl);
    QString getText();

    void getCurrentKeyboardLayout(QString username);

    void updateKeybordLayoutStatus(const QString &username);
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
    QStringList m_keyboardList;

    QStringList keyboardLayoutTextList;
//    KeyboardLayoutFrame* m_keybdLayoutFrame;

    ParseXML* m_parseMainDescriptionXml;
    ParseXML* m_parseShortDescriptionXml;
};
#endif // PASSWDEDIT
