#ifndef CONTROLLISTWIDGET_H
#define CONTROLLISTWIDGET_H

#include <QWidget>
#include <QMetaType>
#include "CanvasWidget.h"
#include "Control/ControlWidget.h"
class QListWidget;

/**
 * @brief The ControlListWidget class
 * 用来显示控件图层
 * 点击选中界面控件（获得焦点）
 * 提供与界面控件右键一样的功能（删除，上移，下移，置顶，置底）
 */

// 右键类型（用于发射信号）
enum MenuOption {
    REMOVECONTROL = 0,
    MOVELAYERUP,
    MOVELAYERDOWN,
    MOVELAYERTOP,
    MOVELAYERBOTTOM
};

// 结构体用于发送信号
struct ST_SIGNAL_DATA {
    ControlWidget *pControlWidget;
    MenuOption menuOption;
};
Q_DECLARE_METATYPE(ST_SIGNAL_DATA)


class ControlListWidget : public QWidget
{
    Q_OBJECT
public:
    ControlListWidget(QWidget *parent = nullptr);
    void setCanvasWidget(CanvasWidget *canvasWidget);   // 设置画布

signals:
    void menuClicked(QVariant variant);                 // 右键菜单信号
    void itemClicked(ControlWidget* pControlWidget);    // item点击信号

private:
    void initializeUi();                                // 初始化界面
    void handleEvents();                                // 信号与槽处理
    void contextMenu();                                 // 右键菜单
    void updateCanvas();                                // 切换画布

    QListWidget *m_pListWidget;                         // List列表
    CanvasWidget *m_pCanvasWidget;                      // 画布指针
    ControlWidget *m_pControlWidget;                    // 控件指针
};

#endif // CONTROLLISTWIDGET_H
