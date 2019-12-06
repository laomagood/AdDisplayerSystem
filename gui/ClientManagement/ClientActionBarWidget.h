#ifndef ClientActionBarWidget_H
#define ClientActionBarWidget_H

#include <QWidget>
class QPushButton;

class ClientActionBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClientActionBarWidget(QWidget *parent = nullptr);

signals:

private:
    void initializeUi();                        // 初始化界面
    void initializeData();                      // 初始化数据
    void handleEvents();                        // 信号与槽处理

    QPushButton *m_pBtnProgram;                 // 节目
    QPushButton *m_pBtnOrder;                   // 指令
    QPushButton *m_pBtnDelete;                  // 删除按钮
    QPushButton *m_pBtnRefresh;                 // 刷新按钮
};

#endif // ClientActionBarWidget_H
