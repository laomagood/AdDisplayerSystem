#include "ClientSideBarWidget.h"
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QFile>


ClientSideBarWidget::ClientSideBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
    this->m_pBtnClientManage->click();
}

QPushButton *ClientSideBarWidget::getClickedBtn()
{
    return qobject_cast<QPushButton*> (m_pGroupButtons->checkedButton());
}

void ClientSideBarWidget::initializeData()
{
    this->m_pBtnClientManage->click();
}

void ClientSideBarWidget::initializeUi()
{
    // 填充背景颜色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50, 65, 90));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setFixedWidth(180);

    m_pBtnClientManage        = new QPushButton(tr("终端管理"));
    m_pBtnTransferManage        = new QPushButton(tr("传输管理"));
    m_pBtnClientManage->setObjectName("btnUser");
    m_pBtnTransferManage->setObjectName("btnRole");

    auto vBtnLayout = new QVBoxLayout;
    vBtnLayout->addWidget(m_pBtnClientManage);
    vBtnLayout->addWidget(m_pBtnTransferManage);
    vBtnLayout->addStretch();
    vBtnLayout->setContentsMargins(0, 0, 0, 0);
    vBtnLayout->setSpacing(0);
    this->setLayout(vBtnLayout);

    m_pGroupButtons = new QButtonGroup(this);
    m_pGroupButtons->addButton(m_pBtnClientManage);
    m_pGroupButtons->addButton(m_pBtnTransferManage);
    m_pGroupButtons->setExclusive(true);

    for (int i=0; i<m_pGroupButtons->buttons().size(); i++) {
        m_pGroupButtons->buttons().at(i)->setCheckable(true);
    }

    // 设置样式表
    QFile file("./resources/qss/SettingSideBarWidget.qss");
    file.open(QFile::ReadOnly);
    if (file.isOpen()) {
        QString styleSheet = this->styleSheet();
        styleSheet += QLatin1String(file.readAll());
        this->setStyleSheet(styleSheet);
    }

    this->m_pBtnClientManage->setChecked(true);
}

void ClientSideBarWidget::handleEvents()
{
    // 点击分组按钮，显示与隐藏相应的组员
    connect(this->m_pGroupButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {
        Q_UNUSED(button)
        //QPushButton *qButton = qobject_cast<QPushButton*> (button);
        emit buttonClicked();
    });
}
