#ifndef MusicWidget_H
#define MusicWidget_H

#include <QWidget>
#include "MVWidgetBase.h"

class MusicWidget : public MVWidgetBase
{
    Q_OBJECT
public:
    explicit MusicWidget(QWidget *parent = nullptr);

private:
    void initializeUi();                                        // 初始化数据
    void handleEvents();                                        // 信号与槽处理
};

#endif // MusicWidget_H
