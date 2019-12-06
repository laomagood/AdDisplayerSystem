#include "ClientManagementWidget.h"
#include "../../function/Transfer/DeviceBroadcaster.h"
#include "../../function/Transfer/DeviceListModel.h"
#include "../../function/Transfer/Sender.h"
#include "../../function/Transfer/Transfertablemodel.h"
#include "../../function/Transfer/TransferTableWidget.h"
#include "../../function/Transfer/Transferserver.h"
#include "util/Util.h"

#include "ClientSideBarWidget.h"
#include "ClientActionBarWidget.h"
#include <QDebug>
#include <QSqlTableModel>
#include <QTableView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QFileDialog>
#include <QProgressBar>

ClientManagementWidget::ClientManagementWidget(QWidget *parent) : QWidget(parent)
{
    setupActions();
    initializeUi();
    initializeData();
    handleEvents();
}

ClientManagementWidget::~ClientManagementWidget()
{

}

void ClientManagementWidget::initializeUi()
{
    // 侧边栏
    m_pClientSideBarWidget = new ClientSideBarWidget(this);

    // 模型------------------------------------------------------------------
    // 广播（客服端）
    auto pBroadcaster = new DeviceBroadcaster(this);
    pBroadcaster->start();
    m_pDeviceListModel = new DeviceListModel(pBroadcaster, this);

    // 创建服务器并监听
    m_pTransferServer = new TransferServer(m_pDeviceListModel, this);
    m_pTransferServer->listen();


    // 视图------------------------------------------------------------------
    m_pClientTableView = new QTableView(this);
    m_pClientTableView->setModel(m_pDeviceListModel);
    //    m_pClientTableView->setItemDelegate(m_pUserManagementDeleget);

    //    m_pClientTableView->setSortingEnabled(true);                                                  // 排序
    m_pClientTableView->setSelectionMode(QAbstractItemView::ExtendedSelection);                   // 选择模式
    m_pClientTableView->setSelectionBehavior(QAbstractItemView::QAbstractItemView::SelectRows);   // 选择行
    m_pClientTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);                       // 不可编辑
    //    m_pClientTableView->setShowGrid(false);                                                       // 设置不显示格子线
    m_pClientTableView->horizontalHeader()->setHighlightSections(false);                          // 点击表头不高亮
    m_pClientTableView->verticalHeader()->hide();                                                 // 设置垂直头不可见
    m_pClientTableView->setFrameShape(QFrame::NoFrame);                                           // 设置无边框
    m_pClientTableView->horizontalHeader()->setFixedHeight(30);                                   // 设置表头的高度
    m_pClientTableView->setMouseTracking(true);

    // 列宽调整
    // [1] 获取分辨率
    QScreen *screen = QGuiApplication::primaryScreen ();
    QRect mm = screen->availableGeometry() ;
    int nWidth = (mm.width() - 240 - 60 - 120*2)/4;
    nWidth = nWidth > 135 ? nWidth : 135;

    // [2] 设置列宽
    m_pClientTableView->horizontalHeader()->setStretchLastSection(true);
    m_pClientTableView->setColumnWidth(0, 60);
    m_pClientTableView->setColumnWidth(1, nWidth);
    m_pClientTableView->setColumnWidth(2, 120);
    m_pClientTableView->setColumnWidth(3, 120);
    m_pClientTableView->setColumnWidth(4, nWidth);
    m_pClientTableView->setColumnWidth(5, nWidth);
    m_pClientTableView->setColumnWidth(6, nWidth);

    // 行高
    m_pClientTableView->verticalHeader()->setDefaultSectionSize(35);

    // 设置样式
    m_pClientTableView->setAlternatingRowColors(true);
    m_pClientTableView->setStyleSheet( "QTableView{ background-color: rgb(255, 255, 255);"
                                       "alternate-background-color: rgb(230, 230, 255);"
                                       "selection-background-color:rgb(65, 180, 80);}" );

    m_pClientTableView->horizontalHeader()->setStyleSheet("QHeaderView::section{ background:#1E90FF;"
                                                          "border:0px solid #E0DDDC;"
                                                          "border-bottom:1px solid #E0DDDC;"
                                                          "height:40 };");

    //设置表头字体加粗
    QFont font = m_pClientTableView->horizontalHeader()->font();
    font.setBold(true);
    m_pClientTableView->horizontalHeader()->setFont(font);

    // 右键
    m_pClientTableView->setContextMenuPolicy(Qt::CustomContextMenu);

    //------------------------------------------------------------------
    m_pClientActionBarWidget = new ClientActionBarWidget;

    auto pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pClientActionBarWidget);
    pLayout->addWidget(m_pClientTableView);
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
    pMainLayout->addWidget(m_pClientSideBarWidget);
    pMainLayout->addWidget(pFrame);
    this->setLayout(pMainLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);
    this->layout()->setSpacing(0);
}

void ClientManagementWidget::initializeData()
{

}

void ClientManagementWidget::handleEvents()
{
    connect(m_pClientTableView, &QTableView::customContextMenuRequested, this, &ClientManagementWidget::onClinetTableContextMenuRequested);
}

void ClientManagementWidget::setupActions()
{
    m_clientInfoAction = new QAction(tr("查看信息"), this);
    m_sendProgramAction = new QAction(QIcon("./image/ClientManagement/SendProgram.png"), tr("发送节目"), this);
    m_sendOrderAction = new QAction(QIcon("./image/ClientManagement/SendOrder.png"), tr("发送指令"), this);
    m_deleteAction = new QAction(tr("删除"), this);

    connect(m_clientInfoAction, &QAction::triggered, this, &ClientManagementWidget::onClientInfoActionTriggered);
    connect(m_sendProgramAction, &QAction::triggered, this, &ClientManagementWidget::onSendProgramActionTriggered);
    connect(m_sendOrderAction, &QAction::triggered, this, &ClientManagementWidget::onSendOrderActionTriggered);
    connect(m_deleteAction, &QAction::triggered, this, &ClientManagementWidget::onDeleteActionTriggered);
}

void ClientManagementWidget::setupToolbar()
{

}

void ClientManagementWidget::onClinetTableContextMenuRequested(const QPoint& pos)
{
    QModelIndex currIndex = m_pClientTableView->indexAt(pos);
    QMenu contextMenu;

    if (currIndex.isValid()) {
        Device dev = m_pDeviceListModel->device(currIndex.row());
        if (dev.getStatus() == ConnectionStatus::Offline) {
            m_clientInfoAction->setEnabled(false);
            m_sendProgramAction->setEnabled(false);
            m_sendOrderAction->setEnabled(false);
            m_deleteAction->setEnabled(true);
        } else {
            m_clientInfoAction->setEnabled(true);
            m_sendProgramAction->setEnabled(true);
            m_sendOrderAction->setEnabled(true);
            m_deleteAction->setEnabled(false);
        }

        contextMenu.addAction(m_clientInfoAction);
        contextMenu.addAction(m_sendProgramAction);
        contextMenu.addAction(m_sendOrderAction);
        contextMenu.addSeparator();
        contextMenu.addAction(m_deleteAction);
    } else { }

    QPoint globPos = m_pClientTableView->mapToGlobal(pos);
    contextMenu.exec(globPos);
}

void ClientManagementWidget::onClientInfoActionTriggered()
{

}

void ClientManagementWidget::onSendProgramActionTriggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开节目"), "/home", tr("节目文件 (*.addi)"));
    QStringList fileList = Util::getADJsonFiles(fileName);
    QVector<Device> receivers = clinetTableSelectedDevices();

    // 创建Model与View
    auto mSenderModel =  new TransferTableModel();
    auto mSenderWidget = new TransferTableWidget(mSenderModel);
    mSenderModel->setParent(mSenderWidget);
    mSenderWidget->show();

    for (Device &receiver : receivers) {
        // 离线
        if (receiver.getStatus() == ConnectionStatus::Offline)
            continue;
        for (auto &file : fileList) {
            Sender* sender = new Sender(receiver, "AA", file, this);
            sender->start();
            mSenderModel->insertTransfer(sender);

            // 进度条
            QModelIndex progressIdx = mSenderModel->index(0, (int)TransferTableModel::Column::Progress);
            QProgressBar* progress = new QProgressBar();
            progress->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            connect(sender->getTransferInfo(), &TransferInfo::progressChanged, progress, &QProgressBar::setValue);

            mSenderWidget->setIndexWidget(progressIdx, progress);
            mSenderWidget->scrollToTop();
        }
    }
}

void ClientManagementWidget::onSendOrderActionTriggered()
{
    clinetTableSelectedDevices();
}

void ClientManagementWidget::onDeleteActionTriggered()
{

}

// 表格选择的客服端
QVector<Device> ClientManagementWidget::clinetTableSelectedDevices()
{
    QVector<Device> receivers;
    QItemSelectionModel *pModelSelection = m_pClientTableView->selectionModel();
    QModelIndexList modelIndexList= pModelSelection->selectedRows();
    for (const auto &index : modelIndexList){
        if (index.isValid()) {
            Device dev = m_pDeviceListModel->device(index.row());
            if (dev.isValid()) {
                receivers.append(dev);
            }
        }
    }
    return receivers;
}

// 发送：确定文件
void ClientManagementWidget::selectFiles()
{
    QStringList fileNames;
    if (fileNames.size() <= 0)
        return;

    for (const auto& fName : fileNames)
        ;
}
