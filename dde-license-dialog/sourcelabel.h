#ifndef SOURCELABEL_H
#define SOURCELABEL_H

#include <QLabel>

class QWebEngineView;
class QBoxLayout;

class Document : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER m_text NOTIFY textChanged FINAL)
public:
    explicit Document(QObject *parent = nullptr) : QObject(parent) {}

    void setText(const QString &text);

signals:
    void textChanged(const QString &text);

private:
    QString m_text;
};


class SourceLabel : public QLabel
{
    Q_OBJECT
public:
    explicit SourceLabel(QWidget *parent = nullptr);

public Q_SLOTS:
    void setText(const QString& text);

Q_SIGNALS:
    void loadFinished(bool ok);

private:
    Document *m_content;
    QBoxLayout *m_layout;
    QWebEngineView *m_view;
};

#endif // SOURCELABEL_H
