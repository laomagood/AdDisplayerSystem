#ifndef CanvasWidget_H
#define CanvasWidget_H

#include <QWidget>
#include <QRect>
#include <QJsonDocument>
class ControlWidget;

/**
 * @brief The CanvasWidget class
 * 用来放置各种controlWidget的画布
 * 可以设置大小（制作大小不与显示大小一致）与背景颜色
 * 删除控件（未实现）
 * 点击控件时上移到stack顶部，失去焦点后回到原来位置（未实现）
 */

class CanvasWidget : public QWidget
{
    Q_OBJECT
public:
    enum FillStyle { TILE, STRETCH };                               // 背景图片填充方式

    explicit CanvasWidget(QWidget *parent = 0);

    QList<ControlWidget *> getListControlWidget() const;            // 获取所有控件
    void addControlWidget(ControlWidget *controlWidget);            // 添加控件
    void removeControlWidget(ControlWidget *controlWidget);         // 移除控件
    void moveLayerUP(ControlWidget *controlWidget);                 // 上移一层
    void moveLayerDown(ControlWidget *controlWidget);               // 下移一层
    void moveLayerTop(ControlWidget *controlWidget);                // 移动顶层
    void moveLayerBottom(ControlWidget *controlWidget);             // 移动底层

    void setBackgroundColor(const QColor &backgroundColor);         // 设置背景颜色
    void setBackgroundImage(const QString &imageFilePath, FillStyle fillStyle = STRETCH);       // 设置背景图片

    void setCanvasSize(int w, int h);                               // 设置画布大小
    void setControlWidgetGeometry(ControlWidget *controlWidget, int x, int y, int w, int h);    // 设置控件位置大小

    QColor getBackgroundColor() const;                              // 获取背景颜色
    QString getBackgroundImage() const;                             // 获取背景图片

    QList<QRect> getListRect();                                     // 获取所有控件的矩形
    void selectCanvasWidget();                                      // 设置当前widget,用来切换属性控件

    QJsonDocument toJson();                                         // 转为Json
    bool toBean(const QJsonDocument &doc);                          // 转为Bean
    QString generateThumbnail();                                    // 生成缩略图

protected:
//    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

signals:
    void focusWidget(QWidget *);                                     // 发送当前选择的widget信号，用于属性框的切换
    void updateControlList(CanvasWidget *);                          // 更新控件列表

private:
    void initializeUi();                                             // 初始化界面
    void handleEvents();                                             // 信号与槽处理
    QString pixmapTostring(const QPixmap& pixmap);                   // 图片转二进制

    QColor m_nBackgroundColor;                                       // 背景颜色
    QString m_nBackgroundImage;                                      // 背景图片
    FillStyle m_nFillStyle;                                          // 填充样式

    QList<ControlWidget *> m_listControlWidget;                      // 部件的List列表

    QPoint m_nReleaseSize;                                           // 发布尺寸
    float m_nControlWidgetScale;                                     // 控件缩放比例
};

#endif // CanvasWidget_H
