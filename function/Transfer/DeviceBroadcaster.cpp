#include "devicebroadcaster.h"
#include "../settings.h"
#include <cassert>
#include <QDebug>

DeviceBroadcaster::DeviceBroadcaster(QObject *parent) : QObject(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &DeviceBroadcaster::sendBroadcast);
    connect(&m_udpSock, &QUdpSocket::readyRead, this, &DeviceBroadcaster::processBroadcast);

    m_udpSock.bind(Singleton<Settings>::getInstance().getBroadcastPort(), QUdpSocket::ShareAddress);
}

// 启动广播
void DeviceBroadcaster::start()
{
    sendBroadcast();
    if (!m_timer.isActive())
        m_timer.start(Singleton<Settings>::getInstance().getBroadcastInterval());
}

// 将自己Device信息广播出去
void DeviceBroadcaster::sendBroadcast()
{
    int port = Singleton<Settings>::getInstance().getBroadcastPort();
    Device dev = Singleton<Settings>::getInstance().getMyDevice();
    QJsonObject obj(QJsonObject::fromVariantMap({
                                                    {"id", dev.getId()},
                                                    {"name", dev.getName()},
                                                    {"os", dev.getOSName()},
                                                    {"remarks", dev.getRemarks()},
                                                    {"port", port}
                                                }));

    QVector<QHostAddress> addresses = getBroadcastAddressFromInterfaces();
    QByteArray data(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    foreach (QHostAddress address, addresses) {
        m_udpSock.writeDatagram(data, address, port);
    }
}

// 从网络上其他设备收到的广播过程
void DeviceBroadcaster::processBroadcast()
{
    while (m_udpSock.hasPendingDatagrams()) {
        QByteArray data;
        
        qint64 datagramSize = m_udpSock.pendingDatagramSize();
        assert(datagramSize <= std::numeric_limits <int>::max());

        data.resize(static_cast<int>(datagramSize));
        QHostAddress sender;

        m_udpSock.readDatagram(data.data(), data.size(), &sender);

        QJsonObject obj = QJsonDocument::fromJson(data).object();
        if (obj.keys().length() == 5) {
            if (obj.value("port").toVariant().value<quint16>() ==
                    Singleton<Settings>::getInstance().getBroadcastPort()) {

                Device device{obj.value("name").toString(), obj.value("os").toString(),
                            obj.value("id").toString(), obj.value("remarks").toString(), sender};
                emit broadcastReceived(device);
            }
        }
    }
}

// 要广播自己的地址
QVector<QHostAddress> DeviceBroadcaster::getBroadcastAddressFromInterfaces()
{
    QVector<QHostAddress> addresses;
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces()) {
        if (iface.flags() & QNetworkInterface::CanBroadcast) {
            foreach (QNetworkAddressEntry addressEntry, iface.addressEntries()) {
                if (!addressEntry.broadcast().isNull()) {
                    addresses.push_back(addressEntry.broadcast());
                }
            }
        }
    }
    return addresses;
}
