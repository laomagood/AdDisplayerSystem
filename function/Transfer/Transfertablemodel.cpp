#include "transfertablemodel.h"
#include "util/Util.h"

TransferTableModel::TransferTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
}

TransferTableModel::~TransferTableModel()
{
    for (Transfer* t : mTransfers) {
        delete t;
    }
}

int TransferTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mTransfers.size();
}

int TransferTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (int) Column::Count;
}

QVariant TransferTableModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid()) {
        TransferInfo* info = mTransfers.at(index.row())->getTransferInfo();
        QString strFilaName = QFileInfo(info->getFilePath()).fileName();

        if (info) {
            Column col = (Column) index.column();
            if (role == Qt::DisplayRole) {
                switch (col) {
                case Column::Peer : return info->getPeer().getName();
                case Column::FileName : return strFilaName;
                case Column::FileSize : return Util::sizeToString(info->getDataSize());
                case Column::State : return getStateString(info->getState());
                case Column::Progress : return info->getProgress();
                default : break;
                }
            }
            else if (role == Qt::ForegroundRole && col == Column::State) {
                return getStateColor(info->getState());
            }
        }
    }

    return QVariant();
}

QVariant TransferTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        Column col = (Column) section;
        switch (col) {
        case Column::Peer : return tr("客户端");
        case Column::FileName : return tr("文件名称");
        case Column::FileSize : return tr("文件大小");
        case Column::State : return tr("状态");
        case Column::Progress : return tr("进度");
        default : break;
        }
    }

    return QVariant();
}

void TransferTableModel::insertTransfer(Transfer *t)
{
    if (!t) {
        return;
    }

    beginInsertRows(QModelIndex(), 0, 0);
    mTransfers.prepend(t);
    endInsertRows();
    emit dataChanged(index(1, 0), index(mTransfers.size()-1, (int) Column::Count));

    TransferInfo* info = t->getTransferInfo();
    connect(info, &TransferInfo::fileOpened, [=]() {
        int idx = mTransfers.indexOf(info->getTransfer());
        QModelIndex fNameIdx = index(idx, (int) Column::FileName);
        QModelIndex fSizeIdx = index(idx, (int) Column::FileSize);
        emit dataChanged(fNameIdx, fSizeIdx);
    });

    connect(info, &TransferInfo::stateChanged, [=](TransferState state) {
        Q_UNUSED(state);

        int idx = mTransfers.indexOf(info->getTransfer());
        QModelIndex stateIdx = index(idx, (int) Column::State);
        emit dataChanged(stateIdx, stateIdx);
    });
}

void TransferTableModel::clearCompleted()
{
    for (int i = 0; i < mTransfers.size(); i++) {
        Transfer* t = mTransfers.at(i);
        TransferState state = t->getTransferInfo()->getState();
        if (state == TransferState::Idle ||
                state == TransferState::Finish ||
                state == TransferState::Disconnected ) {

            beginRemoveRows(QModelIndex(), i, i);
            mTransfers.remove(i);
            endRemoveRows();
            t->deleteLater();
            i--;
        }
    }
}

Transfer* TransferTableModel::getTransfer(int index) const
{
    if (index < 0 || index >= mTransfers.size()) {
        return nullptr;
    }

    return mTransfers.at(index);
}

TransferInfo* TransferTableModel::getTransferInfo(int index) const
{
    return getTransfer(index)->getTransferInfo();
}

void TransferTableModel::removeTransfer(int index)
{
    if (index < 0 || index >= mTransfers.size()) {
        return;
    }

    beginRemoveRows(QModelIndex(), index, index);
    mTransfers.at(index)->deleteLater();
    mTransfers.remove(index);
    endRemoveRows();
}

QString TransferTableModel::getStateString(TransferState state) const
{
    switch (state) {
    case TransferState::Idle : return tr("Idle");
    case TransferState::Waiting : return tr("Waiting");
    case TransferState::Disconnected : return tr("Disconnected");
    case TransferState::Transfering : return tr("Transfering");
    case TransferState::Finish : return tr("Finish");
    }

    return QString();
}

QColor TransferTableModel::getStateColor(TransferState state) const
{
    switch (state) {
    case TransferState::Idle : return QColor("black");
    case TransferState::Waiting : return QColor("orange");
    case TransferState::Disconnected : return QColor("red");
    case TransferState::Transfering : return QColor("blue");
    case TransferState::Finish : return QColor("green");
    }

    return QColor();
}
