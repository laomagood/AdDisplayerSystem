#ifndef VideoWidget_H
#define VideoWidget_H

#include "MVWidgetBase.h"

class VideoWidget : public MVWidgetBase
{
    Q_OBJECT
public:
    explicit VideoWidget(QWidget *parent = nullptr);

private:
    void initializeUi();                                        // 初始化数据
    void handleEvents();                                        // 信号与槽处理
};

#endif // VideoWidget_H
