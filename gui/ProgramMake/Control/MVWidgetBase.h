#ifndef MVWidgetBase_H
#define MVWidgetBase_H

#include <QWidget>
#include "ControlWidget.h"

class VideoPlayerTool;

class MVWidgetBase : public ControlWidget
{
    Q_OBJECT
public:
    explicit MVWidgetBase(QWidget *parent = nullptr);

    void addList(const QStringList &filePaths);             // 增加
    bool remove(int index);                                 // 移除
    void clearList();                                       // 清空列表
    void moveLayerUP(int index);                            // 上移
    void moveLayerDown(int index);                          // 下移

    void setVolume(int volume);                             // 设置音量
    int getVolume() const;                                  // 获取音量
    QStringList getList() const { return m_strListFiles; }  // 获取列表

    QJsonDocument toJson();                                 // 转为Json
    bool toBean(const QJsonDocument &doc);                  // 转为Bean

private:
    void initializeUi();                                    // 初始化界面
    void initializeData();                                  // 初始化数据
    void handleEvents();                                    // 信号与槽处理

    QStringList m_strListFiles;                             // 视频音频文件列表
    VideoPlayerTool *m_pVideoPlayerTool;                    // 视频播放
};

#endif // MVWidgetBase_H
