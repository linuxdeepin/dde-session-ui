#ifndef APPBODY_H
#define APPBODY_H

#include <QWidget>

class AppBody : public QWidget
{
    Q_OBJECT
public:
    explicit AppBody(QWidget *parent = 0);
    void setText(const QString & text);

protected:
    void paintEvent(QPaintEvent * event);

private:
    const QString holdTextInRect(const QFontMetrics &fm, const QString &text, const QRect &rect) const;

private:
    QString m_bodyText;
};

#endif // APPBODY_H
