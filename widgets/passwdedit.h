#ifndef PASSWDEDIT
#define PASSWDEDIT

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include <libdui/dbuttonlist.h>

#include "util_settings.h"

class PassWdEdit : public QFrame
{
    Q_OBJECT
public:
    PassWdEdit(QString iconId, QWidget* parent=0);
    ~PassWdEdit();

    QMap<QString, QString> keybdLayoutMap;
    QStringList keybdLayoutMainDescriptionList;
    QMap<QString, QString> keybdLayoutShortDescriptionMap;

    QStringList keyboardLayoutList;
    UtilSettings* utilSettings = NULL;
signals:
    void submit();
    void keybdLayoutButtonClicked();
    void updateKeyboardStatus();
public slots:
    void setLineEditRightImage(QString imageUrl);
    QString getText();

    void updateKeybordLayoutStatus(const QString &username);
    void updateKeybdLayoutUI(QStringList keybdList);
protected:
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();
    void initData();

    void initInsideFrame();
    QFrame* m_insideFrame;

    QLineEdit* m_lineEdit;
    QString m_iconId;
    QPushButton* m_keyboardButton;
    QPushButton* m_iconButton;
    QHBoxLayout* m_Layout;
};
#endif // PASSWDEDIT
