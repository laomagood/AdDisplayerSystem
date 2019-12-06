#ifndef SENDER_H
#define SENDER_H

#include "transfer.h"
#include "device.h"

class Sender : public Transfer
{
public:
    Sender(const Device& receiver, const QString& folderName, const QString& filePath, QObject* parent = nullptr);

    bool start();
    Device getReceiver() const { return mReceiverDev; }

private slots:
    void onBytesWritten(qint64 bytes);
    void onConnected();
    void onDisconnected();

private:
    void finish();
    void sendData();
    void sendHeader();

    Device mReceiverDev;
    QString mFilePath;
    QString mFolderName;
    qint64 mFileSize;
    qint64 mBytesRemaining;

    QByteArray mFileBuff;
    qint32 mFileBuffSize;

    bool mCancelled;
    bool mPaused;
    bool mPausedByReceiver;
};

#endif // SENDER_H
