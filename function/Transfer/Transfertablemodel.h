#ifndef TRANSFERTABLEMODEL_H
#define TRANSFERTABLEMODEL_H

#include <QAbstractTableModel>
#include <QColor>

#include "transfer.h"
#include "transferinfo.h"

class TransferTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TransferTableModel(QObject *parent = nullptr);
    ~TransferTableModel() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void insertTransfer(Transfer* t);
    void clearCompleted();

    Transfer* getTransfer(int index) const;
    TransferInfo* getTransferInfo(int index) const;

    void removeTransfer(int index);

    enum class Column : int {
        Peer = 0, FileName, FileSize, State, Progress,
        Count
    };

private:
    QString getStateString(TransferState state) const;
    QColor getStateColor(TransferState state) const;

    QVector<Transfer*> mTransfers;

};

#endif // TRANSFERTABLEMODEL_H
