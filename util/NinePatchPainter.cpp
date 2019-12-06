#include "NinePatchPainter.h"
#include <QDebug>
#include <QPixmap>
#include <QList>
#include <QRect>
#include <QPainter>

/*-----------------------------------------------------------------
 *                     NinePatchPainterPrivate
 *----------------------------------------------------------------*/
class NinePatchPainterPrivate
{
public:
    NinePatchPainterPrivate(const QPixmap &background,
                            int left, int top, int right, int bottom,
                            bool horizontalStreth = true, bool verticalStreth = true);
    // 根据九宫格4边的宽度把rect按九宫格分为9个rect：左下，左，左上，上，右上，右，右下，下，中
    QList<QRect> calculateNinePatchRects(const QRect &rect, int left, int top, int right, int bottom) const;
    // 对图片进行缩放
    QPixmap scalePixmap(const QPixmap &pixmap, const QSize &size) const;
public:
    int left;
    int top;
    int right;
    int bottom;
    bool horizontalStretch;
    bool verticalStretch;

    QPixmap leftbottomPixmap;   // 左下
    QPixmap leftPixmap;         // 左
    QPixmap leftTopPixmap;      // 左上
    QPixmap topPixmap;          // 上
    QPixmap rightTopPixmap;     // 右上
    QPixmap rightPixmap;        // 右
    QPixmap rightBottomPixmap;  // 右下
    QPixmap bottomPixmap;       // 下
    QPixmap centerPixmap;       // 中

};

NinePatchPainterPrivate::NinePatchPainterPrivate(const QPixmap &background,
                                                 int left, int top, int right, int bottom,
                                                 bool horizontalStretch, bool verticalStretch)
    :left(left), top(top), right(right), bottom(bottom),
      horizontalStretch(horizontalStretch), verticalStretch(verticalStretch)
{
    // 把background分割为9个子图
    QRect pixmapRect(0, 0, background.width(), background.height());
    QList<QRect> rects = calculateNinePatchRects(pixmapRect, left, top, right, bottom);

    leftbottomPixmap    = background.copy(rects.at(0));
    leftPixmap          = background.copy(rects.at(1));
    leftTopPixmap       = background.copy(rects.at(2));
    topPixmap           = background.copy(rects.at(3));
    rightTopPixmap      = background.copy(rects.at(4));
    rightPixmap         = background.copy(rects.at(5));
    rightBottomPixmap   = background.copy(rects.at(6));
    bottomPixmap        = background.copy(rects.at(7));
    centerPixmap        = background.copy(rects.at(8));
}

// 用来计算切割九宫格图片与贴图的rect
QList<QRect> NinePatchPainterPrivate::calculateNinePatchRects(const QRect &rect, int left, int top, int right, int bottom) const
{
    int w = rect.width();
    int h = rect.height();
    int cw = rect.width() - left - right;   // 中间宽
    int ch = rect.height() - top - bottom;  // 中间高

    // 9宫格的rect
    QRect leftBottomRect(0, h-bottom, left, bottom);
    QRect leftRect(0, top, left, ch);
    QRect leftTopRect(0, 0, left, top);
    QRect topRect(left, 0, cw, top);
    QRect rightTopRect(w-right, 0, right, top);
    QRect rightRect(w-right, top, right, ch);
    QRect rightBottomRect(w-right, h-bottom, right, bottom);
    QRect bottomRect(left, h-bottom, cw, bottom);
    QRect centerRect(left, top, cw, ch);

    return QList<QRect>() << leftBottomRect << leftRect << leftTopRect
                          << topRect << rightTopRect << rightRect
                          << rightBottomRect << bottomRect << centerRect;
}
// 缩放图片
QPixmap NinePatchPainterPrivate::scalePixmap(const QPixmap &pixmap, const QSize &size) const
{
    // 缩放时忽略图片的高宽比，使用平滑缩放效果
    return pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
}

/*-----------------------------------------------------------------
 *                      NinePatchPainter
 *----------------------------------------------------------------*/
NinePatchPainter::NinePatchPainter(const QPixmap &background,
                                   int left, int top, int right, int bottom,
                                   bool horizontalStreth, bool verticalStreth)
    : d(new NinePatchPainterPrivate(background, left, top, right, bottom,
                                    horizontalStreth, verticalStreth))
{

}

NinePatchPainter::~NinePatchPainter()
{
    delete d;
}

// 绘制九宫格图片
void NinePatchPainter::paint(QPainter *painter, const QRect &rect, int left, int top, int right, int bottom) const
{
    QList<QRect> rects = d->calculateNinePatchRects(rect, left, top, right, bottom);

    QRect leftBottomRect  = rects.at(0);
    QRect leftRect        = rects.at(1);
    QRect leftTopRect     = rects.at(2);
    QRect topRect         = rects.at(3);
    QRect rightTopRect    = rects.at(4);
    QRect rightRect       = rects.at(5);
    QRect rightBottomRect = rects.at(6);
    QRect bottomRect      = rects.at(7);
    //QRect centerRect      = rects.at(8);

    // 绘制4个角
    painter->drawPixmap(leftTopRect,    d->scalePixmap(d->leftTopPixmap,    leftTopRect.size()));
    painter->drawPixmap(leftBottomRect, d->scalePixmap(d->leftbottomPixmap, leftBottomRect.size()));
    painter->drawPixmap(rightTopRect,   d->scalePixmap(d->rightTopPixmap,   rightTopRect.size()));
    painter->drawPixmap(rightBottomRect,d->scalePixmap(d->rightBottomPixmap,rightBottomRect.size()));

    painter->drawTiledPixmap(leftRect,   d->scalePixmap(d->leftPixmap,   leftRect.size()));
    painter->drawTiledPixmap(topRect,    d->scalePixmap(d->topPixmap,    topRect.size()));
    painter->drawTiledPixmap(rightRect,  d->scalePixmap(d->rightPixmap,  rightRect.size()));
    painter->drawTiledPixmap(bottomRect, d->scalePixmap(d->bottomPixmap, bottomRect.size()));
}
