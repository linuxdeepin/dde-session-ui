/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     kirigaya <kirigaya@mkacg.com>
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef APPBODYLABEL_H
#define APPBODYLABEL_H

#include <QFrame>

class appBodyLabel : public QFrame
{
    Q_OBJECT
public:
    explicit appBodyLabel(QWidget *parent = nullptr);
    void setText(const QString &text);
    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    void setAlignment(Qt::Alignment alignment);

private:
    const QString holdTextInRect(const QFontMetrics &fm, const QString &text, const QRect &rect) const;
    void resizeEvent(QResizeEvent *e) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void updateLineCount();

    QString m_text;
    int m_lineCount = 0;
    Qt::Alignment m_alignment;
};

#endif // APPBODYLABEL_H
