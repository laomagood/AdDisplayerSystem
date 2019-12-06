#ifndef ImageWidget_H
#define ImageWidget_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QJsonDocument>
#include "ControlWidget.h"
class QPropertyAnimation;

/**
 * @brief The ImageWidget class
 * 图片轮播的类，提供了3种图片的切换类型
 * 可以设置图片切换时间与切换类型，增加图片，调整图片的顺序（需实现）
 */
class ImageWidget : public ControlWidget
{
    Q_OBJECT
public:
    enum ImageChangeType{       // 切换类型:
        LeftToRight = 0,        // 1.从左到右
        RightToLeft,            // 2.从右到左
        Opacity                 // 3.透明度
    };

    explicit ImageWidget(QWidget *parent = nullptr);
    ~ImageWidget();

    void addList(const QStringList &fileNameList);              // 增加图片
    bool remove(int index);                                     // 移除图片
    void clearList();                                           // 清空图片列表
    void moveLayerUP(int index);                                // 上移图片
    void moveLayerDown(int index);                              // 下移图片
    void setImageChangeTime(int imageChangeTime);               // 设置图片切换时间
    void setImageChangeType(ImageChangeType imageChangeType);   // 设置图片切换类型

    QStringList getImageList() const;                           // 获取图片列表
    int getImageChangeTime() const;                             // 获取图片切换时间
    ImageChangeType getImageChangeType() const;                 // 获取图片切换类型

    QJsonDocument toJson();                                     // 转为Json
    bool toBean(const QJsonDocument &doc);                      // 转为Bean

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

public slots:
    void onImageChangeTimeout();

private:
    void initializeData();                  // 初始化数据
    void handleEvents();                    // 信号与槽处理

    QStringList m_imageFileNameList;        // 图片列表
    QTimer m_imageChangeTimer;              // 定时器
    int m_imageChangeTime;                  // 图片切换时间
    ImageChangeType m_imageChangeType;      // 图片切换类型

    int m_currentDrawImageIndx;             // 当前图片索引
    QPixmap m_currentPixmap;                // 当前图片
    QPixmap m_nextPixmap;                   // 下一张图片

    QPropertyAnimation* m_opacityAnimation;
    QPropertyAnimation* m_positionAnimation;
};

#endif // ImageWidget_H
