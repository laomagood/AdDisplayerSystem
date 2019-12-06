#ifndef TRANSFERINFO_H
#define TRANSFERINFO_H

#include <QObject>

#include "device.h"

enum class TransferState {
    Idle,
    Waiting,
    Disconnected,
    Transfering,
    Finish
};


class Transfer;

class TransferInfo : public QObject
{
    Q_OBJECT

public:
    explicit TransferInfo(Transfer* owner, QObject *parent = nullptr);

    inline Device getPeer() const { return mPeer; }
    inline int getProgress() const { return mProgress; }
    inline TransferState getState() const { return mState; }
    inline TransferState getLastState() const { return mLastState; }
    inline qint64 getDataSize() const { return mDataSize; }
    inline QString getFilePath() const { return mFilePath; }
    inline Transfer* getTransfer() const { return mOwner; }

    void setPeer(Device peer);
    void setState(TransferState state);
    void setProgress(int progress);
    void setDataSize(qint64 size);
    void setFilePath(const QString& fileName);

signals:
    void done();
    void errorOcurred(const QString& errStr);
    void progressChanged(int progress);
    void fileOpened();
    void stateChanged(TransferState state);

private:
    Device mPeer;
    TransferState mState;
    TransferState mLastState;
    int mProgress;
    qint64 mDataSize;
    QString mFilePath;

    Transfer* mOwner;
};

#endif // TRANSFERINFO_H
