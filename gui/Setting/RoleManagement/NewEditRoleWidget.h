#ifndef NEWEDITROLEWIDGET_H
#define NEWEDITROLEWIDGET_H

#include <QWidget>
#include "bean/RoleRightBean.h"

namespace Ui {
class NewEditRoleWidget;
}

class NewEditRoleWidget : public QWidget
{
    Q_OBJECT

public:
    // 用来判断是新建还是编辑
    enum WidgetType {
        NewRole,
        ChangeRight
    };

    explicit NewEditRoleWidget(QWidget *parent = 0);
    ~NewEditRoleWidget();
    void setWidgetType(WidgetType type);
    void setBean(const RoleRightBean &roleRightBean);

signals:
    void newRole(RoleRightBean roleRightBean);
    void changeRole(RoleRightBean roleRightBean);

private:
    Ui::NewEditRoleWidget *ui;
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    bool checkName();                                       // 检查角色名

    WidgetType m_nWidgetType;                               // 窗口类型
    RoleRightBean m_nRoleRightBean;
};

#endif // NEWEDITROLEWIDGET_H
