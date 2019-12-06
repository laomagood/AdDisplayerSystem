#ifndef IMAGEPROPERTYWIDGET_H
#define IMAGEPROPERTYWIDGET_H

#include <QWidget>
class ImageWidget;

namespace Ui {
class ImagePropertyWidget;
}

class ImagePropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImagePropertyWidget(QWidget *parent = 0);
    ~ImagePropertyWidget();
    void setImageWidget(QWidget *widget);

private:
    Ui::ImagePropertyWidget *ui;
    void initializeUi();                // 初始化界面
    void handleEvents();                // 信号与槽处理

    void getData();                     // 获取数据

    ImageWidget *m_pImageWidget;
};

#endif // IMAGEPROPERTYWIDGET_H
