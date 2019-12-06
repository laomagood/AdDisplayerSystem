#include "ControlWidget.h"
#include "../CanvasWidget.h"
#include "ControlBorderWidget.h"
#include <QDebug>
#include <QMouseEvent>
#include <QtGlobal>
#include <QMenu>


ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    m_magneticPointTL(QPoint(0, 0)),
    m_magneticPointBR(QPoint(0, 0)),
    m_isMagneticDragX(false),
    m_isMagneticDragY(false),
    m_isMagneticMoveX(false),
    m_isMagneticMoveY(false),
    m_isPressed(false)
{
    m_tIsMagnetic = false;
    m_bIsMagnetic = false;
    m_lIsMagnetic = false;
    m_rIsMagnetic = false;

    // 无边框
    setWindowFlags(Qt::FramelessWindowHint);
    this->setMouseTracking(true);
    this->setMinimumSize(40, 20);
    this->setBaseSize(150, 100);

    // 焦点策略
    this->setFocusPolicy(Qt::ClickFocus);

    // 边框
    this->resize(300, 200);
    qDebug() << "W H:" << this->width() << this->height();
    p_controlBorderWidget = new ControlBorderWidget("控件名称", this);
    p_controlBorderWidget->setGeometry(0, 0, width(), height());
}

QRect ControlWidget::getRect()
{
    return QRect(x(), y(), width(), height());
}

void ControlWidget::setControlName(QString name)
{
     m_nControlName = name;
     p_controlBorderWidget->setControlName(name);
}

void ControlWidget::raiseControlBorder()
{
    p_controlBorderWidget->raise();
}

void ControlWidget::setControlBorderBackgroundVisible(bool visible)
{
    p_controlBorderWidget->setBackgroundVisible(visible);
}

void ControlWidget::setControlBorderVisible(bool visible)
{
    p_controlBorderWidget->setVisible(visible);
}

void ControlWidget::resizeControlBorder()
{
    p_controlBorderWidget->setGeometry(0, 0, width(), height());
}

// 鼠标按下事件
void ControlWidget::mousePressEvent(QMouseEvent *event)
{
    this->setFocus();
    if(event->button() == Qt::LeftButton) {
        m_isPressed = true;
        m_rect = QRect(geometry().topLeft(), geometry().bottomRight());

        if(m_dir != NONE) {
            this->mouseGrabber();
        } else {
            m_dragPosition = event->pos();
            //            qDebug() << "鼠标按下 " << "x:" << m_dragPosition.x() << "y:" << m_dragPosition.y();
        }
    }
}

// 鼠标移动事件
void ControlWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isPressed) {
        resizeWidget(event);
        // 调整边框大小
        p_controlBorderWidget->setGeometry(0, 0, width(), height());
    } else {
        QPoint gloPoint = event->globalPos();
        mouseRegion(gloPoint);
    }

}

// 鼠标离开事件
void ControlWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        m_isPressed = false;
        if(m_dir != NONE) {
            this->releaseMouse();
            this->setCursor(QCursor(Qt::ArrowCursor));
        }
    }
}

// 获取焦点事件
void ControlWidget::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    p_controlBorderWidget->setBorderVisible(true);
    emit select(this);
}

// 失去焦点事件
void ControlWidget::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event)
    p_controlBorderWidget->setBorderVisible(false);
}

// 大小调整
void ControlWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    qDebug() << "resizeEvent";
    p_controlBorderWidget->setGeometry(0, 0, width(), height());
}

// 计算鼠标区域
void ControlWidget::mouseRegion(const QPoint &cursorGlobalPoint)
{
    // 获取窗体在屏幕上的位置区域，tl为topleft点，rb为rightbottom点
    QRect rect = this->rect();
    QPoint tl = mapToGlobal(rect.topLeft());
    QPoint rb = mapToGlobal(rect.bottomRight());

    int x = cursorGlobalPoint.x();
    int y = cursorGlobalPoint.y();

    if(tl.x() + PADDING >= x && tl.x() <= x && tl.y() + PADDING >= y && tl.y() <= y) {
        // 左上角
        m_dir = LEFTTOP;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));  // 设置鼠标形状
    } else if(x >= rb.x() - PADDING && x <= rb.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        // 右下角
        m_dir = RIGHTBOTTOM;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x() && y >= rb.y() - PADDING && y <= rb.y()) {
        //左下角
        m_dir = LEFTBOTTOM;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= rb.x() && x >= rb.x() - PADDING && y >= tl.y() && y <= tl.y() + PADDING) {
        // 右上角
        m_dir = RIGHTTOP;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } else if(x <= tl.x() + PADDING && x >= tl.x()) {
        // 左边
        m_dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } else if( x <= rb.x() && x >= rb.x() - PADDING) {
        // 右边
        m_dir = RIGHT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    }else if(y >= tl.y() && y <= tl.y() + PADDING){
        // 上边
        m_dir = TOP;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } else if(y <= rb.y() && y >= rb.y() - PADDING) {
        // 下边
        m_dir = BOTTOM;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }else {
        // 默认
        m_dir = NONE;
        this->setCursor(QCursor(Qt::ArrowCursor));
    }
}

// 调整窗口大小位置
void ControlWidget::resizeWidget(QMouseEvent *event)
{
    // 调整大小原理
    // [1] tl rb为鼠标按下前的窗口左上和右下坐标（相对于父窗口除去标题栏）
    // [2] 获取鼠标坐标point（相对于父窗口除去标题栏）
    // [3] 判断鼠标的区域位置，进入相应的case
    // [4] 通过point与tl，rb计算对QRect rMove 进行相应的更改
    // [5] setGeometry(rMove) 设置窗口的坐标和大小（相对于父窗口除去标题栏）

    // 鼠标坐标（相对于父窗口除去标题栏）
    QPoint point = event->pos() + this->pos();

    QRect rect = this->rect();
    QPoint tl = mapToParent(rect.topLeft());
    QPoint rb = mapToParent(rect.bottomRight());

    magneticCalculation(event);        // 磁吸计算

    // [1]  判断鼠标的区域
    // [2]
    // [3]  记录鼠标的坐标
    // [4]  被磁吸，判断鼠标偏量
    // [5]  鼠标偏量够大：自由移动，不够：不移动
    if(m_dir != NONE) {

        int offsetTLX = m_magneticPointTL.x() - point.x();    // 鼠标移动偏距左上x
        int offsetTLY = m_magneticPointTL.y() - point.y();    // 鼠标移动偏距左上y
        int offsetBRX = m_magneticPointBR.x() - point.x();    // 鼠标移动偏距右下x
        int offsetBRY = m_magneticPointBR.y() - point.y();    // 鼠标移动偏距右下y



        QRect rMove(tl, rb);
        switch(m_dir) {
        case LEFT:
            if(rb.x() - point.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else {
                // [1] 左边线磁吸
                if (m_lIsMagnetic) {
                    // [2] X方向锁定
                    if (m_isMagneticDragX) {
                        // [3] 脱离值
                        if (qAbs(offsetTLX) > MAGNETIC_RANGE+5) {
                            rMove.setX(point.x());
                            m_isMagneticDragX = false;
                        }
                    } else {
                        rMove.setX(m_magneticPointTL.x());
                        m_isMagneticDragX = true;
                    }
                } else {
                    rMove.setX(point.x());
                    m_isMagneticDragX = false;
                }
            }
            break;
        case RIGHT:
            if (m_rIsMagnetic) {
                if (m_isMagneticDragX) {
                    if (qAbs(offsetBRX) > MAGNETIC_RANGE+5) {
                        m_isMagneticDragX = false;
                        rMove.setWidth(point.x() - tl.x());
                    }
                } else {
                    rMove.setWidth(m_magneticPointBR.x() - tl.x());
                    m_isMagneticDragX = true;
                }
            } else {
                rMove.setWidth(point.x() - tl.x());
                m_isMagneticDragX = false;
            }
            break;
        case TOP:
            if(rb.y() - point.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else {
                if (m_tIsMagnetic) {
                    if (m_isMagneticDragY) {
                        if (qAbs(offsetTLY) > MAGNETIC_RANGE+5) {
                            rMove.setY(point.y());
                            m_isMagneticDragY = false;
                        }
                    } else {
                        rMove.setY(m_magneticPointTL.y());
                        m_isMagneticDragY = true;
                    }
                } else {
                    rMove.setY(point.y());
                    m_isMagneticDragY = false;
                }
            }
            break;
        case BOTTOM:
            if (m_bIsMagnetic) {
                if (m_isMagneticDragY) {
                    if (qAbs(offsetBRY) > MAGNETIC_RANGE+5) {
                        rMove.setHeight(point.y() - tl.y());
                        m_isMagneticDragY = false;
                    }
                } else {
                    rMove.setHeight(m_magneticPointBR.y() - tl.y());
                    m_isMagneticDragY = true;
                }
            } else {
                rMove.setHeight(point.y() - tl.y());
                m_isMagneticDragY = false;
            }
            break;
        case LEFTTOP:
            // [1] 左边线磁吸----------------------------------------
            if(rb.x() - point.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else {
                if (m_lIsMagnetic) {
                    // [2] X方向锁定
                    if (m_isMagneticDragX) {
                        // [3] 脱离值
                        if (qAbs(offsetTLX) > MAGNETIC_RANGE+5) {
                            rMove.setX(point.x());
                            m_isMagneticDragX = false;
                        }
                    } else {    // [2] X方向没有锁定
                        rMove.setX(m_magneticPointTL.x());
                        m_isMagneticDragX = true;
                    }
                } else {    // [1] 左边线没有磁吸
                    rMove.setX(point.x());
                    m_isMagneticDragX = false;
                }
            }
            // [1] 上边线磁吸----------------------------------------
            if(rb.y() - point.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else{
                if (m_tIsMagnetic) {
                    // [2] Y方向锁定
                    if (m_isMagneticDragY) {
                        // [3] 脱离值
                        if (qAbs(offsetTLY) > MAGNETIC_RANGE+5) {
                            rMove.setY(point.y());
                            m_isMagneticDragY = false;
                        }
                    } else {    // [2] Y方向没有锁定
                        rMove.setY(m_magneticPointTL.y());
                        m_isMagneticDragY = true;
                    }
                } else {    // [1] 上边线没有磁吸
                    rMove.setY(point.y());
                    m_isMagneticDragY = false;
                }
            }
            break;
        case RIGHTTOP:
            // [1] 右边线磁吸----------------------------------------
            if (m_rIsMagnetic) {
                // [2] X方向锁定
                if (m_isMagneticDragX) {
                    // [3] 脱离值
                    if (qAbs(offsetBRX) > MAGNETIC_RANGE+5) {
                        rMove.setWidth(point.x() - tl.x());
                        m_isMagneticDragX = false;
                    }
                } else {    // [2] X方向没有锁定
                    rMove.setWidth(m_magneticPointBR.x() - tl.x());
                    m_isMagneticDragX = true;
                }
            } else {    // [1] 右边线没有磁吸
                rMove.setWidth(point.x() - tl.x());
                m_isMagneticDragX = false;
            }
            // [1] 上边线磁吸----------------------------------------
            if(rb.y() - point.y() <= this->minimumHeight())
                rMove.setY(tl.y());
            else{
                if (m_tIsMagnetic) {
                    // [2] Y方向锁定
                    if (m_isMagneticDragY) {
                        // [3] 脱离值
                        if (qAbs(offsetTLY) > MAGNETIC_RANGE+5) {
                            rMove.setY(point.y());
                            m_isMagneticDragY = false;
                        }
                    } else {    // [2] Y方向没有锁定
                        rMove.setY(m_magneticPointTL.y());
                        m_isMagneticDragY = true;
                    }
                } else {    // [1] 上边线没有磁吸
                    rMove.setY(point.y());
                    m_isMagneticDragY = false;
                }
            }
            break;
        case LEFTBOTTOM:
            // [1] 左边线磁吸----------------------------------------
            if(rb.x() - point.x() <= this->minimumWidth())
                rMove.setX(tl.x());
            else {
                if (m_lIsMagnetic) {
                    // [2] X方向锁定
                    if (m_isMagneticDragX) {
                        // [3] 脱离值
                        if (qAbs(offsetTLX) > MAGNETIC_RANGE+5) {
                            rMove.setX(point.x());
                            m_isMagneticDragX = false;
                        }
                    } else {    // [2] X方向没有锁定
                        rMove.setX(m_magneticPointTL.x());
                        m_isMagneticDragX = true;
                    }
                } else {    // [1] 右边线没有磁吸
                    rMove.setX(point.x());
                    m_isMagneticDragX = false;
                }
            }
            // [1] 下边线磁吸----------------------------------------
            if (m_bIsMagnetic) {
                // [2] Y方向锁定
                if (m_isMagneticDragY) {
                    // [3] 脱离值
                    if (qAbs(offsetBRY) > MAGNETIC_RANGE+5) {
                        rMove.setHeight(point.y() - tl.y());
                        m_isMagneticDragY = false;
                    }
                } else {    // [2] Y方向没有锁定
                    rMove.setHeight(m_magneticPointBR.y() - tl.y());
                    m_isMagneticDragY = true;
                }
            } else {    // [1] 右边线没有磁吸
                rMove.setHeight(point.y() - tl.y());
                m_isMagneticDragY = false;
            }
            break;
        case RIGHTBOTTOM:
            // [1] 右边线磁吸----------------------------------------
            if (m_rIsMagnetic) {
                // [2] X方向锁定
                if (m_isMagneticDragX) {
                    // [3] 脱离值
                    if (qAbs(offsetBRX) > MAGNETIC_RANGE+5) {
                        rMove.setWidth(point.x() - tl.x());
                        m_isMagneticDragX = false;
                    }
                } else {    // [2] X方向没有锁定
                    rMove.setWidth(m_magneticPointBR.x() - tl.x());
                    m_isMagneticDragX = true;
                }
            } else {    // [1] 右边线没有磁吸
                rMove.setWidth(point.x() - tl.x());
                m_isMagneticDragX = false;
            }

            // [1] 下边线磁吸----------------------------------------

            if (m_bIsMagnetic) {
                // [2] Y方向锁定
                if (m_isMagneticDragY) {
                    // [3] 脱离值
                    if (qAbs(offsetBRY) > MAGNETIC_RANGE+5) {
                        rMove.setHeight(point.y() - tl.y());
                        m_isMagneticDragY = false;
                    }
                } else {    // [2] Y方向没有锁定
                    rMove.setHeight(m_magneticPointBR.y() - tl.y());
                    m_isMagneticDragY = true;
                }
            } else {    // [1] 下边线没有磁吸
                rMove.setHeight(point.y() - tl.y());
                m_isMagneticDragY = false;
            }
            break;
        default:
            break;
        }
        this->setGeometry(rMove);
    } else {
        // [1-1]  没有被磁吸，计算出左上磁吸坐标
        // [1-2]  移动到磁吸点setGeometry
        // [1-3]  记录鼠标的坐标
        // [2-1]  被磁吸，判断鼠标偏量
        // [2-2]  鼠标偏量够大：自由移动，不够：不移动

        // 获取一开始的矩形W,H的值
        QRect rect = this->rect();
        int width = rect.width();
        int height = rect.height();
        QPoint tl = mapToParent(rect.topLeft());
        int x = tl.x();
        int y = tl.y();
        int offsetX = event->pos().x() - m_dragPosition.x();    // 鼠标移动偏距x
        int offsetY = event->pos().y() - m_dragPosition.y();    // 鼠标移动偏距y

        // [1]---------------------------------------
        // X方向已经被锁定
        if (m_isMagneticMoveX) {
            // qDebug() << "X被锁定offsetX:" << offsetX;

            if (qAbs(offsetX) > MAGNETIC_RANGE+5) {
                m_isMagneticMoveX = false;
                x = offsetX + pos().x();
            } else {
                // qDebug() << "不能脱离x:" << x;
            }
        } else {
            if (m_lIsMagnetic) {
                x = m_magneticPointTL.x();
                m_isMagneticMoveX = true;
            } else if (m_rIsMagnetic) {
                x = m_magneticPointBR.x() - width;
                m_isMagneticMoveX = true;
            } else {
                x = offsetX + pos().x();
            }
        }

        // Y方向已经被锁定
        if (m_isMagneticMoveY) {
            // qDebug() << "Y被锁定offsetY:" << offsetY;
            if (qAbs(offsetY) > MAGNETIC_RANGE+5) {
                m_isMagneticMoveY = false;
                y = offsetY + pos().y();
            } else {
                // qDebug() << "不能脱离y:" << y;
            }
        } else {
            if (m_tIsMagnetic) {
                y = m_magneticPointTL.y();
                m_isMagneticMoveY = true;
            } else if (m_bIsMagnetic) {
                y = m_magneticPointBR.y() - height;
                m_isMagneticMoveY = true;
            } else {
                y = offsetY + pos().y();
            }
        }

        // [2]---------------------------------------
        // 设置移动坐标点
        QRect finalRect(x, y, width, height);
        this->setGeometry(finalRect);

        event->accept();
    }

    //    qDebug() << "T:" << m_tIsMagnetic << "  "
    //             << "B:" << m_bIsMagnetic << "  "
    //             << "L:" << m_lIsMagnetic << "  "
    //             << "R:" << m_rIsMagnetic;

    m_tIsMagnetic = false;
    m_bIsMagnetic = false;
    m_lIsMagnetic = false;
    m_rIsMagnetic = false;
}

// 磁吸计算
void ControlWidget::magneticCalculation(QMouseEvent *event)
{
    Q_UNUSED(event)
    CanvasWidget *m = qobject_cast<CanvasWidget *>(this->parentWidget());
    if (m != nullptr) {
        QList<QRect> rectList = m->getListRect();   // 获取Rect列表
        QRect thisRect = this->getRect();
        rectList.removeOne(thisRect);        // 去除自身

        // [1] 计算自身矩形角点是否在其他矩形的磁力范围
        for (int i=0; i<rectList.size(); i++) {
            QRect otherRect = rectList.at(i);
            // 计算磁吸点坐标
            magneticLine(otherRect, thisRect);
        }

    }
}

// 计算磁吸点坐标
void ControlWidget::magneticLine(QRect otherRect, QRect thisRect)
{
    // 矩形角点在磁吸范围
    if (isInMagneticRange(otherRect, thisRect) || isInMagneticRange(thisRect, otherRect)) {

        // 两个矩形的各个边的边距（绝对值）
        int LL = qAbs(thisRect.left() - otherRect.left());
        int LR = qAbs(thisRect.left() - otherRect.right());

        int RL = qAbs(thisRect.right() - otherRect.left());
        int RR = qAbs(thisRect.right() - otherRect.right());

        int TT = qAbs(thisRect.top() - otherRect.top());
        int TB = qAbs(thisRect.top() - otherRect.bottom());

        int BT = qAbs(thisRect.bottom() - otherRect.top());
        int BB = qAbs(thisRect.bottom() - otherRect.bottom());

        // 没有与其他矩形左吸附or右吸附
        if (!m_lIsMagnetic || !m_rIsMagnetic) {
            // 磁吸：左边线
            if (LL <= MAGNETIC_RANGE*2) {
                m_lIsMagnetic = true;
                m_magneticPointTL.setX(otherRect.left());
            } else if (LR <= MAGNETIC_RANGE*2){
                m_lIsMagnetic = true;
                m_magneticPointTL.setX(otherRect.right()+1);
            }
            // 磁吸：右边线
            if (RL <= MAGNETIC_RANGE*2) {
                m_rIsMagnetic = true;
                m_magneticPointBR.setX(otherRect.left());
            } else if (RR <= MAGNETIC_RANGE*2){
                m_rIsMagnetic = true;
                m_magneticPointBR.setX(otherRect.right()+1);
            }
        }

        // 没有与其他矩形上吸附or下吸附
        if (!m_tIsMagnetic || !m_bIsMagnetic) {
            // 磁吸：上边线
            if (TT <= MAGNETIC_RANGE*2) {
                m_tIsMagnetic = true;
                m_magneticPointTL.setY(otherRect.top());
            } else if (TB <= MAGNETIC_RANGE*2){
                m_tIsMagnetic = true;
                m_magneticPointTL.setY(otherRect.bottom()+1);
            }
            // 磁吸：下边线
            if (BT <= MAGNETIC_RANGE*2) {
                m_bIsMagnetic = true;
                m_magneticPointBR.setY(otherRect.top());
            } else if (BB <= MAGNETIC_RANGE*2){
                m_bIsMagnetic = true;
                m_magneticPointBR.setY(otherRect.bottom()+1);
            }
        }

        /*
        // 同时靠近左右
        if (m_lIsMagnetic && m_rIsMagnetic) {
            LL < RR ? m_rIsMagnetic = false : m_lIsMagnetic = false;
        }
        // 同时靠近上下
        if (m_tIsMagnetic && m_bIsMagnetic) {
            TT < BB ? m_bIsMagnetic = false : m_tIsMagnetic = false;
        }
        */
    }
}

// 矩形角点是否在磁吸范围
bool ControlWidget::isInMagneticRange(QRect otherRect, QRect thisRect)
{
    // 四个角点
    QList<QPoint> listPoint;
    listPoint.append(thisRect.topLeft());
    listPoint.append(thisRect.topRight());
    listPoint.append(thisRect.bottomLeft());
    listPoint.append(thisRect.bottomRight());

    // 内外范围矩形
    QPoint point(MAGNETIC_RANGE, MAGNETIC_RANGE);
    QRect outRect(otherRect.topLeft() - point, otherRect.bottomRight() + point);
    QRect inRect (otherRect.topLeft() + point, otherRect.bottomRight() - point);

    // 判断角点在不在范围内
    for (int i=0; i<listPoint.size(); i++) {
        if (outRect.contains(listPoint.at(i)) && !inRect.contains(listPoint.at(i))) {
            return true;
        }
    }
    return false;
}
