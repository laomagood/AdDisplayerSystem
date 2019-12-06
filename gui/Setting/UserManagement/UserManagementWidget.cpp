#include "UserManagementWidget.h"
#include "UserActionBarWidget.h"
#include "gui/MaterialManagement/MultipageButtonsWidget.h"
#include "NewEditUserWidget.h"
#include "UserManagementModel.h"
#include "UserManagementDeleget.h"
#include "UserManagementProxyModel.h"
#include "../RoleManagement/RoleManagementModel.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QScopedPointer>
#include <QMessageBox>

UserManagementWidget::UserManagementWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

UserManagementWidget::~UserManagementWidget()
{
    delete m_pNewEditUserWidget;
    m_pNewEditUserWidget = NULL;
}

void UserManagementWidget::initializeUi()
{
    // 这个为直接用QSqlTableModel，我们这里不用先注释掉
    /*
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "Connection_Name");
    db.setDatabaseName("./data/AdDisplayerSystem_Server.db");
    if (!db.open()) {
        qDebug() << "UserManagementWidget:" << "数据库打开失败";
    }

    // 模型------------------------------------------------------------------
    QSqlTableModel *pModel = new QSqlTableModel(this, db);
    pModel->setTable("TUser");
    pModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    pModel->select();
    // 移除密码行
    pModel->removeColumn(2);
    pModel->setHeaderData(0, Qt::Horizontal, "序号");
    pModel->setHeaderData(1, Qt::Horizontal, "用户名");
    pModel->setHeaderData(2, Qt::Horizontal, "创建时间");
    pModel->setHeaderData(3, Qt::Horizontal, "最近登录时间");
    pModel->setHeaderData(4, Qt::Horizontal, "上次登录时间");
    pModel->setHeaderData(5, Qt::Horizontal, "角色");
    pModel->sort(0, Qt::AscendingOrder); // 第0列升序排序
    */

    // 模型------------------------------------------------------------------
    m_pUserManagementModel = new UserManagementModel(this);
    m_pUserManagementDeleget = new UserManagementDeleget(this);
    m_pUserManagementProxyModel = new UserManagementProxyModel(this);
    m_pUserManagementProxyModel->setSourceModel(m_pUserManagementModel);

    // 视图------------------------------------------------------------------
    QTableView *pView = new QTableView(this);
    pView->setModel(m_pUserManagementProxyModel);
    pView->setItemDelegate(m_pUserManagementDeleget);

    //    pView->setSortingEnabled(true);                                                  // 排序
    pView->setSelectionMode(QAbstractItemView::ExtendedSelection);                   // 选择模式
    pView->setSelectionBehavior(QAbstractItemView::QAbstractItemView::SelectRows);   // 选择行
    pView->setEditTriggers(QAbstractItemView::NoEditTriggers);                       // 不可编辑
    //    pView->setShowGrid(false);                                                       // 设置不显示格子线
    pView->horizontalHeader()->setHighlightSections(false);                          // 点击表头不高亮
    pView->verticalHeader()->hide();                                                 // 设置垂直头不可见
    pView->setFrameShape(QFrame::NoFrame);                                           // 设置无边框
    pView->horizontalHeader()->setFixedHeight(30);                                   // 设置表头的高度
    pView->setMouseTracking(true);

    // 列宽调整
    // [1] 获取分辨率
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect mm = screen->availableGeometry() ;
    int nWidth = (mm.width() - 240 - 80*3 - 300)/4;
    nWidth = nWidth > 135 ? nWidth : 135;

    // [2] 设置列宽
    pView->horizontalHeader()->setStretchLastSection(true);
    pView->setColumnWidth(0, 60);
    pView->setColumnWidth(1, 60);
    pView->setColumnWidth(2, nWidth);
    pView->setColumnWidth(3, nWidth);
    pView->setColumnWidth(4, nWidth);
    pView->setColumnWidth(5, nWidth);
    pView->setColumnWidth(6, 120);

    // 行高
    pView->verticalHeader()->setDefaultSectionSize(35);

    // 设置样式
    pView->setAlternatingRowColors(true);
    pView->setStyleSheet( "QTableView{ background-color: rgb(255, 255, 255);"
                          "alternate-background-color: rgb(220, 220, 250);"
                          "selection-background-color:rgb(200, 200, 200);}" );

    pView->horizontalHeader()->setStyleSheet("QHeaderView::section{ background:#1E90FF;"
                                             "border:0px solid #E0DDDC;"
                                             "border-bottom:1px solid #E0DDDC;"
                                             "height:40 };");

    //设置表头字体加粗
    QFont font = pView->horizontalHeader()->font();
    font.setBold(true);
    pView->horizontalHeader()->setFont(font);

    //------------------------------------------------------------------
    // [1] 上：操作栏
    // [2] 中：TableView布局区
    // [3] 下：多页翻页按钮(先不实现)
    m_pUserActionBarWidget = new UserActionBarWidget;
    m_pMultipageButtonsWidget = new MultipageButtonsWidget;

    // 设置菜单栏角色筛选的模型
    m_pUserActionBarWidget->setModel(m_pUserManagementModel);

    auto pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pUserActionBarWidget);
    pLayout->addWidget(pView);
    //    pLayout->addWidget(m_pMultipageButtonsWidget);
    pLayout->setMargin(0);
    pLayout->setContentsMargins(15, 15, 15, 0);

    //  右区：用于美观的外框pFrame
    auto pFrame  = new QFrame;
    pFrame->setObjectName("centreFrame");
    pFrame->setLayout(pLayout);
    this->setStyleSheet("#centreFrame {"
                        "background: rgb(255, 255, 255);"
                        "border:15px solid rgb(230, 230, 230);}");

    auto pMainLayout = new QHBoxLayout;
    pMainLayout->addWidget(pFrame);
    this->setLayout(pMainLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);

    // 编辑新建用户对话框
    m_pNewEditUserWidget = new NewEditUserWidget();
    auto pRoleManagementModel = new RoleManagementModel(this);
    m_pNewEditUserWidget->setRoleModle(pRoleManagementModel, 2);
}

void UserManagementWidget::initializeData()
{

}

void UserManagementWidget::handleEvents()
{
    // 删除数据(单条)
    connect(m_pUserManagementDeleget, &UserManagementDeleget::deleteData, [this](const QModelIndex &index) {
        QString strName = m_pUserManagementModel->getUserBean(index).getName();
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setWindowTitle(tr("删除"));
        msgBox.setText(tr("删除用户: ") + strName);
        msgBox.setInformativeText(tr("是否确认要删除此用户？(删除后该用户将无法登录系统)"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            m_pUserManagementModel->removeData(index);
        }
    });
    // 删除数据(多条)
    connect(m_pUserActionBarWidget, &UserActionBarWidget::deleteData, [this] {
        if (m_pUserManagementModel->isChecked()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle(tr("删除用户"));
            msgBox.setInformativeText(tr("是否确认要删除勾选的用户？(删除后这些用户将无法登录系统)"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes) {
                m_pUserManagementModel->removeDatas();
            }
        }
    });
    // 新增
    connect(m_pUserActionBarWidget, &UserActionBarWidget::insert, [this] {
        m_pNewEditUserWidget->setWindowModality(Qt::ApplicationModal);
        m_pNewEditUserWidget->setWidgetType(NewEditUserWidget::NewUser);
        m_pNewEditUserWidget->show();
    });

    connect(m_pNewEditUserWidget, &NewEditUserWidget::newUser, [this] (const UserBean &userBean) {
        m_pUserManagementModel->insertData(userBean);
    });
    // 编辑
    connect(m_pUserManagementDeleget, &UserManagementDeleget::edit, [this](const QModelIndex &index) {
        QString strName = m_pUserManagementModel->getUserBean(index).getName();
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("编辑"));
        msgBox.setText(tr("编辑用户: ") + strName);
        msgBox.setInformativeText(tr("请选择你需要更改的内容"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.button(QMessageBox::Yes)->setText("更改密码");
        msgBox.button(QMessageBox::No)->setText("更改权限");
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            m_pNewEditUserWidget->setWindowModality(Qt::ApplicationModal);
            m_pNewEditUserWidget->setWidgetType(NewEditUserWidget::ChangePassword);
            m_pNewEditUserWidget->setUserBean(m_pUserManagementModel->getUserBean(index));
            m_pNewEditUserWidget->show();
        } else if (ret == QMessageBox::No) {
            m_pNewEditUserWidget->setWindowModality(Qt::ApplicationModal);
            m_pNewEditUserWidget->setWidgetType(NewEditUserWidget::ChangeRole);
            m_pNewEditUserWidget->setUserBean(m_pUserManagementModel->getUserBean(index));
            m_pNewEditUserWidget->show();
        }

        connect(m_pNewEditUserWidget, &NewEditUserWidget::changePassword, [index, this] (const QString &password) {
            m_pUserManagementModel->updataBy(index, password);
        });

        connect(m_pNewEditUserWidget, &NewEditUserWidget::changeRole, [index, this] (const int &role) {
            m_pUserManagementModel->updataBy(index, role);
        });
    });

    // 查询
    connect(m_pUserActionBarWidget, &UserActionBarWidget::search, [this](const QString &name) {
        if (!m_pUserManagementModel->findData(name)) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("查询用户"));
            msgBox.setInformativeText(tr("没有找到你要查询的用户名"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    });
    // 刷新
    connect(m_pUserActionBarWidget, &UserActionBarWidget::refresh, [this] {
        m_pUserManagementModel->loadData();
    });

    // 角色筛选
    connect(m_pUserActionBarWidget, &UserActionBarWidget::roleCheckClicked, [this](bool checked) {
        if (checked) {
            QString strRole = m_pUserActionBarWidget->getComboBoxText();
            m_pUserManagementProxyModel->setRole(strRole);
        } else {
            m_pUserManagementProxyModel->clearFilters();
            m_pUserManagementModel->loadData();
        }
    });
    connect(m_pUserActionBarWidget, &UserActionBarWidget::roleComboBoxTextChanged, [this] (const QString &text){
        if (m_pUserActionBarWidget->isCheckBoxClicked()) {
            m_pUserManagementProxyModel->setRole(text);
        }
    });

}
