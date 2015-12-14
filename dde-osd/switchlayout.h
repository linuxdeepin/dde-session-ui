#ifndef SWITCHLAYOUT_H
#define SWITCHLAYOUT_H

#include "dbus/layoutdbus.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVariantAnimation>

class SwitchLayout : public QWidget
{
    Q_OBJECT
public:
    explicit SwitchLayout(QWidget *parent = 0);

    void setKeyboard();
    void loadSwitchLayout();
    void highlightNextLayout();
    void hideLayout();
    void showLayout();
    int layoutCount();
    void reHiglightKeyboard();
    void resizeParent();

private:
    void initGlobalVar(QWidget *parent);
    void deleteOsd();
    void calculateKeyboardSize();
    void initKeyboard();
    void initListWidget();
    void fillListWidget();
    void addAnimation();
    void reAlignCurrentIndex();

    LayoutDbus* m_LayoutInterface;
    QListWidget* m_ListWidget;
    QWidget* m_ParentItem;
    QHBoxLayout* m_HBoxLayout;
    QVariantAnimation *m_Animation;
    QFont m_f;
    QSize size;

    QStringList m_KeyboardList;
    QList <QLabel *> m_KeyboradTextList;
    int m_MaxTextWidth;
    int m_KeyboradLayoutHeight;
    int m_CurrentIndexOfKeyBoard = 0;
    int contentY = 0;

    const int KEYBOARD_ITEM_HEIGHT = 36;
    const int LAYOUT_MARGIN = 10;
    const QString KEYBOARD_ITEM_HIGHLIGHT_STYLE = "QLabel{color:#01bdff;font-size:14px;background:rgba(0,0,0,0.7);border:none;border-radius:5px}";
    const QString KEYBOARD_ITEM_NORMAL_STYLE = "QLabel{color:#ffffff;font-size:14px;}";
};

#endif // SWITCHLAYOUT_H
