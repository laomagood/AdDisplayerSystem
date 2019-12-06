#ifndef TRANSFERSERVER_H
#define TRANSFERSERVER_H

#include <QTcpServer>
#include <QObject>

#include "receiver.h"
#include "devicelistmodel.h"

class TransferServer : public QObject
{
    Q_OBJECT

public:
    explicit TransferServer(DeviceListModel* devList, QObject *parent = nullptr);

    bool listen(const QHostAddress& addr = QHostAddress::Any);

Q_SIGNALS:
    void newReceiverAdded(Receiver* receiver);

private Q_SLOTS:
    void onNewConnection();

private:
    DeviceListModel* mDevList;
    QTcpServer* mServer;
    QVector<Receiver*> mReceivers;
};

#endif // TRANSFERSERVER_H
