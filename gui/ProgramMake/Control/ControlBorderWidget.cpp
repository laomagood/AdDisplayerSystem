#include "ControlBorderWidget.h"
#include <QPainter>
#include <QDebug>

ControlBorderWidget::ControlBorderWidget(const QString &controlName, QWidget *parent) :
    QWidget(parent),
    m_controlName(controlName),
    m_backgroundVisible(true),
    m_borderVisible(false)

{
    // 直接忽略鼠标事件
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    // 无边框
    setWindowFlags(Qt::FramelessWindowHint);
}

void ControlBorderWidget::setBackgroundVisible(bool visible)
{
    m_backgroundVisible = visible;
    update();
}

void ControlBorderWidget::setBorderVisible(bool visible)
{
    m_borderVisible = visible;
    update();
}

void ControlBorderWidget::setControlName(const QString &controlName)
{
    m_controlName = controlName;
    update();
}

void ControlBorderWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // 背景颜色（用于有无素材）
    QPainter painter(this);
    if (m_backgroundVisible) {
        qDebug() << "绘制背景";
        painter.setBrush(QColor(200, 240, 255));
        painter.drawRect(QRect(-1, -1, width()+1, height()+1));
    }

    // （用于失获焦点）
    if (m_borderVisible) {
        // 绘制边线
        painter.setBrush(QColor(0, 0, 0, 0));
        painter.setPen(QPen(QColor(95, 150, 255), 1));
        painter.drawRect(QRect(0, 0, width()-1, height()-1));

        // 绘制四角
        QPen pen(QColor(255, 10, 10), 3);
        pen.setJoinStyle(Qt::MiterJoin);
        painter.setPen(pen);
        QPolygon polygon;

        int x, y;
        x = 1;
        y = 1;
        polygon.setPoints(3, x,y+10, x,y, x+10,y);
        painter.drawPolyline(polygon);
        x = width() - 2;
        y = 1;
        polygon.setPoints(3, x-10,y, x,y, x,y+10);
        painter.drawPolyline(polygon);
        x = width() - 2;
        y = height() - 2;
        polygon.setPoints(3, x,y-10, x,y, x-10,y);
        painter.drawPolyline(polygon);
        x = 1;
        y = height() - 2;
        polygon.setPoints(3, x+10,y, x,y, x,y-10);
        painter.drawPolyline(polygon);
    }

    // 说明文字（用于有无素材）
    if (m_backgroundVisible) {
        QFont font(m_controlName, 10);
        painter.setFont(font);

        QFontMetrics fontMetric(font);
        int textWidth = fontMetric.width(m_controlName);
        QPoint textPoint(width()/2-textWidth/2, height()/2+7);
        painter.setPen(QColor(0, 120, 255));
        painter.drawText(textPoint, m_controlName);
    }
}

void ControlBorderWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    update();
}
