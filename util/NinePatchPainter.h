#ifndef NINEPATCHPAINTER_H
#define NINEPATCHPAINTER_H

class QRect;
class QMargins;
class QPainter;
class QPixmap;
class NinePatchPainterPrivate;
/**
 * @brief The NinePatchPainter class 用于九宫格的方式绘图，当背景图和需要绘制的范围不一样大时，
 * 能够最大限度的保证绘制出来的效果跃背景图接近。类似quick里的BorderImage
 * 第一步：将传入的图片切割为9张pixmap，QMargins &borderImageBorders 用来控制切割图片的位置
 * 第二步：将图片贴入指定区域，QMargins &windowPaddings 与 rect 用来控制图片贴入的位置
 */

class NinePatchPainter
{
public:
    /**
    * @brief 使用pixmap，九宫格的4个边宽，水平垂直的缩放方式创建NinePatchPainter对象
    * @param background 背景图
    * @param left   图片的左边宽
    * @param top    图片的上边宽
    * @param right  图片的右边宽
    * @param bottom 图片的下边宽
    * @param horizontalStreth   水平是否拉伸，默认true
    * @param verticalStreth     垂直是否拉伸，默认true
    */
    NinePatchPainter(const QPixmap &background,
                     int left, int top, int right, int bottom,
                     bool horizontalStreth = true, bool verticalStreth = true);
    ~NinePatchPainter();
    /**
    * @brief 在rect中使用九宫格的方式进行绘图，rect 是窗口的尺寸 left top right bottom 为窗口的边距
    */
    void paint(QPainter *painter, const QRect &rect, int left, int top, int right, int bottom) const;

private:
    NinePatchPainterPrivate *d;
};

#endif // NINEPATCHPAINTER_H
