#include "ImageWidget.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include <QButtonGroup>
#include <QJsonObject>
#include <QJsonArray>
#include "util/Json.h"

ImageWidget::ImageWidget(QWidget *parent) :
    ControlWidget(parent),
    m_imageChangeTime(3000),
    m_imageChangeType(LeftToRight),
    m_currentDrawImageIndx(0)
{
    this->setControlName(tr("图片"));
    this->setControlIconPath("./image/icon/programMakeWidget/image.png");
    this->setControlType("ImageWidget");

//    this->resize(300, 200);
    this->setWindowFlags(Qt::FramelessWindowHint);

    this->setProperty("ImageOpacity", 1.0);
    m_opacityAnimation = new QPropertyAnimation(this, "ImageOpacity");
    m_opacityAnimation->setDuration(750);
    m_opacityAnimation->setStartValue(1.0);
    m_opacityAnimation->setEndValue(0.0);

    this->setProperty("ImagePosition", 0);
    m_positionAnimation = new QPropertyAnimation(this, "ImagePosition");
    m_positionAnimation->setDuration(750);
    m_positionAnimation->setStartValue(0);
    m_positionAnimation->setEndValue(this->width());
    m_positionAnimation->setEasingCurve(QEasingCurve::OutCubic);

    initializeData();
    handleEvents();
}

ImageWidget::~ImageWidget()
{

}

void ImageWidget::addList(const QStringList &fileNameList)
{
    m_imageFileNameList = m_imageFileNameList+fileNameList;
    initializeData();
}

bool ImageWidget::remove(int index)
{
    if (m_imageFileNameList.size() >= index) {
        m_imageFileNameList.removeAt(index);
        initializeData();
        return true;
    }
    return false;
}

void ImageWidget::clearList()
{
    m_imageFileNameList.clear();
    initializeData();
}

void ImageWidget::moveLayerUP(int index)
{
    if ((m_imageFileNameList.size() > 0) && (m_imageFileNameList.size() >= index)) {
        if (index > 0) {
            m_imageFileNameList.swap(index, index-1);
        }
    }
    initializeData();
}

void ImageWidget::moveLayerDown(int index)
{
    if ((m_imageFileNameList.size() > 0) && (m_imageFileNameList.size() >= index)) {
        if (index < m_imageFileNameList.size()-1) {
            m_imageFileNameList.swap(index, index+1);
        }
    }
    initializeData();
}

void ImageWidget::setImageChangeTime(int imageChangeTime)
{
    if (imageChangeTime < 1) {
        return;
    }
    m_imageChangeTime = imageChangeTime*1000;
    initializeData();
}

void ImageWidget::setImageChangeType(ImageWidget::ImageChangeType imageChangeType)
{
    m_imageChangeType = imageChangeType;
}

QStringList ImageWidget::getImageList() const
{
    return m_imageFileNameList;
}

int ImageWidget::getImageChangeTime() const
{
    return m_imageChangeTime/1000;
}

ImageWidget::ImageChangeType ImageWidget::getImageChangeType() const
{
    return m_imageChangeType;
}

QJsonDocument ImageWidget::toJson()
{
    // 图片列表
    QJsonObject imageList;
    if (!m_imageFileNameList.isEmpty()) {
        for(int i=0; i<m_imageFileNameList.size(); i++) {
            imageList.insert(QString::number(i), m_imageFileNameList.at(i));
        }
    }

    QJsonObject json;
    json.insert("Name", getControlName());
    json.insert("ChangeType", (int)m_imageChangeType);
    json.insert("ChangeTime", m_imageChangeTime);
    json.insert("ListSize", m_imageFileNameList.size());
    json.insert("List", QJsonObject(imageList));

    // 构建 Json 文档
    QJsonDocument document;
    document.setObject(json);
    return document;
}

bool ImageWidget::toBean(const QJsonDocument &doc)
{
    QByteArray byteArray = doc.toJson(QJsonDocument::Compact);
    QString strJson(byteArray);
    Json nJson(strJson, false);

    this->setControlName(nJson.getString("Name"));
    this->m_imageChangeType = (ImageChangeType)nJson.getInt("ChangeType");
    this->m_imageChangeTime = nJson.getInt("ChangeTime");

    int size = nJson.getInt("ListSize");
    for (int i=0; i<size; i++) {
        QString strImage = "List." + QString::number(i);
        m_imageFileNameList.append(nJson.getString(strImage));
    }
    initializeData();
}

void ImageWidget::onImageChangeTimeout()
{
    if (m_imageFileNameList.isEmpty()) {
        return ;
    }

    // 设置前后的图片;
    m_currentPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));
    m_currentDrawImageIndx++;
    if (m_currentDrawImageIndx >= m_imageFileNameList.count()) {
        m_currentDrawImageIndx = 0;
    }
    m_nextPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));

    // 动画类重新开始;
    switch (m_imageChangeType) {
    case LeftToRight:
    case RightToLeft:
        m_positionAnimation->start();
        break;
    case Opacity:
        m_opacityAnimation->start();
        break;
    default:
        break;
    }
}

void ImageWidget::initializeData()
{
    if (m_imageFileNameList.isEmpty()){
        this->setControlBorderBackgroundVisible(true);
        return ;
    } else {
        this->setControlBorderBackgroundVisible(false);
    }

    m_currentDrawImageIndx = 0;
    m_currentPixmap = QPixmap(m_imageFileNameList.at(m_currentDrawImageIndx));
    m_imageChangeTimer.start(m_imageChangeTime);
    update();
}

void ImageWidget::handleEvents()
{
    connect(m_opacityAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    connect(m_positionAnimation, SIGNAL(valueChanged(const QVariant&)), this, SLOT(update()));
    connect(&m_imageChangeTimer, SIGNAL(timeout()), this, SLOT(onImageChangeTimeout()));
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    QRect imageRect = this->rect();

    // 图片列表为空，显示默认图片;
    if (m_imageFileNameList.isEmpty()) {
        QPixmap backPixmap = QPixmap("");
        painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size()));
    }
    // 只有一张图片;
    else if (m_imageFileNameList.count() == 1) {
        QPixmap backPixmap = QPixmap(m_imageFileNameList.first());
        painter.drawPixmap(imageRect, backPixmap.scaled(imageRect.size()));
    }
    // 多张图片;
    else if (m_imageFileNameList.count() > 1) {
        float imageOpacity = this->property("ImageOpacity").toFloat();
        int imagePosition = this->property("ImagePosition").toInt();

        switch (m_imageChangeType) {
        case LeftToRight:
            painter.drawPixmap(-imagePosition, 0, width(), height(), m_currentPixmap.scaled(imageRect.size()));
            painter.drawPixmap(width()-imagePosition, 0, width(), height(),  m_nextPixmap.scaled(imageRect.size()));
            break;
        case RightToLeft:
            painter.drawPixmap(imagePosition, 0, width(), height(),  m_currentPixmap.scaled(imageRect.size()));
            painter.drawPixmap(-width()+imagePosition, 0, width(), height(),  m_nextPixmap.scaled(imageRect.size()));
            break;
        case Opacity:
            painter.setOpacity(1);
            painter.drawPixmap(imageRect, m_nextPixmap.scaled(imageRect.size()));
            painter.setOpacity(imageOpacity);
            painter.drawPixmap(imageRect, m_currentPixmap.scaled(imageRect.size()));
            break;
        default:
            break;
        }
    }
}

void ImageWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_positionAnimation->setEndValue(this->width());
    update();
    this->resizeControlBorder();
}
