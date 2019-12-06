#ifndef SettingSideBarWidget_H
#define SettingSideBarWidget_H

#include <QWidget>
class QPushButton;
class QButtonGroup;

class SettingSideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SettingSideBarWidget(QWidget *parent = nullptr);
    QPushButton *getClickedBtn();                       // 获取当前选中的按钮
    void initializeData();                              // 初始化点击按钮

signals:
    void buttonClicked();                               // 发送信号

private:
    void initializeUi();                                // 初始化界面
    void handleEvents();                                // 信号与槽处理

    QPushButton *m_pBtnUserManage;                      // 用户管理
    QPushButton *m_pBtnRoleManage;                      // 角色管理
    QPushButton *m_pBtnSysSetting;                      // 系统设置
    QButtonGroup *m_pGroupButtons;                      // 分类按钮组
};

#endif // SettingSideBarWidget_H
