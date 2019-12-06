#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QtDebug>

#include "../Settings.h"
#include "sender.h"

Sender::Sender(const Device& receiver, const QString& folderName, const QString& filePath, QObject* parent)
    : Transfer(nullptr, parent), mReceiverDev(receiver), mFilePath(filePath), mFolderName(folderName)
{
    mFileSize = -1;
    mBytesRemaining = -1;

    mFileBuffSize = Singleton<Settings>::getInstance().getFileBufferSize();
    mFileBuff.resize(mFileBuffSize);

    mCancelled = false;
    mPaused = false;
    mPausedByReceiver = false;

    mInfo->setPeer(receiver);
}

bool Sender::start()
{
    qDebug() << "start";
    mInfo->setFilePath(mFilePath);
    mFile = new QFile(mFilePath, this);
    bool ok = mFile->open(QIODevice::ReadOnly);
    if (ok) {
        mFileSize = mFile->size();
        mInfo->setDataSize(mFileSize);
        mBytesRemaining = mFileSize;
        emit mInfo->fileOpened();
    }

    if (mFileSize > 0) {
        QHostAddress receiverAddress = mReceiverDev.getAddress();
        setSocket(new QTcpSocket(this));
        mSocket->connectToHost(receiverAddress, Singleton<Settings>::getInstance().getTransferPort(), QAbstractSocket::ReadWrite);
        mInfo->setState(TransferState::Waiting);

        connect(mSocket, &QTcpSocket::bytesWritten, this, &Sender::onBytesWritten);
        connect(mSocket, &QTcpSocket::connected, this, &Sender::onConnected);
        connect(mSocket, &QTcpSocket::disconnected, this, &Sender::onDisconnected);
    }

    return ok && mSocket;
}

void Sender::onConnected()
{
    mInfo->setState(TransferState::Transfering);
    sendHeader();
}

void Sender::onDisconnected()
{
    mInfo->setState(TransferState::Disconnected);
    emit mInfo->errorOcurred(tr("Receiver disconnected"));
}

void Sender::onBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);

    if (!mSocket->bytesToWrite()) {
        sendData();
    }
}

void Sender::finish()
{
    mFile->close();
    mInfo->setState(TransferState::Finish);
    emit mInfo->done();

    writePacket(0, PacketType::Finish, QByteArray());
}

void Sender::sendData()
{
    qDebug() << "sendData";
    if (mBytesRemaining < mFileBuffSize) {
        mFileBuff.resize(mBytesRemaining);
        mFileBuffSize = mFileBuff.size();
    }

    qint64 bytesRead = mFile->read(mFileBuff.data(), mFileBuffSize);
    if (bytesRead == -1) {
        emit mInfo->errorOcurred(tr("Error while reading file."));
        return;
    }

    mBytesRemaining -= bytesRead;
    if (mBytesRemaining < 0)
        mBytesRemaining = 0;

    mInfo->setProgress( (int) ((mFileSize-mBytesRemaining) * 100 / mFileSize) );

    writePacket(mFileBuffSize, PacketType::Data, mFileBuff);

    if (!mBytesRemaining) {
        finish();
    }
}

void Sender::sendHeader()
{
    qDebug() << "sendHeader";
    QString fName = QDir(mFile->fileName()).dirName();

    QJsonObject obj( QJsonObject::fromVariantMap({
                                    {"name", fName},
                                    {"folder", mFolderName },
                                    {"size", mFileSize}
                                }));

    QByteArray headerData( QJsonDocument(obj).toJson() );

    writePacket(headerData.size(), PacketType::Header, headerData);
}
