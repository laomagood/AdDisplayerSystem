#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
class SettingSideBarWidget;
class QStackedWidget;
class UserManagementWidget;
class RoleManagementWidget;
class QPushButton;

class Setting : public QWidget
{
    Q_OBJECT
public:
    explicit Setting(QWidget *parent = nullptr);

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    void createWidgetInContentStackedWidget(QPushButton *button);

    SettingSideBarWidget *m_pSettingSideBarWidget;
    QStackedWidget *m_pStackedWidget;
    UserManagementWidget *m_pUserManagementWidget;
    RoleManagementWidget *m_pRoleManagementWidget;

    QHash<QPushButton *, QWidget *> m_nButtonWidgetHash;

};

#endif // SETTING_H
