#ifndef PASSWDEDIT
#define PASSWDEDIT

#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <libdui/dbuttonlist.h>

#include "util_updateui.h"
#include "util_settings.h"

class PassWdEdit : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool alert READ alert NOTIFY alertChanged DESIGNABLE true)

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
    void alertChanged(const int alert);

    void leftKeyPressed();
    void rightKeyPressed();

public slots:
    QString getText();

    void updateKeybordLayoutStatus(const QString &username);
    void updateKeybdLayoutUI(QStringList keybdList);

    void show();
    void hide();

    void setAlert(bool alert);
    inline bool alert() const {return m_alert;}
    void lineEditGrabKeyboard();
protected:
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:
    void initUI();
    void initConnect();
    void initData();

    bool m_alert = false;

    QLineEdit* m_lineEdit;
    QString m_iconId;
    QPushButton* m_keyboardButton;
    QPushButton* m_iconButton;
    QHBoxLayout* m_Layout;

    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_showAni;
    QPropertyAnimation *m_hideAni;
};
#endif // PASSWDEDIT
