#ifndef ICONBUTTON_H
#define ICONBUTTON_H
#include <DWidget>

DWIDGET_USE_NAMESPACE

/*!
 * \~chinese \class IconButton
 * \~chinese \brief 自定义一个控件,继承于DWidget
 */
class ClearButton : public DWidget
{
    Q_OBJECT
public:
    explicit ClearButton(QWidget *parent = nullptr);
    /*!
     * \~chinese \name IconButton
     * \~chinese \brief 创建一个带文字的按钮
     * \~chinese \param text 按钮上需要显示的文字
     */
    explicit ClearButton(const QString &iconPath, QWidget *parent = nullptr);

    /*!
     * \~chinese \name setText
     * \~chinese \brief 设置按钮上显示的文字
     * \~chinese \param text 按钮上需要显示的文字
     */
    void setIconPath(const QString &iconPath);
    /*!
     * \~chinese \name setFocusState
     * \~chinese \brief 控制按钮被选中时,设置m_hasFocus的状态
     * \~chinese \param has 是否被选中
     */
    void setFocusState(bool has);
    /*!
     * \~chinese \name setBackOpacity
     * \~chinese \brief 设置控件背景不透明度
     * \~chinese \param opacity 不透明度,取值范围为(0-255)
     */
    void setBackOpacity(int opacity);
    /*!
     * \~chinese \name setRadius
     * \~chinese \brief 设置控件圆角半径
     * \~chinese \param radius 半径,当输入的半径<=0时控件显示为直角
     */
    void setRadius(int radius);

Q_SIGNALS:
    void clicked();

private:
    QString m_iconPath;
    bool m_hasFocus;
    bool m_hover;
    int m_opacity;
    int m_radius;
    bool m_hasBackColor;

protected:
    /*!
     * \~chinese \name paintEvent
     * \~chinese \brief 绘制控件的样式
     * \~chinese \param event 事件
     */
    virtual void paintEvent(QPaintEvent *event) override;
    /*!
     * \~chinese \name mousePressEvent
     * \~chinese \brief 鼠标点击事件
     * \~chinese \param event 事件
     */
    // TODO 鼠标事件就不要传递给父对象了，包括mouseReleaseEvent
    virtual void mousePressEvent(QMouseEvent *event) override;
    /*!
     * \~chinese \name enterEvent
     * \~chinese \brief 鼠标移动到控件上产生的事件,产生悬停的效果
     * \~chinese \param event 事件
     */
    virtual void enterEvent(QEvent *event) override;
    /*!
     * \~chinese \name leaveEvent
     * \~chinese \brief 鼠标移出控件产生的事件,消除悬停的效果
     * \~chinese \param event 事件
     */
    virtual void leaveEvent(QEvent *event) override;
};

#endif // ICONBUTTON_
