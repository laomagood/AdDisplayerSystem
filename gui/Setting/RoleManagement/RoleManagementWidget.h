#ifndef RoleManagementWidget_H
#define RoleManagementWidget_H

#include <QWidget>
class RoleActionBarWidget;
class MultipageButtonsWidget;
class RoleManagementModel;
class RoleManagementDeleget;
class NewEditRoleWidget;

class RoleManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RoleManagementWidget(QWidget *parent = nullptr);
    ~RoleManagementWidget();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    RoleActionBarWidget *m_pRoleActionBarWidget;            // 过滤栏
    MultipageButtonsWidget *m_pMultipageButtonsWidget;      // 多页按钮

    RoleManagementModel *m_pRoleManagementModel;            // 模型
    RoleManagementDeleget *m_pRoleManagementDeleget;        // 代理

    NewEditRoleWidget *m_pNewEditRoleWidget;                // 新建编辑用户对话框

};

#endif // RoleManagementWidget_H
