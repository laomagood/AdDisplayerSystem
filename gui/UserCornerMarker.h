#ifndef USERCORNERMARKER_H
#define USERCORNERMARKER_H

#include <QWidget>
#include "bean/UserBean.h"
class QLabel;
class QPushButton;

class UserCornerMarker : public QWidget
{
    Q_OBJECT
public:
    explicit UserCornerMarker(QWidget *parent = nullptr);
    void setUserBean(const UserBean &userBean);
    UserBean getUserBean() const { return m_userBean; }

private:
    void initializeUi();                        // 初始化界面
    void initializeData();                      // 初始化数据
    void handleEvents();                        // 信号与槽处理
    void setupActions();

private:
    QLabel *m_pUserIcon;                        // 用户头像
    QLabel *m_pUserName;                        // 用户名称
    QPushButton *m_pDropDownBtn;                // 下拉按钮

    QAction *m_changeUser;                      // 切换用户
    QAction *m_changePassword;                  // 更改密码

    UserBean m_userBean;
};

#endif // USERCORNERMARKER_H
