/******************************************************************************
    Simple Player:  this file is part of QtAV examples
    Copyright (C) 2012-2016 Wang Bin <wbsecg1@gmail.com>

*   This file is part of QtAV

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef VideoPlayerTool_H
#define VideoPlayerTool_H

#include <QWidget>
#include <QtAV>
class Slider;
class QPushButton;
class QLabel;

class VideoPlayerTool : public QWidget
{
    Q_OBJECT
public:
    explicit VideoPlayerTool(QWidget *parent = 0);
    void setFilePaths(QStringList filePaths);               // 设置视频音频文件路径
    void setTWidgetVisible(bool visible);                   // 隐藏顶操作层
    void setVolume(int value);                              // 设置声音(0-100)

    QStringList getFilePaths() const { return m_strListFiles; }     // 获取视频音频文件路径
    int getVolume() const { return m_nVolume; }                     // 获取音量

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void initializeUi();                                    // 初始化界面
    void handleEvents();                                    // 信号与槽处理

    QtAV::VideoOutput *m_pVideoOutput;
    QtAV::AVPlayer *m_pPlayer;

    QLabel *m_pTitleLabel;                                  // 标题
    QLabel *m_pVideoTimeLabel;                              // 视频时间
    Slider *m_pTimeSlider;                                 // 播放进度栏
    Slider *m_pVolumeSlider;                               // 音量调节栏
    QPushButton *m_pPlayPauseBtn;                           // 暂停播放按钮
    QPushButton *m_pVolumeBtn;                              // 声音按钮
    QWidget *m_pTWidget;                                    // 顶层widget

    QString m_strVideoCurrentTime;                          // 视频当前时间
    QString m_strVideoTotalTime;                            // 视频总时间
    QStringList m_strListFiles;                             // 文件列表
    QString m_strCurrentFiles;                              // 当前播放文件
    int m_nVolume;                                          // 音量
};

#endif // VideoPlayerTool_H
