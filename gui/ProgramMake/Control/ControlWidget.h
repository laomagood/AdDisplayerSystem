#ifndef ControlWidget_H
#define ControlWidget_H

/**
 * @brief The ControlWidget class
 * 各种控件的基类，具有磁吸功能，当获得焦点时显示边框ControlBorderWidget，失去焦点时，隐藏边框
 */

#include <QWidget>
#include <QJsonDocument>
#include <QDebug>
class ControlBorderWidget;

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlWidget(QWidget *parent = 0);
    QRect getRect();                                            // 获取左上相对于父窗口位置

    void setControlName(QString name);
    inline void setControlIconPath(QString path) { m_nControlIconPath = path; }
    inline void setControlType(QString type) { m_nControlType = type; }
    inline QString getControlName() const { return m_nControlName; }
    inline QString getControlIconPath() const { return m_nControlIconPath; }
    inline QString getControlType() { return m_nControlType; }
    void raiseControlBorder();                                  // 将控件框提升
    void setControlBorderBackgroundVisible(bool visible);       // 设置控件边框背景是否可见
    void setControlBorderVisible(bool visible);                 // 设置控件边框是否可见
    void resizeControlBorder();                                 // 调整控件边框大小（有些类重写了resizeEvent，加入这条）

    virtual QJsonDocument toJson() { qDebug() << "toJson"; }    // 虚函数

protected:
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
//    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

signals:
    void removeControl();
    void moveLayerUP();
    void moveLayerDown();
    void select(QWidget*);

private:
    void mouseRegion(const QPoint &cursorGlobalPoint);          // 计算鼠标区域
    void resizeWidget(QMouseEvent *event);                      // 调整窗口大小
    void magneticCalculation(QMouseEvent *event);               // 磁吸计算
    void magneticLine(QRect otherRect, QRect thisRect);         // 磁吸线判断自身那条边线
    bool isInMagneticRange(QRect otherRect, QRect thisRect);    // 矩形角点是否在磁吸范围

    const int PADDING = 3;                                      // 拖动范围
    enum Direction { NONE = 0x00,   TOP,
                     BOTTOM, LEFT,
                     RIGHT,  LEFTTOP,
                     LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP,
                   } m_dir = {NONE};                            // 窗口区域

    const int MAGNETIC_RANGE = 7;                               // 磁力吸附范围
    QPoint m_magneticPointTL;                                   // 磁力吸附点（用来控制应该吸附的坐标）左上
    QPoint m_magneticPointBR;                                   // 磁力吸附点（用来控制应该吸附的坐标）右下

    bool m_isMagneticDragX;                                     // X轴是否已经磁吸：拖拽状态
    bool m_isMagneticDragY;                                     // Y轴是否已经磁吸：拖拽状态
    bool m_isMagneticMoveX;                                     // X轴是否已经磁吸：移动状态
    bool m_isMagneticMoveY;                                     // Y轴是否已经磁吸：移动状态

    bool m_tIsMagnetic;                                         // 上吸附
    bool m_bIsMagnetic;                                         // 下吸附
    bool m_lIsMagnetic;                                         // 左吸附
    bool m_rIsMagnetic;                                         // 右吸附

    QPoint m_dragPosition;                                      // 鼠标位置
    bool m_isPressed;                                           // 鼠标按下标记
    QRect m_rect;                                               // 鼠标按下窗口Rect

    ControlBorderWidget *p_controlBorderWidget;                 // 边框

    QString m_nControlName;                                     // 控件名称
    QString m_nControlIconPath;                                 // 控件图标路径
    QString m_nControlType;                                     // 控件类型
};


#endif // ControlWidget_H
