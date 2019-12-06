#include <QPixmap>
#include "devicelistmodel.h"
#include "../settings.h"

DeviceListModel::DeviceListModel(DeviceBroadcaster* deviceBC, QObject* parent)
    : QAbstractListModel(parent)
{
    mDBC = deviceBC;
    if (!mDBC) {
        mDBC = new DeviceBroadcaster(this);
    }

    connect(mDBC, &DeviceBroadcaster::broadcastReceived, this, &DeviceListModel::onBCReceived);
    loadData();
}


int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mDevices.size();
}

int DeviceListModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (int)Column::Count;
}

QVariant DeviceListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            Column col = (Column) section;
            switch (col) {
            case Column::SerialNumber : return tr("序号");
            case Column::Name : return tr("名称");
            case Column::ConnectionStatus : return tr("状态");
            case Column::OSName : return tr("系统");
            case Column::Id : return tr("Mac");
            case Column::PlayingProgram : return tr("正在播放");
            case Column::Remarks : return tr("备注");
            default: break;
            }
        }
    }
    }

    return QVariant();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        int nRow = index.row();
        int nColumn = index.column();
        Device dev = mDevices[index.row()];

        switch (role) {
        case Qt::TextAlignmentRole : {
            return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        case Qt::DisplayRole : {
            Column col = (Column) nColumn;
            switch (col) {
            case Column::SerialNumber : return nRow + 1;
            case Column::Name : return dev.getName();
            case Column::ConnectionStatus : {
                if (dev.getStatus() == ConnectionStatus::Offline)
                    return tr("离线");
                if (dev.getStatus() == ConnectionStatus::Online)
                    return tr("在线");
            }
            case Column::OSName : return dev.getOSName();
            case Column::Id : return dev.getId();
            case Column::PlayingProgram : return dev.getProgram();
            case Column::Remarks : return dev.getRemarks();
            default: break;
            }
        }
        case Qt::ToolTipRole : {
            QString str = dev.getId() + "<br>" +
                    dev.getName() + " (" + dev.getOSName() + ")<br>" +
                    dev.getAddress().toString();
            return str;
        }
        case Qt::DecorationRole : {
            if (nColumn == (int)Column::OSName) {
                QString os = dev.getOSName();
                if (os == "Linux") {
                    return QPixmap("./image/ClientManagement/linux.png");
                } else if (os == "Windows") {
                    return QPixmap("./image/ClientManagement/windows.png");
                } else if (os == "Mac OSX") {
                    return QPixmap("./image/ClientManagement/osx.png");
                }
            }
            return QPixmap();
        }
        case Qt::ForegroundRole : {
            if (nColumn == (int)Column::ConnectionStatus) {
                if (dev.getStatus() == ConnectionStatus::Offline)
                    return QColor("red");
                if (dev.getStatus() == ConnectionStatus::Online)
                    return QColor("green");
            }
            //            return QColor();
        }
        }
    }

    return QVariant();
}

Device DeviceListModel::device(int index) const
{
    if (index < 0 || index >= mDevices.size()) {
        return Device();
    }

    return mDevices.at(index);
}

Device DeviceListModel::device(const QString &id) const
{
    for (Device dev : mDevices) {
        if (dev.getId() == id) {
            return dev;
        }
    }

    return Device();
}

Device DeviceListModel::device(const QHostAddress &address) const
{
    for (Device dev : mDevices) {
        if (dev.getAddress() == address) {
            return dev;
        }
    }

    return Device();
}

QVector<Device> DeviceListModel::getDevices() const
{
    return mDevices;
}

void DeviceListModel::setDevices(const QVector<Device> &devices)
{
    beginResetModel();
    mDevices = devices;
    endResetModel();
}

void DeviceListModel::loadData()
{
    // 从数据库加载数据，这里先模拟一些客服端
    QHostAddress add;
    foreach (QHostAddress address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol &&
                address != QHostAddress::LocalHost) {
            add = address;
            break;
        }
    }

    Device dev01("dev01", "Windows", "mm000011", "1号大门口广告机", add);
    Device dev02("dev02", "Mac OSX", "mm000012", "1号大门口广告机", add);
    Device dev03("dev03", "Linux", "mm000013", "1号大门口广告机", add);
    Device dev04("dev04", "Windows", "mm000014", "1号大门口广告机", add);
    dev01.setConnectionStatus(ConnectionStatus::Online);
    dev02.setConnectionStatus(ConnectionStatus::Online);

    mDevices.append(dev01);
    mDevices.append(dev02);
    mDevices.append(dev03);
    mDevices.append(dev04);

    // 定时器
    for(int i=0; i<mDevices.size(); i++){
        addTimer();
    }
}

void DeviceListModel::onBCReceived(const Device &fromDevice)
{
    // 与自身相同
    QString id = fromDevice.getId();
    if (id == Singleton<Settings>::getInstance().getMyDevice().getId()) {
        return;
    }

    // mDevices是否已经存在,心跳判断
    bool found = false;
    for(Device& dev : mDevices) {
        if (dev.getId() == id) {
            found = true;
            if (dev.getStatus() == ConnectionStatus::Offline) {
                beginResetModel();
                dev.setConnectionStatus(ConnectionStatus::Online);
                endResetModel();
            }
            int index = mDevices.indexOf(dev);
            mTimers.at(index)->start();
            break;
        }
    }

    // 不存在加入：加入数据库,加入Model
    if (!found) {
        beginInsertRows(QModelIndex(), mDevices.size(), mDevices.size());
        mDevices.push_back(fromDevice);
        mDevices.last().setConnectionStatus(ConnectionStatus::Online);
        addTimer();
        endInsertRows();
    }
}

void DeviceListModel::addTimer()
{
    auto pTimer = new QTimer(this);
    pTimer->setInterval(5000);    // 离线心跳
    pTimer->start();
    mTimers.append(pTimer);

    // 超时离线
    connect(pTimer, &QTimer::timeout, [this, pTimer] {
        int index = mTimers.indexOf(pTimer);
        if (index < mDevices.size()) {
            if (mDevices[index].getStatus() == ConnectionStatus::Online) {
                beginResetModel();
                mDevices[index].setConnectionStatus(ConnectionStatus::Offline);
                endResetModel();
            }
        }
    });
}
