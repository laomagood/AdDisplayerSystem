#ifndef SETTINGS_H
#define SETTINGS_H

#include <QHostAddress>
#include <QString>
#include "Transfer/Device.h"
#include "util/Singleton.h"


#if defined (Q_OS_WIN)
#define OS_NAME "Windows"
#elif defined (Q_OS_OSX)
#define OS_NAME "Mac OSX"
#elif defined (Q_OS_LINUX)
#define OS_NAME "Linux"
#else
#define OS_NAME "Unknown"
#endif

const QString PROGRAM_NAME = "AdDisplayerSystem";
const QString PROGRAM_DESC = "多媒体广告系统将视频、音频信号、图片信息和滚动字幕通过网络传输到终端，"
                             "然后在终端以全高清的方式播出，除了播放各种广告之外，还可以实时播放新闻、"
                             "天气预报、紧急通知等即时信息，在第一时间将最新鲜的资讯传递给受众人群。";
constexpr int PROGRAM_X_VER = 1;
constexpr int PROGRAM_Y_VER = 1;
constexpr int PROGRAM_Z_VER = 1;
const QString SETTINGS_FILE ="AdDisplayerConfig";

/**
 * @brief The Settings class
 * 保存需要保存的设置
 */

class Settings
{
    SINGLETON(Settings)

public:
    quint16 getBroadcastPort() const;
    quint16 getTransferPort() const;
    quint16 getBroadcastInterval() const;
    qint32 getFileBufferSize() const;
    QString getDownloadDir() const;

    Device getMyDevice() const;
    QString getDeviceId() const;
    QString getDeviceName() const;
    QHostAddress getDeviceAddress() const;
    bool getReplaceExistingFile() const;
    
    void setDeviceName(const QString& name);
    void setBroadcastPort(quint16 port);
    void setTransferPort(quint16 port);
    void setBroadcastInterval(quint16 interval);
    void setFileBufferSize(qint32 size);
    void setDownloadDir(const QString& dir);
    void setReplaceExistingFile(bool replace);

    void saveSettings();
    void reset();

private:
    void loadSettings();
    QString getDefaultDownloadPath();       // 获取默认下载地址
    QString getMacInfo();                   // 获取Mac信息

private:
    Device m_thisDevice;                    // 设备
    quint16 m_BCPort = 0;                   // 广播端口
    quint16 m_transferPort = 0;             // 传输端口
    quint16 m_BCInterval = 0;               // 广播间隔
    qint32 m_fileBuffSize = 0;              // 文件Buff大小
    QString m_downloadDir;                  // 下载地址
    bool m_replaceExistingFile = false;     // 替换已经存在的文件
};

#endif // SETTINGS_H
