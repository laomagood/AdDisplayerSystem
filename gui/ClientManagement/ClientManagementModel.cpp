#include "ClientManagementModel.h"
#include "dao/UserDao.h"
#include <QColor>
#include <qDebug>

ClientManagementModel::ClientManagementModel(QObject *parent) : QAbstractTableModel(parent)
{
//    loadData();
}

int ClientManagementModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8;
//    return m_userBeanList.count();
}

int ClientManagementModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (int)Column::Count;
}

Qt::ItemFlags ClientManagementModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == (int)Column::CheckBox) {
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return flags;
}

// 将数据从模型取出放到视图显示
QVariant ClientManagementModel::data(const QModelIndex &index, int role) const
{
    /*
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    UserBean record = m_userBeanList.at(nRow);

    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::black);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (nColumn == CheckBoxColumn )
            return "";
        if (nColumn == SerialNumberColumn)
            return nRow + 1;
        if (nColumn == LoginNameColumn)
            return record.getName();
        if (nColumn == GenerateTimeColumn)
            return record.getStrGenerateTime();
        if (nColumn == LoginTimeColumn)
            return record.getStrLoginTime();
        if (nColumn == LastLoginTimeColumn)
            return record.getStrLastLoginTime();
        if (nColumn == RoleColumn)
            return record.getRoleName();
        if (nColumn == EditColumn )
            return "";
    }
    case Qt::UserRole:
    {
        if (nColumn == CheckBoxColumn )
            return record.getCheck();
    }
    default:
        return QVariant();
    }
    */
    return QVariant();

}

// 将视图更改后的数据存储到模型
bool ClientManagementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    /*
    if (!index.isValid())
        return false;

    int nRow = index.row();
    int nColumn = index.column();
    UserBean record = m_userBeanList.at(nRow);
    switch (role)
    {
    case Qt::DisplayRole:
    {
        return "";
    }
    case Qt::CheckStateRole:
    case Qt::UserRole:
    {
        if (nColumn == Column::CheckBox)
        {
            record.setCheck(value.toBool());
            m_userBeanList.replace(index.row(), record);
            emit dataChanged(index, index);
            return true;
        }
    }
    default:
        return false;
    }
    return false;
    */
}

// 设置标题-----------------------------------------------------
QVariant ClientManagementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            Column col = (Column) section;
            switch (col) {
            case Column::CheckBox : return tr("选择");
            case Column::SerialNumber : return tr("序号");
            case Column::Name : return tr("名称");
            case Column::ConnectionStatus : return tr("状态");
            case Column::System : return tr("系统");
            case Column::Id : return tr("ID");
            case Column::PlayingProgram : return tr("正在播放");
            case Column::Remarks : return tr("备注");
            default: break;
            }
        }
    }
    }

    return QVariant();
}

//void ClientManagementModel::loadData()
//{
//    UserDao userDao;
//    m_userBeanList.clear();
//    m_userBeanList = userDao.findAll();
//    beginResetModel();
//    endResetModel();
//}

//bool ClientManagementModel::findData(const QString &name)
//{
//    UserDao userDao;
//    m_userBeanList.clear();
//    m_userBeanList = userDao.findBy(name);
//    beginResetModel();
//    endResetModel();
//    return m_userBeanList.size() == 0 ? false : true;
//}

//bool ClientManagementModel::removeData(const QModelIndex &index)
//{
//    int nRow = index.row();
//    qlonglong id = m_userBeanList.at(nRow).getID();
//    UserDao userDao;
//    if (!userDao.deleteById(id)) {
//        return false;
//    }
//    m_userBeanList.removeAt(nRow);
//    beginResetModel();
//    endResetModel();

//}

//bool ClientManagementModel::removeDatas()
//{
//    for (int i=m_userBeanList.size()-1; i>0; i--) {
//        bool bChecked = m_userBeanList.at(i).getCheck();
//        if (bChecked) {
//            qlonglong id = m_userBeanList.at(i).getID();
//            UserDao userDao;
//            if (!userDao.deleteById(id)) {
//                continue;
//            }
//            m_userBeanList.removeAt(i);
//        }
//    }
//    beginResetModel();
//    endResetModel();
//    return true;
//}

//bool ClientManagementModel::insertData(const UserBean &user)
//{
//    UserDao userDao;
//    if (!userDao.insert(user)) {
//        return false;
//    }
//    m_userBeanList.append(user);
//    beginResetModel();
//    endResetModel();
//    return true;
//}

//bool ClientManagementModel::updataBy(const QModelIndex &index, const QString &password)
//{
//    int nRow = index.row();
//    UserBean userBean = m_userBeanList.at(nRow);
//    UserDao userDao;
//    if (!userDao.updateBy(userBean.getID(), password)) {
//        return false;
//    }
//    return true;
//}

//bool ClientManagementModel::updataBy(const QModelIndex &index, const int &roleId)
//{
//    int nRow = index.row();
//    UserBean userBean = m_userBeanList.at(nRow);
//    UserDao userDao;
//    if (!userDao.updateBy(userBean.getID(), roleId)) {
//        return false;
//    }
//    userBean = userDao.findById(userBean.getID());
//    m_userBeanList.replace(nRow, userBean);
//    beginResetModel();
//    endResetModel();
//    return true;
//}

//bool ClientManagementModel::isChecked()
//{
//    for (int i=0; i<m_userBeanList.size(); i++) {
//        bool bChecked = m_userBeanList.at(i).getCheck();
//        if (bChecked) {
//            return true;
//        }
//    }
//    return false;
//}

//UserBean ClientManagementModel::getUserBean(const QModelIndex &index)
//{
//    int nRow = index.row();
//    return m_userBeanList.at(nRow);
//}
