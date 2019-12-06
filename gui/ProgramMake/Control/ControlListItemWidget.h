#ifndef CONTROLLISTITEMWIDGET_H
#define CONTROLLISTITEMWIDGET_H

#include <QWidget>
#include "ControlWidget.h"
class QLabel;

class ControlListItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ControlListItemWidget(QWidget *parent = nullptr);
    void setControlWidget(ControlWidget* controlWidget);
    inline ControlWidget* getControlWidget() const { return m_pControlWidget; }

private:
    void initializeUi();                // 初始化界面
    void initializeData();              // 初始化数据
    void handleEvents();                // 信号与槽处理

    QLabel *m_pControlName;             // 控件名称
    QLabel *m_pControlIcon;             // 控件图标
    ControlWidget *m_pControlWidget;
};

#endif // CONTROLLISTITEMWIDGET_H
