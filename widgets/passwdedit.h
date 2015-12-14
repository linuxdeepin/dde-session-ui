#ifndef PASSWDEDIT
#define PASSWDEDIT

#include <QFrame>
#include <QLineEdit>
#include <QKeyEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

#include <libdui/dbuttonlist.h>
#include <libdui/dimagebutton.h>

#include "util_updateui.h"
#include "util_settings.h"

DUI_USE_NAMESPACE

class PassWdEdit : public QFrame
{
    Q_OBJECT
    Q_PROPERTY(bool alert READ alert NOTIFY alertChanged DESIGNABLE true)

public:
    PassWdEdit(QWidget* parent=0);
    ~PassWdEdit();

    QMap<QString, QString> keybdLayoutMap;
    QStringList keybdLayoutMainDescriptionList;
    QMap<QString, QString> keybdLayoutShortDescriptionMap;

    QStringList keyboardLayoutList;
    UtilSettings* utilSettings = NULL;
    QTimer* getFocusTimer;
signals:
    void submit();
    void keybdLayoutButtonClicked();
    void updateKeyboardStatus();
    void alertChanged(const int alert);
    void focusIn();

    void leftKeyPressed();
    void rightKeyPressed();

public slots:
    QString getText();
    void setEnterBtnStyle(const QString &normalPic, const QString &hoverPic, const QString &pressedPic);

    void updateKeybordLayoutStatus(const QString &username);
    void updateKeybdLayoutUI(QStringList keybdList);

    void show();
    void hide();

    void setAlert(bool alert, const QString &text = QString());
    inline bool alert() const {return m_alert;}
    void lineEditGrabKeyboard();

protected:
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *) Q_DECL_OVERRIDE;
    bool eventFilter(QObject *o, QEvent *e) Q_DECL_OVERRIDE;

private:

    int m_timerCount = 0;

    void initUI();
    void initConnect();
    void initData();

    bool m_alert = false;
    bool m_alert_enter = false;

    QLineEdit* m_lineEdit;
    QPushButton* m_keyboardButton;
    DImageButton* m_iconButton;
    QHBoxLayout* m_Layout;

    QGraphicsOpacityEffect *m_opacityEffect;
    QPropertyAnimation *m_showAni;
    QPropertyAnimation *m_hideAni;
};
#endif // PASSWDEDIT
