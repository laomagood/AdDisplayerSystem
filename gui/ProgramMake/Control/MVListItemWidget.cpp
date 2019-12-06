#include "MVListItemWidget.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QFileInfo>

MVListItemWidget::MVListItemWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

void MVListItemWidget::setFilePath(QString filePath)
{
    m_nFilePath = filePath;
    initializeData();
}

void MVListItemWidget::initializeUi()
{
    m_pMVName = new QLabel;
    m_pMVDuration = new QLabel;

    auto *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pMVName);
    pHLayout->addWidget(m_pMVDuration);
    pHLayout->setSpacing(2);

    this->setLayout(pHLayout);
}

void MVListItemWidget::initializeData()
{
    m_pMVName->setText(QFileInfo(m_nFilePath).fileName());
    m_pMVDuration->setText(tr("时长:5s"));
}

void MVListItemWidget::handleEvents()
{

}
