#ifndef ClientManagementModel_H
#define ClientManagementModel_H

#include <QAbstractTableModel>
//#include "Device.h"

class ClientManagementModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ClientManagementModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    bool setHeaderData(int, Qt::Orientation, const QVariant&, int = Qt::EditRole) { return false; }

    //    void loadData();                                                    // 从数据库加载数据
    //    bool findData(const QString &name);                                 // 从数据库查找数据(模糊查询)
    //    bool removeData(const QModelIndex &index);                          // 删除一条数据
    //    bool removeDatas();                                                 // 删除多条数据
    //    bool insertData(const UserBean &user);                              // 插入一条数据
    //    bool updataBy(const QModelIndex &index, const QString &password);   // 更新密码
    //    bool updataBy(const QModelIndex &index, const int &role);           // 更新角色
    //    bool isChecked();                                                   // 是否勾选
    //    UserBean getUserBean(const QModelIndex &index);                     // 由index获取Bean对象

    enum class Column : int {
        CheckBox = 0, SerialNumber, Name, ConnectionStatus,
        System, Id, PlayingProgram, Remarks, Count };

private:
    //    QList<Device> m_deviceList;
};

#endif // ClientManagementModel_H
