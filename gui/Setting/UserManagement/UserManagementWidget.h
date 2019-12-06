#ifndef UserManagementWidget_H
#define UserManagementWidget_H

#include <QWidget>
class UserActionBarWidget;
class MultipageButtonsWidget;
class UserManagementModel;
class UserManagementDeleget;
class NewEditUserWidget;
class UserManagementProxyModel;

class UserManagementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserManagementWidget(QWidget *parent = nullptr);
    ~UserManagementWidget();

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    UserActionBarWidget *m_pUserActionBarWidget;            // 过滤栏
    MultipageButtonsWidget *m_pMultipageButtonsWidget;      // 多页按钮

    UserManagementModel *m_pUserManagementModel;            // 模型
    UserManagementDeleget *m_pUserManagementDeleget;        // 代理
    UserManagementProxyModel *m_pUserManagementProxyModel;  // 代理模型

    NewEditUserWidget *m_pNewEditUserWidget;                // 新建编辑用户对话框

};

#endif // UserManagementWidget_H
