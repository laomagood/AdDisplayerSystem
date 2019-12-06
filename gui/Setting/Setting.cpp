#include "Setting.h"
#include "SettingSideBarWidget.h"
#include "UserManagement/UserManagementWidget.h"
#include "RoleManagement/RoleManagementWidget.h"
#include "SystemSettings/SystemSettingsWidget.h"
#include <QDebug>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QPushButton>

Setting::Setting(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
    this->m_pSettingSideBarWidget->initializeData();
}

void Setting::initializeUi()
{
    m_pSettingSideBarWidget = new SettingSideBarWidget;
    m_pStackedWidget = new QStackedWidget;

    auto pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pSettingSideBarWidget);
    pLayout->addWidget(m_pStackedWidget);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(pLayout);
}

void Setting::initializeData()
{

}

void Setting::handleEvents()
{
    connect(m_pSettingSideBarWidget, &SettingSideBarWidget::buttonClicked, [this]() {
        QPushButton *pButton = m_pSettingSideBarWidget->getClickedBtn();
        if (!m_nButtonWidgetHash.contains(pButton)) {
            createWidgetInContentStackedWidget(pButton);
        }

        m_pStackedWidget->setCurrentWidget(m_nButtonWidgetHash.value(pButton));
    });
}

void Setting::createWidgetInContentStackedWidget(QPushButton *button)
{
    if (button->objectName() == "btnUser") {
        // 用户管理
        QWidget *w = new UserManagementWidget;
        m_nButtonWidgetHash.insert(button, w);
        m_pStackedWidget->addWidget(w);
    } else if (button->objectName() == "btnRole") {
        // 角色管理
        QWidget *w = new RoleManagementWidget;
        m_nButtonWidgetHash.insert(button, w);
        m_pStackedWidget->addWidget(w);
    } else if (button->objectName() == "btnSys") {
        // 系统设置
        QWidget *w = new SystemSettingsWidget;
        m_nButtonWidgetHash.insert(button, w);
        m_pStackedWidget->addWidget(w);
    }
}
