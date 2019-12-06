#ifndef ControlBorderWidget_H
#define ControlBorderWidget_H

#include <QWidget>

/**
 * @brief The ControlBorderWidget class
 * 控件边框用显示示控件名称，以及用来显示控件是否被选中的状态
 * 1.当控件获取焦点时显示边框，失去焦点隐藏边框
 * 2.当控件没有素材时显示控件背景颜色与控件名称，当控件有素材时隐藏背景颜色与控件名称
 */

class ControlBorderWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief ControlBorderWidget
     * @param controlName 需要显示控件的名称
     * @param parent 父对象
     */
    explicit ControlBorderWidget(const QString &controlName, QWidget *parent = 0);
    /**
     * @brief setIsBackgroundVisible 设置背景与说明文字是否可见
     * @param visible true:背景及文字可见 false:背景及文字不可见
     */
    void setBackgroundVisible(bool visible);
    void setBorderVisible(bool visible);
    void setControlName(const QString &controlName);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    QString m_controlName;      // 名称
    bool m_backgroundVisible;   // 背景与说明文字是否可见(用于是否素材为空)
    bool m_borderVisible;       // 边框是否可见(用于焦点)
};

#endif // ControlBorderWidget_H
