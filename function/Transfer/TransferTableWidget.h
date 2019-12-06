#ifndef TRANSFERTABLEWIDGET_H
#define TRANSFERTABLEWIDGET_H

#include <QWidget>
#include <QAbstractItemModel>

namespace Ui {
class TransferTableWidget;
}

class TransferTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransferTableWidget(QAbstractItemModel *model, QWidget *parent = 0);
    ~TransferTableWidget();

    void setName(QString name);
    void setSize(qint64 size);
    void setFileNum(int num);
    void setModel(QAbstractItemModel *model);

    void setIndexWidget(const QModelIndex &index, QWidget *widget);
    void scrollToTop();

private:
    Ui::TransferTableWidget *ui;
    QAbstractItemModel *m_pModel;
};

#endif // TRANSFERTABLEWIDGET_H
