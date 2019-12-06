#ifndef NewEditUserWidget_H
#define NewEditUserWidget_H

#include <QWidget>
#include <bean/UserBean.h>
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QAbstractItemModel;

class NewEditUserWidget : public QWidget
{
    Q_OBJECT
public:
    // 用来判断是新建还是编辑
    enum WidgetType {
        NewUser,
        ChangePassword,
        ChangeRole
    };

    explicit NewEditUserWidget(QWidget *parent = nullptr);

    void setWidgetType(WidgetType type);
    void setUserBean(const UserBean &userBean);
    void setRoleModle(QAbstractItemModel* modle, int modelColumn);  // RoleModle

signals:
    void newUser(const UserBean &userBean);
    void changePassword(const QString &password);
    void changeRole(const int &role);

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    bool checkLoginName();                                  // 检查用户名
    bool checkPassword();                                   // 检查密码
    bool checkPasswordConfirm();                            // 检查确认密码

    QLabel *m_pLabelPassword;                               // 密码
    QLabel *m_pLabelPasswordConfirm;                        // 确认密码
    QLabel *m_pLabelRole;                                   // 角色

    QLineEdit *m_pLineEditLoginName;                        // 用户名
    QLineEdit *m_pLineEditPassword;                         // 密码
    QLineEdit *m_pLineEditPasswordConfirm;                  // 确认密码
    QComboBox *m_pComboBoxRole;                             // 角色
    QPushButton *m_pBtnOK;                                  // 确认按钮
    QPushButton *m_pBtnCancel;                              // 取消按钮
    WidgetType m_nWidgetType;                               // 窗口类型
    UserBean m_nUserBean;
};

#endif // NewEditUserWidget_H
