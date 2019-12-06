#ifndef IMAGELISTITEMWIDGET_H
#define IMAGELISTITEMWIDGET_H

#include <QWidget>
class QLabel;
class ImageWidget;

class ImageListItemWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageListItemWidget(QWidget *parent = nullptr);
    void setImageFilePath(QString imageFilePath);

private:
    void initializeUi();                // 初始化界面
    void initializeData();              // 初始化数据
    void handleEvents();                // 信号与槽处理

    QLabel *m_pImageIcon;               // 图片缩略图
    QLabel *m_pImageName;               // 图片名称

    QString m_ImageFilePath;            // 图片路径
};

#endif // IMAGELISTITEMWIDGET_H
