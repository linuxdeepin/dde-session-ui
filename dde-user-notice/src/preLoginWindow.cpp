// SPDX-FileCopyrightText: 2026 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "preLoginWindow.h"

#include "noticeconfig.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFutureWatcher>
#include <QKeyEvent>
#include <QScreen>
#include <QtConcurrent>

namespace {

// 文件读取放在工作线程，避免大文件阻塞界面
QString readNoticeContent(const QString &path)
{
    if (path.isEmpty()) {
        qWarning() << "User notice path is empty";
        return QString();
    }

    QFile instructionFile(path);
    if (!instructionFile.exists()) {
        qWarning() << "User notice file not exists:" << path;
        return QString();
    }
    if (!instructionFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open user notice file:" << path;
        return QString();
    }

    const QByteArray buf = instructionFile.readAll();
    instructionFile.close();

    return QString::fromUtf8(buf);
}

} // namespace

PreLoginWindow::PreLoginWindow(QWidget *parent)
    : QFrame(parent)
    , m_contentLabel(new TextBrowser(this))
    , m_commitButton(new QPushButton(this))
    , m_layout(new QVBoxLayout(this))
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    initUi();
    initConnect();
}

void PreLoginWindow::initUi()
{
    m_commitButton->setText(tr("OK"));
    m_commitButton->setFixedSize(214, 36);
    m_commitButton->setEnabled(false);

    m_layout->setSpacing(0);
    m_layout->setContentsMargins(0, 0, 0, 0);

    m_layout->addSpacing(60);
    m_layout->addWidget(m_contentLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_commitButton, 0, Qt::AlignHCenter | Qt::AlignBottom);
    m_layout->addSpacing(60);

    setLayout(m_layout);
    setContentsMargins(0, 8, 0, 0);

    const QString noticePath = UserNoticeConfig::noticePath();

    auto *watcher = new QFutureWatcher<QString>(this);
    connect(watcher, &QFutureWatcher<QString>::finished, this, [this, watcher] {
        const QString content = watcher->result();
        watcher->deleteLater();
        if (content.isEmpty()) {
            QCoreApplication::exit(0);
            return;
        }
        m_contentLabel->setText(content);
        m_commitButton->setEnabled(true);
        m_commitButton->setFocus();
    });
    watcher->setFuture(QtConcurrent::run(readNoticeContent, noticePath));

    setFocusPolicy(Qt::FocusPolicy::ClickFocus);
}

void PreLoginWindow::initConnect()
{
    connect(m_commitButton, &QPushButton::clicked, this, &PreLoginWindow::acknowledge);
}

void PreLoginWindow::acknowledge()
{
    if (!m_commitButton->isEnabled())
        return;

    QApplication::setOverrideCursor(Qt::BlankCursor);
    // 不能用 qApp->quit()：QEvent::Quit 会先关闭所有顶层窗口，
    // 而 FullScreen::closeEvent() 会 ignore 掉关闭事件，导致 quit 被取消、进程残留，
    // greeter 脚本同步等待本进程退出，于是永远进不到登录界面。
    QCoreApplication::exit(0);
}

void PreLoginWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        acknowledge();
        event->accept();
        return;
    }
    QWidget::keyPressEvent(event);
}

PreLoginWindow *PreLoginWindow::instance()
{
    static PreLoginWindow *instance = new PreLoginWindow();
    return instance;
}

void PreLoginWindow::updateBrowserSize(int width)
{
    double scale = width / 1920.0;
    m_contentLabel->setFixedWidth(800 * scale);
}

void PreLoginWindow::showEvent(QShowEvent *event)
{
    qDebug() << "Content label geometry:" << geometry() << m_contentLabel->geometry();
    updateBrowserSize(this->width());
    QWidget::showEvent(event);
}
