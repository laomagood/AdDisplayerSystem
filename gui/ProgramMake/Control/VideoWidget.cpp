#include "VideoWidget.h"
#include <QDebug>
#include <QVBoxLayout>

VideoWidget::VideoWidget(QWidget *parent) : MVWidgetBase(parent)
{
    initializeUi();
    handleEvents();
}

void VideoWidget::initializeUi()
{
    this->setControlName(tr("视频"));
    this->setControlIconPath("./image/icon/programMakeWidget/video.png");
    this->setControlType("VideoWidget");
}

void VideoWidget::handleEvents()
{

}
