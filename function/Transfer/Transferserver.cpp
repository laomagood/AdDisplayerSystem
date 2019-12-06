#include "transferserver.h"
#include "../settings.h"

TransferServer::TransferServer(DeviceListModel* devList, QObject *parent) : QObject(parent)
{
    mDevList = devList;
    mServer = new QTcpServer(this);
    connect(mServer, &QTcpServer::newConnection, this, &TransferServer::onNewConnection);
}

bool TransferServer::listen(const QHostAddress &addr)
{
    return mServer->listen(addr, Singleton<Settings>::getInstance().getTransferPort());
}

void TransferServer::onNewConnection()
{
    QTcpSocket* socket = mServer->nextPendingConnection();
    if (socket) {
        Device dev = mDevList->device(socket->peerAddress());
        Receiver* rec = new Receiver(dev, socket);
        mReceivers.push_back(rec);
        emit newReceiverAdded(rec);
    }
}
