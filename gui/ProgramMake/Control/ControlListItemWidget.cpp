#include "ControlListItemWidget.h"
#include <qDebug>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

ControlListItemWidget::ControlListItemWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
}

void ControlListItemWidget::setControlWidget(ControlWidget *controlWidget)
{
    m_pControlWidget = controlWidget;
    if (m_pControlWidget != nullptr) {
        initializeData();
    }
}

void ControlListItemWidget::initializeUi()
{
    m_pControlIcon = new QLabel(tr("Icon"));
    m_pControlName = new QLabel(tr("Name"));
    auto *pDuration = new QLabel("00:00:00");

    auto *pVLayout = new QVBoxLayout;
    pVLayout->addWidget(m_pControlName);
    pVLayout->addWidget(pDuration);
    pVLayout->setSpacing(1);
    pVLayout->setContentsMargins(0, 0, 0, 0);

    auto *pHLayout = new QHBoxLayout;
    pHLayout->addWidget(m_pControlIcon);
    pHLayout->addSpacing(15);
    pHLayout->addLayout(pVLayout);
    pHLayout->addStretch();

    this->setLayout(pHLayout);
    this->layout()->setContentsMargins(2, 2, 2, 2);
}

void ControlListItemWidget::initializeData()
{
    // 设置名称
    QFont font("Microsoft YaHei", 9, QFont::Bold);
    m_pControlName->setFont(font);
    m_pControlName->setText(m_pControlWidget->getControlName());

    // 设置图标
    QPixmap iconImage;
    iconImage.load(m_pControlWidget->getControlIconPath());
    m_pControlIcon->setPixmap(iconImage.scaled(30, 30));
    qDebug() << "control list item";
}

void ControlListItemWidget::handleEvents()
{

}
