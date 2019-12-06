#include "NewEditRoleWidget.h"
#include "ui_NewEditRoleWidget.h"
#include "dao/rolerightdao.h"
#include <QMessageBox>
#include <QDebug>

NewEditRoleWidget::NewEditRoleWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewEditRoleWidget)
{
    ui->setupUi(this);
    initializeUi();
    initializeData();
    handleEvents();
}

NewEditRoleWidget::~NewEditRoleWidget()
{
    delete ui;
}

void NewEditRoleWidget::setWidgetType(NewEditRoleWidget::WidgetType type)
{
    m_nWidgetType = type;
    initializeData();
}

void NewEditRoleWidget::setBean(const RoleRightBean &roleRightBean)
{
    m_nRoleRightBean = roleRightBean;
    initializeData();
}

void NewEditRoleWidget::initializeUi()
{
    ui->lineEdit_roleName->setFocus();
    ui->lineEdit_roleName->setPlaceholderText(tr("请输入角色名(可以中文)"));
    ui->lineEdit_roleDescription->setPlaceholderText(tr("请输入角色的描述(如：拥有所有权限)"));
}

void NewEditRoleWidget::initializeData()
{
    switch (m_nWidgetType) {
    case NewRole:
    {
        this->setWindowTitle(tr("新建角色"));
        ui->lineEdit_roleName->setText("");
        ui->lineEdit_roleDescription->setText("");

        ui->checkBox_genUser->setChecked(false);
        ui->checkBox_manageUser->setChecked(false);
        ui->checkBox_deleteUser->setChecked(false);

        ui->checkBox_genRole->setChecked(false);
        ui->checkBox_manageRole->setChecked(false);
        ui->checkBox_deleteRole->setChecked(false);

        ui->checkBox_uploadMaterial->setChecked(false);
        ui->checkBox_checkMaterial->setChecked(false);
        ui->checkBox_deleteMaterial->setChecked(false);

        ui->checkBox_uploadProgram->setChecked(false);
        ui->checkBox_checkProgram->setChecked(false);
        ui->checkBox_deleteProgram->setChecked(false);
        ui->checkBox_genProgram->setChecked(false);
        ui->checkBox_releaseProgram->setChecked(false);
        ui->checkBox_manageProgram->setChecked(false);
        break;
    }
    case ChangeRight:
    {
        this->setWindowTitle(tr("编辑角色"));

        RoleBean *pRoleBean = m_nRoleRightBean.getRoleBean();
        RightBean *pRightBean = m_nRoleRightBean.getRightBean();

        ui->lineEdit_roleName->setText(pRoleBean->getName());
        ui->lineEdit_roleDescription->setText(pRoleBean->getDescription());

        ui->checkBox_genUser->setChecked(pRightBean->isGenUser());
        ui->checkBox_manageUser->setChecked(pRightBean->isManageUser());
        ui->checkBox_deleteUser->setChecked(pRightBean->isDeleteUser());

        ui->checkBox_genRole->setChecked(pRightBean->isGenRole());
        ui->checkBox_manageRole->setChecked(pRightBean->isManageRole());
        ui->checkBox_deleteRole->setChecked(pRightBean->isDeleteRole());

        ui->checkBox_uploadMaterial->setChecked(pRightBean->isUploadMaterial());
        ui->checkBox_checkMaterial->setChecked(pRightBean->isCheckMaterial());
        ui->checkBox_deleteMaterial->setChecked(pRightBean->isDeleteMaterial());

        ui->checkBox_uploadProgram->setChecked(pRightBean->isUploadProgram());
        ui->checkBox_checkProgram->setChecked(pRightBean->isCheckProgram());
        ui->checkBox_deleteProgram->setChecked(pRightBean->isDeleteProgram());
        ui->checkBox_genProgram->setChecked(pRightBean->isGenProgram());
        ui->checkBox_releaseProgram->setChecked(pRightBean->isReleaseProgram());
        ui->checkBox_manageProgram->setChecked(pRightBean->isManageProgram());
        break;
    }
    default:
        break;
    }
}

void NewEditRoleWidget::handleEvents()
{
    connect(ui->lineEdit_roleName, &QLineEdit::returnPressed, [this] {
        if (!checkName())
            return;
        ui->lineEdit_roleDescription->setFocus();
    });
    connect(ui->btn_ok, &QPushButton::clicked, [this] {
        if (!checkName())
            return;

        RoleBean *pRoleBean = m_nRoleRightBean.getRoleBean();
        RightBean *pRightBean = m_nRoleRightBean.getRightBean();

        pRoleBean->setName(ui->lineEdit_roleName->text());
        pRoleBean->setDescription(ui->lineEdit_roleDescription->text());

        pRightBean->setGenUser(ui->checkBox_genUser->isChecked());
        pRightBean->setManageUser(ui->checkBox_manageUser->isChecked());
        pRightBean->setDeleteUser(ui->checkBox_deleteUser->isChecked());

        pRightBean->setGenRole(ui->checkBox_genRole->isChecked());
        pRightBean->setManageRole(ui->checkBox_manageRole->isChecked());
        pRightBean->setDeleteRole(ui->checkBox_deleteRole->isChecked());

        pRightBean->setUploadMaterial(ui->checkBox_uploadMaterial->isChecked());
        pRightBean->setCheckMaterial(ui->checkBox_checkMaterial->isChecked());
        pRightBean->setDeleteMaterial(ui->checkBox_deleteMaterial->isChecked());

        pRightBean->setUploadProgram(ui->checkBox_uploadProgram->isChecked());
        pRightBean->setCheckProgram(ui->checkBox_checkProgram->isChecked());
        pRightBean->setDeleteProgram(ui->checkBox_deleteProgram->isChecked());
        pRightBean->setGenProgram(ui->checkBox_genProgram->isChecked());
        pRightBean->setReleaseProgram(ui->checkBox_releaseProgram->isChecked());
        pRightBean->setManageProgram(ui->checkBox_manageProgram->isChecked());

        switch (m_nWidgetType) {
        case NewRole:
        {
            emit newRole(m_nRoleRightBean);
            break;
        }
        case ChangeRight:
        {
            emit changeRole(m_nRoleRightBean);
            break;
        }
        default:
            break;
        }
        this->close();
    });
    connect(ui->btn_cancel, &QPushButton::clicked, [this] {
        this->close();
    });
}

bool NewEditRoleWidget::checkName()
{
    if (ui->lineEdit_roleName->text().isEmpty()) {
        QMessageBox::warning(this, "warning", "用户名为空，请重新输入",QMessageBox::Ok);
        return false;
    }
    // 当编辑时，名字没有变更不用去数据库检查名字是否已经存在
    if (m_nWidgetType == ChangeRight) {
        QString name = m_nRoleRightBean.getRoleBean()->getName();
        if (name == ui->lineEdit_roleName->text())
            return true;
    }
    RoleDao roleDao;
    if (roleDao.isExistName(ui->lineEdit_roleName->text())) {
        QMessageBox::warning(this, "warning", "该用户名已经存在，请重新输入",QMessageBox::Ok);
        return false;
    }
    return true;
}
