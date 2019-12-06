#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    /**
    * @brief 检测是否成功登陆
    * @return 登陆成功返回 true，失败返回false
    */
    bool isLoginSuccess() const;
    /**
    * @brief 显示登陆窗口（静态函数）
    * @return 登陆成功返回 true，失败返回false
    */
    static bool login();

private:
    Ui::LoginWidget *ui;

    void initializeUi();    // 初始化界面
    void handleEvents();    // 信号与槽处理

    // 登陆逻辑
    bool login(const QString &username, const QString &password);
    bool loginSuccess = false;
};

#endif // LOGINWIDGET_H
