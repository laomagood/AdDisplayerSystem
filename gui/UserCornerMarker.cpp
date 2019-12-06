#include "UserCornerMarker.h"
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFile>
#include <QAction>
#include <QMenu>

UserCornerMarker::UserCornerMarker(QWidget *parent) : QWidget(parent)
{
    initializeUi();
//    initializeData();
    handleEvents();
}

void UserCornerMarker::setUserBean(const UserBean &userBean)
{
    m_userBean = userBean;
    initializeData();
}

void UserCornerMarker::initializeUi()
{
    m_pUserIcon = new QLabel;
    m_pUserName = new QLabel;
    m_pDropDownBtn = new QPushButton(tr("操作"));

    m_pUserIcon->setPixmap(QPixmap("./image/User/User.png").scaled(25, 25));
    m_pUserName->setText("管理员");
    m_pDropDownBtn->setCheckable(true);

    // 字体颜色
    m_pUserName->setFont(QFont("微软雅黑", 10));
    m_pUserName->setStyleSheet("color:rgb(255,255,255);");

    // 按钮图片
    m_pDropDownBtn->setStyleSheet("background-image: url(:image/User/Instructions.png);");

    auto pLayout = new QHBoxLayout;
    pLayout->addWidget(m_pUserIcon);
    pLayout->addWidget(m_pUserName);
    pLayout->addWidget(m_pDropDownBtn);
    this->setLayout(pLayout);

    setupActions();
}

void UserCornerMarker::initializeData()
{
    m_pUserName->setText(m_userBean.getName());
}

void UserCornerMarker::handleEvents()
{

}

void UserCornerMarker::setupActions()
{
    // 系统托盘
    m_changeUser = new QAction(tr("切换用户"), this);
    connect(m_changeUser, &QAction::triggered, [this] {  });
    m_changePassword = new QAction(tr("更改密码"), this);
    connect(m_changePassword, &QAction::triggered, [this] {  });

    auto pMenu = new QMenu;
    pMenu->addAction(m_changeUser);
    pMenu->addAction(m_changePassword);

    m_pDropDownBtn->setMenu(pMenu);
}
