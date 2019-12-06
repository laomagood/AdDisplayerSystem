/*
    LANShare - LAN file transfer.
    Copyright (C) 2016 Abdul Aris R. <abdularisrahmanudin10@gmail.com>

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
*/

#ifndef DEVICEBROADCASTER_H
#define DEVICEBROADCASTER_H

#include <QObject>
#include <QTimer>
#include <QtNetwork>

#include "model/device.h"

/*
 * deviceBroadcaster用于广播设备，方法是使用UDP协议发送包含设备信息的数据包数据
 */
class DeviceBroadcaster : public QObject
{
    Q_OBJECT

public:
    explicit DeviceBroadcaster(QObject *parent = nullptr);

Q_SIGNALS:
    void broadcastReceived(const Device& fromDevice);

public Q_SLOTS:
    void start();
    void sendBroadcast();   // 发送广播

private Q_SLOTS:
    void processBroadcast();

private:
    QVector<QHostAddress> getBroadcastAddressFromInterfaces();

    QTimer mTimer;
    QUdpSocket mUdpSock;
};

#endif // DEVICEBROADCASTER_H
