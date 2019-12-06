#ifndef MainWidget_H
#define MainWidget_H

#include <QWidget>
#include <QButtonGroup>
#include <QSystemTrayIcon>

class UserWidget;
class DeviceBroadcaster;
namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWidget *ui;

    void initializeUi();                            // 初始化界面
    void initializeData();                          // 初始化数据
    void handleEvents();                            // 信号与槽处理
    void createWidgetInContentStackedWidget(QAbstractButton *button);   // 创建需要在内容区 stacked widget 中显示的 widget
    void setupActions();                            // 设置Actions
    void setupSystrayIcon();                        // 设置系统托盘图标
    void setMainWindowVisibility(bool visible);     // 设置窗口可见性

    QButtonGroup *mItemButtons;                     // 成员按钮组
    QHash<QAbstractButton *, QWidget *> mButtonWidgetHash;              // key 是侧边栏切换界面的按钮的指针，value 是右侧 widget 的指针

    DeviceBroadcaster* m_pBroadcaster;              // 广播

    // 系统托盘图标
    QSystemTrayIcon* mSystrayIcon;
    QMenu* mSystrayMenu;
    QAction* mShowMainWindowAction;
    QAction* mSettingsAction;
    QAction* mAboutAction;
    QAction* mAboutQtAction;
    QAction* mQuitAction;

    bool mForceQuit = false;
};

#endif // MainWidget_H
