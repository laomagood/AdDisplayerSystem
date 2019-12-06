#ifndef TEXTPROPERTYWIDGET_H
#define TEXTPROPERTYWIDGET_H

#include <QWidget>
class TextWidget;

namespace Ui {
class TextPropertyWidget;
}

/**
 * @brief The TextPropertyWidget class
 * 文字工具的属性栏
 */

class TextPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TextPropertyWidget(QWidget *parent = 0);
    ~TextPropertyWidget();
    void setTextWidget(QWidget *widget);

private:
    Ui::TextPropertyWidget *ui;
    void initializeUi();                // 初始化界面
    void handleEvents();                // 信号与槽处理
    void setTextBtnColor(QColor color); // 设置字体按钮颜色
    void setBGBtnColor(QColor color);   // 设置背景按钮颜色

    void getData();                     // 获取数据

    TextWidget *m_pTextWidget;

};

#endif // TEXTPROPERTYWIDGET_H
