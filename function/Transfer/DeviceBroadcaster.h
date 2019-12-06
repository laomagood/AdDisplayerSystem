#ifndef DEVICEBROADCASTER_H
#define DEVICEBROADCASTER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork>
#include "device.h"

/**
 * @brief The DeviceBroadcaster class
 * 服务器广播将自己的信息发出去，局域网中的客户端接收信息连接服务器
 * 1.用来更新客户端现在正在播放的节目
 * 2.相当心跳机制
 */
class DeviceBroadcaster : public QObject
{
    Q_OBJECT

public:
    explicit DeviceBroadcaster(QObject *parent = nullptr);

public slots:
    void start();
    void sendBroadcast();

signals:
    void broadcastReceived(const Device& fromDevice);

private slots:
    void processBroadcast();

private:
    QVector<QHostAddress> getBroadcastAddressFromInterfaces();

    QTimer m_timer;
    QUdpSocket m_udpSock;
};

#endif // DEVICEBROADCASTER_H
