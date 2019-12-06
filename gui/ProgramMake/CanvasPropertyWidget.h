#ifndef CanvasPropertyWidget_H
#define CanvasPropertyWidget_H

#include <QWidget>
class CanvasWidget;

namespace Ui {
class CanvasPropertyWidget;
}

/**
 * @brief The CanvasPropertyWidget class
 * 画布属性栏大小与背景颜色的设定
 */

class CanvasPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasPropertyWidget(QWidget *parent = 0);
    ~CanvasPropertyWidget();
    void setCanvasWidget(QWidget *widget);

private:
    Ui::CanvasPropertyWidget *ui;
    void initializeUi();                    // 初始化界面
    void handleEvents();                    // 信号与槽处理
    void setBGBtnColor(QColor color);       // 背景颜色选择

    void getData();                         // 获取数据

    CanvasWidget *m_pCanvasWidget;          // 画布
};

#endif // CanvasPropertyWidget_H
