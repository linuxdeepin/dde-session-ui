#ifndef CONTENT_H
#define CONTENT_H

#include <QWidget>
#include <dtkwidget_global.h>

DWIDGET_BEGIN_NAMESPACE
class DSuggestButton;
class DCommandLinkButton;
DWIDGET_END_NAMESPACE

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
    void setCnSource(const QString& source);
    void setEnSource(const QString& language);

private:
    void updateLanguageBtn();
    void updateContent();

private:
    QScrollArea* m_scrollArea;
    QCheckBox* m_acceptCheck;
    QPushButton* m_cancelBtn;
    DTK_WIDGET_NAMESPACE::DSuggestButton* m_acceptBtn;
    QLabel* m_source;
    DTK_WIDGET_NAMESPACE::DCommandLinkButton* m_languageBtn;
    QString m_cn;
    QString m_en;
    bool m_isCn;
};

#endif // CONTENT_H
