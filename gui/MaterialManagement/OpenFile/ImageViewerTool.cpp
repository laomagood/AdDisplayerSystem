#include "ImageViewerTool.h"
#include "ui_ImageViewerTool.h"
#include "ImageViewerAttributes.h"
#include <QDebug>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>
#include <QImageReader>
#include <QApplication>
#include <QFontMetricsF>
#include <QScreen>

ImageViewerTool::ImageViewerTool(QWidget *parent) : QWidget(parent),
    ui(new Ui::ImageViewerTool),
    m_zoomValue(10),
    m_zoomState(NO_ACTION),
    m_bPressed(false),
    m_offsetX(0),
    m_offsetY(0)
{
    ui->setupUi(this);
    initializeUi();
    handleEvents();

//    m_image = this->loadImage("D:/QtStudy/AdDisplayerSystem/Code/Test/build-test08-ImageView-Desktop_Qt_5_10_1_MinGW_32bit-Debug/bin/timg.jpg");
    initializeData();
}

ImageViewerTool::~ImageViewerTool()
{
    delete ui;
}

QImage ImageViewerTool::loadImage(const QString &fileName)
{
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    qDebug() << "loading" << QDir::toNativeSeparators(fileName);
    if (!reader.canRead()) {
        qDebug() << QDir::toNativeSeparators(fileName) << ": can't load image";
        return QImage();
    }

    QImage image;
    if (!reader.read(&image)) {
        qDebug() << QDir::toNativeSeparators(fileName) << ": corrupted image: " << reader.errorString();
        // 调整窗口大小
        resizeWidget();
        return QImage();
    }
    m_image = image;
    initializeData();
    return image;
}

void ImageViewerTool::zoomInImage()
{
    if (m_zoomValue >= 30)
        return;
    if (m_zoomValue >=10) {
        m_zoomValue += 2;
    } else {
        m_zoomValue += 1;
    }
    m_zoomState = ZOOM_IN;
    adjustImageRect();
}

void ImageViewerTool::zoomOutImage()
{
    if (m_zoomValue <= 1)
        return;
    if (m_zoomValue >10) {
        m_zoomValue -= 2;
    } else {
        m_zoomValue -= 1;
    }
    m_zoomState = ZOOM_OUT;
    adjustImageRect();
}

void ImageViewerTool::presetImage()
{
    m_zoomValue = 10;
    m_offsetX = 0;
    m_offsetY = 0;
    this->update();
}

void ImageViewerTool::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // 背景透明
    painter.setBrush(QColor(0, 0, 0, 200));
    painter.drawRect(0, 0, this->width(), this->height());

    // 绘制图片
    if (m_image.isNull())
        return QWidget::paintEvent(event);
    painter.drawImage(m_imageRect, m_image);
}

void ImageViewerTool::wheelEvent(QWheelEvent *event)
{
    m_mousePos = event->pos();
    if (event->delta() > 0)
        zoomInImage();
    else
        zoomOutImage();
    update();
}

void ImageViewerTool::mousePressEvent(QMouseEvent *event)
{
    m_pressedPos = event->pos();
    m_bPressed = true;
}

void ImageViewerTool::mouseMoveEvent(QMouseEvent *event)
{
    // 上一张，下一张
    if ( event->pos().x() <= 60 ) {
//        ui->previousButton->show();
        ui->previousButton->hide();
    } else {
        ui->previousButton->hide();
    }
    if ( event->pos().x() >= (this->width() - 60) ) {
//        ui->nextButton->show();
        ui->nextButton->hide();
    } else {
        ui->nextButton->hide();
    }
    // 工具栏
    if ( event->pos().y() >= this->height()/4*3 ) {
        ui->toolFrame->show();
    } else {
        ui->toolFrame->hide();
    }

    if (!m_bPressed)
        return QWidget::mouseMoveEvent(event);

    QPoint pos = event->pos();
    m_offsetX = pos.x() - m_pressedPos.x();
    m_offsetY = pos.y() - m_pressedPos.y();

    m_pressedPos = pos;
    adjustImageRect();
}

void ImageViewerTool::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_bPressed = false;
}

void ImageViewerTool::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_imageRect.moveCenter(QPoint(width()/2, height()/2));
    int attributeW = m_pImageViewerAttributes->width();
    int attributeH = m_pImageViewerAttributes->height();
    m_pImageViewerAttributes->move(this->width()-attributeW, this->height()-attributeH);
}

void ImageViewerTool::initializeUi()
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setMinimumSize(720, 540);
    this->setMouseTracking(true);

    ui->previousButton->setToolTip(tr("上一张"));
    ui->nextButton->setToolTip(tr("下一张"));
    ui->previousButton->hide();
    ui->nextButton->hide();
    ui->saveButton->setToolTip(tr("保存"));
    ui->fullScreenButton->setToolTip(tr("全屏"));
    ui->zoomInButton->setToolTip(tr("放大"));
    ui->zoomOutButton->setToolTip(tr("缩小"));
    ui->zoom1to1Button->setToolTip(tr("调整到1:1"));
    ui->rightHandedButton->setToolTip(tr("向右旋转"));
    ui->informationButton->setToolTip(tr("显示图片信息"));
    ui->zoom1to1Button->setEnabled(false);

    ui->toolFrame->setFixedWidth(395);
    ui->informationButton->setCheckable(true);
    ui->informationButton->setChecked(false);

    // 属性小窗口
    m_pImageViewerAttributes = new ImageViewerAttributes(this);
    int attributeW = m_pImageViewerAttributes->width();
    int attributeH = m_pImageViewerAttributes->height();
    m_pImageViewerAttributes->move(this->width()-attributeW, this->height()-attributeH);
    m_pImageViewerAttributes->hide();
}

void ImageViewerTool::initializeData()
{
    m_imageRectOriginal = QRect(0, 0, m_image.width(), m_image.height());
    m_imageRect = m_imageRectOriginal;
    m_imageRect.moveCenter(QPoint(width()/2, height()/2));
    update();
}

void ImageViewerTool::handleEvents()
{
    connect(ui->zoomInButton, &QPushButton::clicked, [this] {
        zoomInImage();
    });
    connect(ui->zoomOutButton, &QPushButton::clicked, [this] {
        zoomOutImage();
    });
    connect(ui->fullScreenButton, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            if (this->parentWidget() != nullptr) {
                this->parentWidget()->showMaximized();
            }
        } else {
            if (this->parentWidget() != nullptr) {
                this->parentWidget()->showNormal();
            }
        }
    });

    connect(ui->zoom1to1Button, &QPushButton::clicked, [this] {
        if (m_zoomValue > 10) {
            m_zoomValue = 10;
            m_zoomState = ZOOM_IN;
            adjustImageRect();
        } else {
            m_zoomValue = 10;
            m_zoomState = ZOOM_OUT;
            adjustImageRect();
        }
    });

    connect(ui->informationButton, &QPushButton::toggled, [this](bool checked) {
        if (checked) {
            m_pImageViewerAttributes->show();
        } else {
            m_pImageViewerAttributes->hide();
        }
    });
}

void ImageViewerTool::adjustImageRect()
{
    // 宽高是否比窗口大
    bool bWidth = m_imageRect.width() >= width() ? true : false;
    bool bHeight = m_imageRect.height() >= height() ? true : false;

    // [1] 缩放
    if (m_zoomState != NO_ACTION) {
        QString strValue;
        strValue = QString::number(m_zoomValue*10) + "%";
        ui->zoomValueLabel->setText(strValue);
        if (m_zoomValue == 10) {
            ui->zoom1to1Button->setEnabled(false);
        } else {
            ui->zoom1to1Button->setEnabled(true);
        }
//        qDebug() << "ZoomValue" << m_zoomValue;

        m_imageRect.setWidth(m_imageRectOriginal.width()*m_zoomValue/10);
        m_imageRect.setHeight(m_imageRectOriginal.height()*m_zoomValue/10);

        m_imageRect.moveCenter(QPoint(width()/2, height()/2));
        m_zoomState = NO_ACTION;
        this->update();
        return;
    }

    QPoint moveValue = boundaryDetection();
    if (bWidth && !bHeight) {               // 左右移动
        m_imageRect.translate(moveValue.x(), 0);
    } else if (!bWidth && bHeight) {        // 上下移动
        m_imageRect.translate(0, moveValue.y());
    } else if (bWidth && bHeight) {         // 随便移动
        m_imageRect.translate(moveValue.x(), moveValue.y());
    } else {
//        qDebug() << "窗口pos  " << this->pos().x() << this->pos().y();
//        qDebug() << "鼠标移动量" <<  m_offsetX << m_offsetY;
        //        this->move(this->pos().x()+m_offsetX, this->pos().y()+m_offsetY);
    }

    this->update();
}

QPoint ImageViewerTool::boundaryDetection()
{
    QPoint pointReturn(0, 0);
    // 在窗口内为负数，在窗口外为正数
    QRect rect;
    rect.setTop(- m_imageRect.top());
    rect.setLeft(- m_imageRect.left());
    rect.setBottom(m_imageRect.bottom() - this->height());
    rect.setRight(m_imageRect.right()  - this->width());

    // 左右移动
    if (m_offsetX >0) { // →
        if (m_offsetX < rect.left())
            pointReturn.setX(m_offsetX);
        if (m_offsetX > rect.left())
            pointReturn.setX(rect.left());
    } else { // ←
        if (qAbs(m_offsetX) < rect.right())
            pointReturn.setX(m_offsetX);
        if (qAbs(m_offsetX) > rect.right())
            pointReturn.setX(-rect.right());
    }
    // 上下移动
    if (m_offsetY >0) { // ↓
        if (m_offsetY < rect.top())
            pointReturn.setY(m_offsetY);
        if (m_offsetY > rect.top())
            pointReturn.setY(rect.top());
    } else { // ↑
        if (qAbs(m_offsetY) < rect.bottom())
            pointReturn.setY(m_offsetY);
        if (qAbs(m_offsetY) > rect.bottom())
            pointReturn.setY(-rect.bottom());
    }
    return pointReturn;
}

void ImageViewerTool::resizeWidget()
{
    // 获取分辨率(不算任务栏)
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect mm = screen->availableGeometry() ;
    int screenW = mm.width();
    int screenH = mm.height();

    // 获取图片尺寸
    int imageW = m_image.size().width();
    int imageH = m_image.size().height();

    int widgetW = 540;
    int widgetH = 720;

    // 调整一下窗口大小
    if (imageW > screenW)
        widgetW = screenW - 50;

    if (imageW <= 720)
        widgetW = 720;

    if (imageH > screenH)
        widgetH = screenH - 50;

    if (imageH <= 540)
        widgetH = 540;

    resize(widgetW, widgetH);
}
