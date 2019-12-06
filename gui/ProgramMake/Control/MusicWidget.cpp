#include "MusicWidget.h"
#include <QDebug>
#include <QVBoxLayout>

MusicWidget::MusicWidget(QWidget *parent) : MVWidgetBase(parent)
{
    initializeUi();
    handleEvents();
}

void MusicWidget::initializeUi()
{
    this->setControlName(tr("音频"));
    this->setControlIconPath("./image/icon/programMakeWidget/music.png");
    this->setControlType("MusicWidget");
}

void MusicWidget::handleEvents()
{

}
