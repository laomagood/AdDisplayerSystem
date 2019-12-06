#include "ImageViewerAttributes.h"
#include "ui_ImageViewerAttributes.h"

ImageViewerAttributes::ImageViewerAttributes(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageViewerAttributes)
{
    ui->setupUi(this);

    initializeUi();
    initializeData();
    handleEvents();
}

ImageViewerAttributes::~ImageViewerAttributes()
{
    delete ui;
}

void ImageViewerAttributes::setImageName(const QString &name)
{
    ui->imageNameLabel->setText(name);
}

void ImageViewerAttributes::setImageSize(int size)
{
    ui->imageSizeLabel->setText(QString::number(size));
}

void ImageViewerAttributes::setUploadTime(const QString &time)
{
    ui->uploadTimeLabel->setText(time);
}

void ImageViewerAttributes::setUploader(const QString &uploader)
{
    ui->uploaderLabel->setText(uploader);
}

void ImageViewerAttributes::setAuditTime(const QString &time)
{
    ui->AuditTimeLabel->setText(time);
}

void ImageViewerAttributes::setAuditor(const QString &auditor)
{
    ui->AuditorLabel->setText(auditor);
}

void ImageViewerAttributes::initializeUi()
{
    this->setFixedSize(132, 140);

    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(0, 0, 0, 160));
    this->setPalette(palette);
}

void ImageViewerAttributes::initializeData()
{

}

void ImageViewerAttributes::handleEvents()
{

}
