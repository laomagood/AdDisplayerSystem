#ifndef ClientManagementWidget_H
#define ClientManagementWidget_H

#include <QWidget>
#include "../../function/Transfer/DeviceListModel.h"
class QTableView;
class DeviceListModel;
class ClientSideBarWidget;
class ClientActionBarWidget;
class TransferServer;

class ClientManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClientManagementWidget(QWidget *parent = nullptr);
    ~ClientManagementWidget();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理
    void setupActions();                                    // 安装动作按钮
    void setupToolbar();                                    // 安装工具栏
    void onClinetTableContextMenuRequested(const QPoint& pos);      // 右键菜单

    void onClientInfoActionTriggered();                     // 点击客服端信息
    void onSendProgramActionTriggered();                    // 点击发送节目
    void onSendOrderActionTriggered();                      // 点击发送指令
    void onDeleteActionTriggered();                         // 点击删除

    void selectFiles();                                     // 发送：选择文件
    QVector<Device> clinetTableSelectedDevices();           // 表格选择的客服端

    QTableView *m_pClientTableView;                         // 客户端列表
    ClientSideBarWidget *m_pClientSideBarWidget;            // 侧边栏
    ClientActionBarWidget *m_pClientActionBarWidget;        // 动作栏
    DeviceListModel *m_pDeviceListModel;                    // 模型
    TransferServer *m_pTransferServer;                      // 服务器监听

    QAction* m_clientInfoAction;                            // 发送节目
    QAction* m_sendProgramAction;                           // 发送节目
    QAction* m_sendOrderAction;                             // 发送指令
    QAction* m_deleteAction;                                // 删除
};

#endif // ClientManagementWidget_H
