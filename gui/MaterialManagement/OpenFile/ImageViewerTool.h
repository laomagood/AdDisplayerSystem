#ifndef ImageViewerTool_H
#define ImageViewerTool_H

#include <QWidget>
#include <QPushButton>
class ImageViewerAttributes;

namespace Ui {
class ImageViewerTool;
}

class ImageViewerTool : public QWidget
{
    Q_OBJECT

    enum ZoomState {
        ZOOM_IN,
        ZOOM_OUT,
        NO_ACTION
    };

public:
    explicit ImageViewerTool(QWidget *parent = nullptr);
    ~ImageViewerTool();
    void zoomInImage();                                     // 放大图像
    void zoomOutImage();                                    // 缩小图像
    void presetImage();                                     // 预置图像
    QImage loadImage(const QString &fileName);              // 加载图像

signals:
    void zoomValueChange(int zoomValue);                    // 缩放值更改

protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::ImageViewerTool *ui;

    void initializeUi();                            // 初始化界面
    void initializeData();                          // 初始化数据
    void handleEvents();                            // 信号与槽处理
    void adjustImageRect();                         // 调整图像Rect
    QPoint boundaryDetection();                     // 移动图像边界检测,返回应该移动距离

    void resizeWidget();                            // 调整widget尺寸

//    QList<ApprovalBean> m_nApprovalBeanList;        // ApprovalBean列表

    QImage m_image;                                 // 图像
    QRect m_imageRectOriginal;                      // 原本图片大小
    QRect m_imageRect;                              // 调整后图像Rect
    int m_zoomValue;                                // 缩放值
    ZoomState m_zoomState;                          // 缩放状态
    QPoint m_mousePos;                              // 记录鼠标位置(滚动中键)

    QPoint m_pressedPos;                            // 鼠标按下位置
    bool m_bPressed;                                // 鼠标是否按下
    int m_offsetX;                                  // 鼠标移动的距离x方向4
    int m_offsetY;                                  // 鼠标移动的距离y方向
    ImageViewerAttributes *m_pImageViewerAttributes;// 图片属性
};

#endif // ImageViewerTool_H
