#include "SettingSideBarWidget.h"
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QFile>


SettingSideBarWidget::SettingSideBarWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
    this->m_pBtnUserManage->click();
}

QPushButton *SettingSideBarWidget::getClickedBtn()
{
    return qobject_cast<QPushButton*> (m_pGroupButtons->checkedButton());
}

void SettingSideBarWidget::initializeData()
{
    this->m_pBtnUserManage->click();
}

void SettingSideBarWidget::initializeUi()
{
    // 填充背景颜色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(50, 65, 90));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->setFixedWidth(180);

    m_pBtnUserManage        = new QPushButton(tr("用户管理"));
    m_pBtnRoleManage        = new QPushButton(tr("角色管理"));
    m_pBtnSysSetting        = new QPushButton(tr("系统设置"));
    m_pBtnUserManage->setObjectName("btnUser");
    m_pBtnRoleManage->setObjectName("btnRole");
    m_pBtnSysSetting->setObjectName("btnSys");

    auto vBtnLayout = new QVBoxLayout;
    vBtnLayout->addWidget(m_pBtnUserManage);
    vBtnLayout->addWidget(m_pBtnRoleManage);
    vBtnLayout->addWidget(m_pBtnSysSetting);
    vBtnLayout->addStretch();
    vBtnLayout->setContentsMargins(0, 0, 0, 0);
    vBtnLayout->setSpacing(0);
    this->setLayout(vBtnLayout);

    m_pGroupButtons = new QButtonGroup(this);
    m_pGroupButtons->addButton(m_pBtnUserManage);
    m_pGroupButtons->addButton(m_pBtnRoleManage);
    m_pGroupButtons->addButton(m_pBtnSysSetting);
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

    this->m_pBtnUserManage->setChecked(true);
}

void SettingSideBarWidget::handleEvents()
{
    // 点击分组按钮，显示与隐藏相应的组员
    connect(this->m_pGroupButtons, QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            [this](QAbstractButton *button) {
        Q_UNUSED(button)
        //QPushButton *qButton = qobject_cast<QPushButton*> (button);
        emit buttonClicked();
    });
}
