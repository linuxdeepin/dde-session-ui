#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <DSuggestButton>

DWIDGET_USE_NAMESPACE

class QScrollArea;
class QPushButton;
class QCheckBox;
class QLabel;
class Content : public QWidget
{
    Q_OBJECT
public:
    explicit Content(QWidget *parent = nullptr);

signals:

public slots:
    void setSource(const QString& source);
    void setAllowCheckBoxText(const QString& text);

private:
    QScrollArea* m_scrollArea;
    QCheckBox* m_acceptCheck;
    QPushButton* m_cancelBtn;
    DSuggestButton* m_acceptBtn;
    QLabel* m_source;
};

#endif // CONTENT_H
