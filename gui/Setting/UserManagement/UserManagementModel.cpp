#include "UserManagementModel.h"
#include "dao/UserDao.h"
#include <QColor>
#include <qDebug>

UserManagementModel::UserManagementModel(QObject *parent) : QAbstractTableModel(parent)
{
    loadData();
}

int UserManagementModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_userBeanList.count();
}

int UserManagementModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return (int)Column::Count;
}

Qt::ItemFlags UserManagementModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    if (index.column() == (int)Column::CheckBox) {
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    } else if (index.column() == (int)Column::Edit) {
        flags |= Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;
    }

    return flags;
}

// 将数据从模型取出放到视图显示
QVariant UserManagementModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    Column nColumn = (Column) index.column();
    UserBean record = m_userBeanList.at(nRow);

    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::black);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (nColumn == Column::CheckBox )
            return "";
        if (nColumn == Column::SerialNumber)
            return nRow + 1;
        if (nColumn == Column::LoginName)
            return record.getName();
        if (nColumn == Column::GenerateTime)
            return record.getStrGenerateTime();
        if (nColumn == Column::LoginTime)
            return record.getStrLoginTime();
        if (nColumn == Column::LastLoginTime)
            return record.getStrLastLoginTime();
        if (nColumn == Column::Role)
            return record.getRoleName();
        if (nColumn == Column::Edit)
            return "";
    }
    case Qt::UserRole:
    {
        if (nColumn == Column::CheckBox )
            return record.getCheck();
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 将视图更改后的数据存储到模型
bool UserManagementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
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
        if (nColumn == (int)Column::CheckBox)
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
}

// 设置标题-----------------------------------------------------
QVariant UserManagementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            Column nColumn = (Column) section;
            if (nColumn == Column::CheckBox)
                return tr("选择");
            if (nColumn == Column::SerialNumber)
                return tr("序号");
            if (nColumn == Column::LoginName)
                return tr("用户名");
            if (nColumn == Column::GenerateTime)
                return tr("用户创建时间");
            if (nColumn == Column::LoginTime)
                return tr("最近登录时间");
            if (nColumn == Column::LastLoginTime)
                return tr("上次登录时间");
            if (nColumn == Column::Role)
                return tr("角色");
            if (nColumn == Column::Edit)
                return tr("编辑");
        }
    }
    default:
        return QVariant();
    }

    return QVariant();
}

void UserManagementModel::loadData()
{
    UserDao userDao;
    m_userBeanList.clear();
    m_userBeanList = userDao.findAll();
    beginResetModel();
    endResetModel();
}

bool UserManagementModel::findData(const QString &name)
{
    UserDao userDao;
    m_userBeanList.clear();
    m_userBeanList = userDao.findBy(name);
    beginResetModel();
    endResetModel();
    return m_userBeanList.size() == 0 ? false : true;
}

bool UserManagementModel::removeData(const QModelIndex &index)
{
    int nRow = index.row();
    qlonglong id = m_userBeanList.at(nRow).getID();
    UserDao userDao;
    if (!userDao.deleteById(id)) {
        return false;
    }
    m_userBeanList.removeAt(nRow);
    beginResetModel();
    endResetModel();
    return true;
}

bool UserManagementModel::removeDatas()
{
    for (int i=m_userBeanList.size()-1; i>0; i--) {
        bool bChecked = m_userBeanList.at(i).getCheck();
        if (bChecked) {
            qlonglong id = m_userBeanList.at(i).getID();
            UserDao userDao;
            if (!userDao.deleteById(id)) {
                continue;
            }
            m_userBeanList.removeAt(i);
        }
    }
    beginResetModel();
    endResetModel();
    return true;
}

bool UserManagementModel::insertData(const UserBean &user)
{
    UserDao userDao;
    if (!userDao.insert(user)) {
        return false;
    }
    m_userBeanList.append(user);
    beginResetModel();
    endResetModel();
    return true;
}

bool UserManagementModel::updataBy(const QModelIndex &index, const QString &password)
{
    int nRow = index.row();
    UserBean userBean = m_userBeanList.at(nRow);
    UserDao userDao;
    if (!userDao.updateBy(userBean.getID(), password)) {
        return false;
    }
    return true;
}

bool UserManagementModel::updataBy(const QModelIndex &index, const int &roleId)
{
    int nRow = index.row();
    UserBean userBean = m_userBeanList.at(nRow);
    UserDao userDao;
    if (!userDao.updateBy(userBean.getID(), roleId)) {
        return false;
    }
    userBean = userDao.findById(userBean.getID());
    m_userBeanList.replace(nRow, userBean);
    beginResetModel();
    endResetModel();
    return true;
}

bool UserManagementModel::isChecked()
{
    for (int i=0; i<m_userBeanList.size(); i++) {
        bool bChecked = m_userBeanList.at(i).getCheck();
        if (bChecked) {
            return true;
        }
    }
    return false;
}

UserBean UserManagementModel::getUserBean(const QModelIndex &index)
{
    int nRow = index.row();
    return m_userBeanList.at(nRow);
}
