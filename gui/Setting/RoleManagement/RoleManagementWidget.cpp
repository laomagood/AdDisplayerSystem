#include "RoleManagementWidget.h"
#include "RoleActionBarWidget.h"
#include "gui/MaterialManagement/MultipageButtonsWidget.h"
#include "NewEditRoleWidget.h"
#include "RoleManagementModel.h"
#include "RoleManagementDeleget.h"
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

RoleManagementWidget::RoleManagementWidget(QWidget *parent) : QWidget(parent)
{
    initializeUi();
    initializeData();
    handleEvents();
}

RoleManagementWidget::~RoleManagementWidget()
{
    delete m_pNewEditRoleWidget;
    m_pNewEditRoleWidget = NULL;
}

void RoleManagementWidget::initializeUi()
{
    // 模型------------------------------------------------------------------
    m_pRoleManagementModel = new RoleManagementModel(this);
    m_pRoleManagementDeleget = new RoleManagementDeleget(this);

    // 视图------------------------------------------------------------------
    QTableView *pView = new QTableView(this);
    pView->setModel(m_pRoleManagementModel);
    pView->setItemDelegate(m_pRoleManagementDeleget);

    //    pView->setSortingEnabled(true);                                                  // 排序
    pView->setSelectionMode(QAbstractItemView::NoSelection);                         // 不能选中
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
    pView->setColumnWidth(5, nWidth + 120);

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
    m_pRoleActionBarWidget = new RoleActionBarWidget;
    m_pMultipageButtonsWidget = new MultipageButtonsWidget;

    auto pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pRoleActionBarWidget);
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
    m_pNewEditRoleWidget = new NewEditRoleWidget();
}

void RoleManagementWidget::initializeData()
{

}

void RoleManagementWidget::handleEvents()
{
    // 删除数据(单条)
    connect(m_pRoleManagementDeleget, &RoleManagementDeleget::deleteData, [this](const QModelIndex &index) {
        QString strName = m_pRoleManagementModel->getRoleRightBean(index).getRoleBean()->getName();
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("删除用户"));
        msgBox.setText(tr("删除用户: ") + strName);
        msgBox.setInformativeText(tr("是否确认要删除此角色？(删除后该用户将无法登录系统)"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes) {
            m_pRoleManagementModel->removeData(index);
        }
    });
    // 删除数据(多条)
    connect(m_pRoleActionBarWidget, &RoleActionBarWidget::deleteData, [this] {
        if (m_pRoleManagementModel->isChecked()) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle(tr("删除用户"));
            msgBox.setInformativeText(tr("是否确认要删除勾选的用户？(删除后这些用户将无法登录系统)"));
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Cancel);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Yes) {
                m_pRoleManagementModel->removeDatas();
            }
        }
    });
    // 新增
    connect(m_pRoleActionBarWidget, &RoleActionBarWidget::insert, [this] {
        m_pNewEditRoleWidget->setWindowModality(Qt::ApplicationModal);
        m_pNewEditRoleWidget->setWidgetType(NewEditRoleWidget::NewRole);
        m_pNewEditRoleWidget->show();
    });

    connect(m_pNewEditRoleWidget, &NewEditRoleWidget::newRole, [this] (RoleRightBean bean) {
        m_pRoleManagementModel->insertData(bean);
    });

    // 编辑
    connect(m_pRoleManagementDeleget, &RoleManagementDeleget::edit, [this](const QModelIndex &index) {
        m_pNewEditRoleWidget->setWindowModality(Qt::ApplicationModal);
        m_pNewEditRoleWidget->setWidgetType(NewEditRoleWidget::ChangeRight);
        m_pNewEditRoleWidget->setBean(m_pRoleManagementModel->getRoleRightBean(index));

        m_pNewEditRoleWidget->show();

        connect(m_pNewEditRoleWidget, &NewEditRoleWidget::changeRole, [index, this] (RoleRightBean roleRightBean) {
            m_pRoleManagementModel->updataBy(index, roleRightBean);
        });
    });

    // 查询
    connect(m_pRoleActionBarWidget, &RoleActionBarWidget::search, [this](const QString &name) {
        if (!m_pRoleManagementModel->findData(name)) {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("查询用户"));
            msgBox.setInformativeText(tr("没有找到你要查询的用户名"));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    });
    // 刷新
    connect(m_pRoleActionBarWidget, &RoleActionBarWidget::refresh, [this] {
        m_pRoleManagementModel->loadData();
    });
}
