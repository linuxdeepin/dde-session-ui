#ifndef VIRTUALKBINSTANCE_H
#define VIRTUALKBINSTANCE_H

#include <QObject>

class VirtualKBInstance : public QObject
{
    Q_OBJECT
public:
    static VirtualKBInstance &Instance();
    QWidget *virtualKBWidget();

    void init();

signals:
    void initFinished();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    VirtualKBInstance(QObject *parent = nullptr);
    VirtualKBInstance(const VirtualKBInstance &) = delete;

private:
    QWidget *m_virtualKBWidget;
};

#endif // VIRTUALKBINSTANCE_H
