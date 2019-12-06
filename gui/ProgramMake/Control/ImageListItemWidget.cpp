#include "ImageListItemWidget.h"
#include "ImageWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QFileInfo>

ImageListItemWidget::ImageListItemWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
}

void ImageListItemWidget::setImageFilePath(QString imageFilePath)
{
    m_ImageFilePath = imageFilePath;
    initializeData();
}

void ImageListItemWidget::initializeUi()
{
    m_pImageIcon = new QLabel;
    m_pImageName = new QLabel;

    auto *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pImageIcon);
    pHLayout->addWidget(m_pImageName);
    pHLayout->setSpacing(2);

    this->setLayout(pHLayout);
//    this->layout()->setContentsMargins(2, 2, 2, 2);
}

void ImageListItemWidget::initializeData()
{
    m_pImageIcon->setPixmap(QPixmap(m_ImageFilePath).scaled(45, 45, Qt::KeepAspectRatio));
    m_pImageName->setText(QFileInfo(m_ImageFilePath).fileName());
}

void ImageListItemWidget::handleEvents()
{

}
