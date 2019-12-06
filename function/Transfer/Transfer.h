#ifndef TRANSFER_H
#define TRANSFER_H

#include <QFile>
#include <QTcpSocket>
#include <QObject>

#include "device.h"
#include "transferinfo.h"

enum class PacketType : char
{
    Header = 0x01,
    Data,
    Finish,
    Oreder
};

class Transfer : public QObject
{
    Q_OBJECT

public:
    Transfer(QTcpSocket* socket, QObject* parent = nullptr);

    inline QFile* getFile() const { return mFile; }
    inline QTcpSocket* getSocket() const { return mSocket; }
    inline TransferInfo* getTransferInfo() const { return mInfo; }

protected:
    void clearReadBuffer();
    void setSocket(QTcpSocket* socket);

    virtual void processPacket(QByteArray& data, PacketType type);
    virtual void processHeaderPacket(QByteArray& data);
    virtual void processDataPacket(QByteArray& data);
    virtual void processFinishPacket(QByteArray& data);

    virtual void writePacket(qint32 packetDataSize, PacketType type, const QByteArray& data);

    QFile *mFile;
    QTcpSocket* mSocket;
    TransferInfo* mInfo;

private slots:
    void onReadyRead();

private:
    QByteArray mBuff;               // 读ByteArray
    qint32 mPacketSize;             // 读取包大小

    int mHeaderSize;                // 文件头大小
};

#endif // TRANSFER_H
