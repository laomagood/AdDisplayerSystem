#include "transferinfo.h"

TransferInfo::TransferInfo(Transfer* owner, QObject *parent) :
    QObject(parent),
    mState(TransferState::Idle), mLastState(TransferState::Idle),
    mProgress(0), mDataSize(0), mOwner(owner)
{
}

void TransferInfo::setPeer(Device peer)
{
    mPeer = std::move(peer);
}

void TransferInfo::setState(TransferState newState)
{
    if (newState != mState) {
        TransferState tmp = mState;

        switch (mState) {
        case TransferState::Idle : {
            if (newState == TransferState::Waiting) {
                mState = newState;
                emit stateChanged(mState);
            }
            break;
        }
        case TransferState::Waiting : {
            if (newState == TransferState::Transfering) {
                mState = newState;
                emit stateChanged(mState);
            }
            break;
        }
        case TransferState::Transfering : {
            if (newState == TransferState::Disconnected ||
                    newState == TransferState::Finish) {
                mState = newState;
                emit stateChanged(mState);
            }
            break;
        }
        default:
            break;
        }

        mLastState = tmp;
    }
}

void TransferInfo::setProgress(int newProgress)
{
    if (newProgress != mProgress) {
        mProgress = newProgress;
        emit progressChanged(mProgress);
    }
}

void TransferInfo::setDataSize(qint64 size)
{
    mDataSize = size;
}

void TransferInfo::setFilePath(const QString &fileName)
{
    mFilePath = fileName;
}
