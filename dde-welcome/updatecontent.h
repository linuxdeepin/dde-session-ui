#ifndef UPDATECONTENT_H
#define UPDATECONTENT_H

#include <QWidget>
#include <QPushButton>

class UpdateContent : public QWidget
{
    Q_OBJECT
public:
    explicit UpdateContent(const QString &version, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QPushButton *m_enterBtn;
};

#endif // UPDATECONTENT_H
