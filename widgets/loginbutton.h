#ifndef LOGINBUTTON_H
#define LOGINBUTTON_H

#include <QObject>
#include <QWidget>

class QLabel;
class LoginButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
public:
    explicit LoginButton(QWidget *parent = nullptr);

    enum State {
        Normal,
        Hover,
        Press
    };

    inline const QString icon() { return m_iconPath; }

public slots:
    void setText(const QString &text);
    void setIcon(const QString &icon);

signals:
    void iconChanged(QString icon);
    void clicked();

protected:
    void enterEvent(QEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QLabel *m_text;
    QLabel *m_icon;
    QString m_iconPath;
    State m_state;
};

#endif // LOGINBUTTON_H
