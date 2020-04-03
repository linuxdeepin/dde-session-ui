#include "sourcelabel.h"

#include <QBoxLayout>
#include <QWebEngineView>
#include <QWebEnginePage>
#include <QWebChannel>
#include <QWebEngineSettings>
#include <QFile>

void Document::setText(const QString &text)
{
    if (text == m_text)
        return;
    m_text = text;
    emit textChanged(m_text);
}

SourceLabel::SourceLabel(QWidget *parent)
    : QLabel(parent)
    , m_content(new Document)
    , m_layout(new QHBoxLayout(this))
    , m_view(new QWebEngineView(this))
{
    m_layout->setSpacing(0);
    m_layout->setMargin(0);
    m_layout->addWidget(m_view);

    QWebEnginePage *page=new QWebEnginePage(this);
    m_view->setPage(page);
    m_view->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars,false);
    connect(page,&QWebEnginePage::loadFinished,this,&SourceLabel::loadFinished);

    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject(QStringLiteral("content"), m_content);
    page->setWebChannel(channel);
    m_view->setUrl(QUrl("qrc:/index.html"));
}

void SourceLabel::setText(const QString &text)
{
    m_content->setText(text);
}
