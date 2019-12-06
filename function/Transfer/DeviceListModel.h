#ifndef DEVICELISTMODEL_H
#define DEVICELISTMODEL_H

#include <QAbstractListModel>

#include "devicebroadcaster.h"
#include "device.h"

/**
 * @brief The DeviceListModel class
 * 设备列表模型
 * 1.先从数据库加载 2.接收广播更新设备连接状态及正在播放节目(新设备连接则加入到数据库中)
 */
class DeviceListModel : public QAbstractListModel
{
public:
    DeviceListModel(DeviceBroadcaster* deviceBC, QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Device device(int index) const;
    Device device(const QString& id) const;
    Device device(const QHostAddress& address) const;
    QVector<Device> getDevices() const;
    void setDevices(const QVector<Device> &getDevices);

    void loadData();        // 加载数据

    enum class Column : int {
        SerialNumber = 0, Name, ConnectionStatus,
        OSName, Id, PlayingProgram, Remarks, Count };

private slots:
    void onBCReceived(const Device &fromDevice);
    void addTimer();

private:
    DeviceBroadcaster* mDBC;
    QVector<Device> mDevices;        // 客户端
    QVector<QTimer*> mTimers;        // 离线计时器
};

#endif // DEVICELISTMODEL_H
