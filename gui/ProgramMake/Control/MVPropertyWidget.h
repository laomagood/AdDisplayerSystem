#ifndef MVPropertyWidget_H
#define MVPropertyWidget_H

#include <QWidget>
class MVWidgetBase;

namespace Ui {
class MVPropertyWidget;
}

class MVPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MVPropertyWidget(QWidget *parent = 0);
    ~MVPropertyWidget();
    void setWidget(QWidget *widget);

private:
    Ui::MVPropertyWidget *ui;
    void initializeUi();                // 初始化界面
    void handleEvents();                // 信号与槽处理

    void getData();                     // 获取数据

    MVWidgetBase *m_pWidget;
};

#endif // MVPropertyWidget_H
