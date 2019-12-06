#include "NewEditUserWidget.h"
#include "bean/UserBean.h"
#include "dao/UserDao.h"
#include "../RoleManagement/RoleManagementModel.h"
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStringListModel>

NewEditUserWidget::NewEditUserWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    handleEvents();
}

void NewEditUserWidget::setWidgetType(NewEditUserWidget::WidgetType type)
{
    initializeData();
    m_nWidgetType = type;
    switch (type) {
    case WidgetType::NewUser:
        m_pLineEditLoginName->setFocus();
        m_pLineEditLoginName->clear();
        m_pLineEditPassword->clear();
        m_pLineEditPasswordConfirm->clear();
        this->setFixedSize(300, 220);
        this->setWindowTitle(tr("新建用户"));
        break;
    case WidgetType::ChangePassword:
        this->setFixedSize(300, 190);
        this->setWindowTitle(tr("更改密码"));
        m_pLineEditLoginName->setEnabled(false);
        m_pLabelRole->setVisible(false);
        m_pComboBoxRole->setVisible(false);
        break;
    case WidgetType::ChangeRole:
        this->setFixedSize(300, 160);
        this->setWindowTitle(tr("更改角色"));
        m_pLineEditLoginName->setEnabled(false);
        m_pLabelPassword->setVisible(false);
        m_pLabelPasswordConfirm->setVisible(false);
        m_pLineEditPassword->setVisible(false);
        m_pLineEditPasswordConfirm->setVisible(false);
        break;
    default:
        break;
    }
}

void NewEditUserWidget::setUserBean(const UserBean &userBean)
{
    m_pLineEditLoginName->setText(userBean.getName());
}

void NewEditUserWidget::setRoleModle(QAbstractItemModel *modle, int modelColumn)
{
    m_pComboBoxRole->setModel(modle);
    m_pComboBoxRole->setModelColumn(modelColumn);
}

void NewEditUserWidget::initializeUi()
{
    this->resize(300, 220);

    m_pLabelPassword = new QLabel(tr("密码:"));
    m_pLabelPasswordConfirm = new QLabel(tr("确认密码:"));
    m_pLabelRole = new QLabel(tr("角色:"));

    m_pLineEditLoginName = new QLineEdit;
    m_pLineEditPassword = new QLineEdit;
    m_pLineEditPasswordConfirm = new QLineEdit;
    m_pComboBoxRole = new QComboBox;
    m_pBtnOK = new QPushButton(tr("确定"));
    m_pBtnCancel = new QPushButton(tr("取消"));

    m_pLineEditLoginName->setPlaceholderText(tr("请输入字母数字"));
    m_pLineEditPassword->setPlaceholderText(tr("请输入字母数字"));
    m_pLineEditPasswordConfirm->setPlaceholderText(tr("再次输入密码"));
    m_pLineEditPassword->setEchoMode(QLineEdit::Password);
    m_pLineEditPasswordConfirm->setEchoMode(QLineEdit::Password);

    // 输入限制
    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx);
    m_pLineEditLoginName->setValidator( validator );
    m_pLineEditPassword->setValidator( validator );
    m_pLineEditPasswordConfirm->setValidator( validator );
    m_pLineEditLoginName->setMaxLength(16);
    m_pLineEditPassword->setMaxLength(16);
    m_pLineEditPasswordConfirm->setMaxLength(16);

    auto pGLayout  = new QGridLayout;
    pGLayout->addWidget(new QLabel(tr("用户名:")), 0, 0);
    pGLayout->addWidget(m_pLineEditLoginName, 0, 1);
    pGLayout->addWidget(m_pLabelPassword, 1, 0);
    pGLayout->addWidget(m_pLineEditPassword, 1, 1);
    pGLayout->addWidget(m_pLabelPasswordConfirm, 2, 0);
    pGLayout->addWidget(m_pLineEditPasswordConfirm, 2, 1);
    pGLayout->addWidget(m_pLabelRole, 3, 0);
    pGLayout->addWidget(m_pComboBoxRole, 3, 1);
    pGLayout->setSpacing(20);

    auto pHLayout = new QHBoxLayout;
    pHLayout->addStretch();
    pHLayout->addWidget(m_pBtnOK);
    pHLayout->addWidget(m_pBtnCancel);
    pHLayout->addStretch();

    QFrame *pLine = new QFrame;
    pLine->setGeometry(QRect(40, 180, 400, 3));
    pLine->setFrameShape(QFrame::HLine);
    pLine->setFrameShadow(QFrame::Sunken);
    pLine->raise();

    auto pVLayout = new QVBoxLayout;
    pVLayout->addLayout(pGLayout);
    pVLayout->addSpacing(10);
    pVLayout->addWidget(pLine);
    pVLayout->addSpacing(10);
    pVLayout->addLayout(pHLayout);

    this->setLayout(pVLayout);
    this->layout()->setContentsMargins(15, 15, 15, 15);
}

void NewEditUserWidget::initializeData()
{
    m_pLabelPassword->setVisible(true);
    m_pLabelPasswordConfirm->setVisible(true);
    m_pLabelRole->setVisible(true);

    m_pComboBoxRole->setVisible(true);
    m_pLineEditLoginName->setVisible(true);
    m_pLineEditPassword->setVisible(true);
    m_pLineEditPasswordConfirm->setVisible(true);

    m_pLineEditLoginName->clear();
    m_pLineEditPassword->clear();
    m_pLineEditPasswordConfirm->clear();

    m_pLineEditLoginName->setEnabled(true);
}

void NewEditUserWidget::handleEvents()
{
    // 按回车键
    connect(m_pLineEditLoginName, &QLineEdit::returnPressed, [this] {
        if (checkLoginName()) {
            m_pLineEditPassword->setFocus();
        }
    });
    connect(m_pLineEditPassword, &QLineEdit::returnPressed, [this] {
        if (checkPassword()) {
            m_pLineEditPasswordConfirm->setFocus();
        }
    });
    connect(m_pLineEditPasswordConfirm, &QLineEdit::returnPressed, [this] {
        if (checkPasswordConfirm()) {
            m_pComboBoxRole->setFocus();
        }
    });

    // 取消
    connect(m_pBtnCancel, &QPushButton::clicked, [this] {
        this->close();
    });

    // 确认
    connect(m_pBtnOK, &QPushButton::clicked ,[this] {
        switch (m_nWidgetType) {
        case WidgetType::NewUser:
        {
            if (!checkLoginName()) {
                return;
            }
            if (!checkPassword()) {
                return;
            }
            if (!checkPasswordConfirm()) {
                return;
            }
            UserBean userBean;
            userBean.setName(m_pLineEditLoginName->text());
            userBean.setPassword(m_pLineEditPassword->text());

            QString roleName  = m_pComboBoxRole->currentText();
            RoleManagementModel *pRoleModel = qobject_cast<RoleManagementModel*>(m_pComboBoxRole->model());
            int roleId = pRoleModel->getRoleIdByName(roleName);
            userBean.setRoleId(roleId);
            userBean.setRoleName(roleName);
            emit newUser(userBean);
            break;
        }
        case WidgetType::ChangePassword:
        {
            if (!checkPassword()) {
                return;
            }
            if (!checkPasswordConfirm()) {
                return;
            }
            emit changePassword(m_pLineEditPassword->text());
            break;
        }
        case WidgetType::ChangeRole:
        {
            QString roleName  = m_pComboBoxRole->currentText();
            RoleManagementModel *pRoleModel = qobject_cast<RoleManagementModel*>(m_pComboBoxRole->model());
            int roleId = pRoleModel->getRoleIdByName(roleName);
            emit changeRole(roleId);
            break;
        }
        default:
            break;
        }

        this->close();
    });
}

bool NewEditUserWidget::checkLoginName()
{
    if (m_pLineEditLoginName->text().isEmpty()) {
        QMessageBox::warning(this, "warning", "用户名为空，请重新输入",QMessageBox::Ok);
        return false;
    }
    UserDao userDao;
    if (userDao.isExistLoginName(m_pLineEditLoginName->text())) {
        QMessageBox::warning(this, "warning", "该用户名已经存在，请重新输入",QMessageBox::Ok);
        return false;
    }
    return true;
}

bool NewEditUserWidget::checkPassword()
{
    if (m_pLineEditPassword->text().isEmpty()) {
        QMessageBox::warning(this, "warning", "密码为空，请重新输入",QMessageBox::Ok);
        return false;
    }
    if (m_pLineEditPassword->text().length() < 6) {
        QMessageBox::warning(this, "warning", "密码不应小于6位，请重新输入",QMessageBox::Ok);
        return false;
    }
    return true;
}

bool NewEditUserWidget::checkPasswordConfirm()
{
    if (m_pLineEditPasswordConfirm->text() != m_pLineEditPassword->text()) {
        QMessageBox::warning(this, "warning", "两次密码不一致请重新输入",QMessageBox::Ok);
        return false;
    }
    return true;
}
