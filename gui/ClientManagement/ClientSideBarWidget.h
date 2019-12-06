#ifndef ClientSideBarWidget_H
#define ClientSideBarWidget_H

#include <QWidget>
class QPushButton;
class QButtonGroup;

class ClientSideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClientSideBarWidget(QWidget *parent = nullptr);
    QPushButton *getClickedBtn();                       // 获取当前选中的按钮
    void initializeData();                              // 初始化点击按钮

signals:
    void buttonClicked();                               // 发送信号

private:
    void initializeUi();                                // 初始化界面
    void handleEvents();                                // 信号与槽处理

    QPushButton *m_pBtnClientManage;                    // 终端管理
    QPushButton *m_pBtnTransferManage;                  // 角色管理
    QPushButton *m_pBtnSysSetting;                      // 系统设置
    QButtonGroup *m_pGroupButtons;                      // 分类按钮组
};

#endif // ClientSideBarWidget_H
