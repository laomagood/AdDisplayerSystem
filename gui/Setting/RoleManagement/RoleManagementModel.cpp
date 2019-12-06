#include "RoleManagementModel.h"
#include "global.h"
#include "dao/RoleRightDao.h"
#include <QColor>
#include <qDebug>

RoleManagementModel::RoleManagementModel(QObject *parent) : QAbstractTableModel(parent)
{
    loadData();
}

int RoleManagementModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_roleRightBeanList.count();
}

int RoleManagementModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

Qt::ItemFlags RoleManagementModel::flags(const QModelIndex &index) const
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
QVariant RoleManagementModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int nRow = index.row();
    int nColumn = index.column();
    RoleRightBean bean = m_roleRightBeanList.at(nRow);
    RoleBean *pRoleBean = bean.getRoleBean();

    switch (role)
    {
    case Qt::TextColorRole:
        return QColor(Qt::black);
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (nColumn == (int)Column::CheckBox )
            return "";
        if (nColumn == (int)Column::SerialNumber)
            return nRow + 1;
        if (nColumn == (int)Column::RoleName)
            return pRoleBean->getName();
        if (nColumn == (int)Column::GenerateTime)
            return pRoleBean->getStrGenerateTime();
        if (nColumn == (int)Column::UpdateTime)
            return pRoleBean->getUpdateTime();
        if (nColumn == (int)Column::Description)
            return pRoleBean->getDescription();
        if (nColumn == (int)Column::Edit )
            return "";
    }
    case Qt::UserRole:
    {
        if (nColumn == (int)Column::CheckBox)
            return pRoleBean->getCheck();
    }
    default:
        return QVariant();
    }

    return QVariant();
}

// 将视图更改后的数据存储到模型
bool RoleManagementModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    int nRow = index.row();
    int nColumn = index.column();
    RoleRightBean roleRightBean = m_roleRightBeanList.at(nRow);
    RoleBean *pRoleBean = roleRightBean.getRoleBean();
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
            pRoleBean->setCheck(value.toBool());
            m_roleRightBeanList.replace(index.row(), roleRightBean);
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
QVariant RoleManagementModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::TextAlignmentRole:
        return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
    case Qt::DisplayRole:
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == (int)Column::CheckBox)
                return tr("选择");
            if (section == (int)Column::SerialNumber)
                return tr("序号");
            if (section == (int)Column::RoleName)
                return tr("角色名");
            if (section == (int)Column::GenerateTime)
                return tr("角色创建时间");
            if (section == (int)Column::UpdateTime)
                return tr("角色更新时间");
            if (section == (int)Column::Description)
                return tr("角色描述");
            if (section == (int)Column::Edit)
                return tr("编辑");
        }
    }
    default:
        return QVariant();
    }

    return QVariant();
}

void RoleManagementModel::loadData()
{
    RoleRightDao roleRightDao;
    m_roleRightBeanList.clear();
    m_roleRightBeanList = roleRightDao.findAll();
    beginResetModel();
    endResetModel();
}

bool RoleManagementModel::findData(const QString &name)
{
    RoleRightDao roleRightDao;
    m_roleRightBeanList.clear();
    m_roleRightBeanList = roleRightDao.findBy(name);
    beginResetModel();
    endResetModel();
    return m_roleRightBeanList.size() == 0 ? false : true;
}

bool RoleManagementModel::removeData(const QModelIndex &index)
{
    int nRow = index.row();
    RoleRightDao roleRightDao;
    if (!roleRightDao.deleteBy(m_roleRightBeanList.at(nRow))) {
        return false;
    }
    m_roleRightBeanList.removeAt(nRow);
    beginResetModel();
    endResetModel();
    return true;
}

bool RoleManagementModel::removeDatas()
{
    for (int i=m_roleRightBeanList.size()-1; i>0; i--) {
        bool bChecked = m_roleRightBeanList[i].getRoleBean()->getCheck();
        if (bChecked) {
            RoleRightDao roleRightDao;
            if (!roleRightDao.deleteBy(m_roleRightBeanList.at(i))) {
                continue;
            }
            m_roleRightBeanList.removeAt(i);
        }
    }
    beginResetModel();
    endResetModel();
    return true;
}

bool RoleManagementModel::insertData(RoleRightBean &bean)
{
    RoleRightDao roleRightDao;
    if (!roleRightDao.insert(bean)) {
        return false;
    }
    m_roleRightBeanList.append(bean);
    beginResetModel();
    endResetModel();
    return true;
}

bool RoleManagementModel::updataBy(const QModelIndex &index, const RoleRightBean &bean)
{
    int nRow = index.row();
    RoleRightBean roleRightBean = m_roleRightBeanList.at(nRow);
    RoleRightDao roleRightDao;
    if (!roleRightDao.updateBy(bean)) {
        return false;
    }
    m_roleRightBeanList.replace(nRow, roleRightBean);
    beginResetModel();
    endResetModel();
    return true;
}

bool RoleManagementModel::isChecked()
{
    for (int i=0; i<m_roleRightBeanList.size(); i++) {
        bool bChecked = m_roleRightBeanList[i].getRoleBean()->getCheck();
        if (bChecked) {
            return true;
        }
    }
    return false;
}

RoleRightBean RoleManagementModel::getRoleRightBean(const QModelIndex &index)
{
    int nRow = index.row();
    return m_roleRightBeanList[nRow];
}

int RoleManagementModel::getRoleIdByName(const QString &name)
{
    for (int i=0; i<m_roleRightBeanList.size(); i++) {
        if (name == m_roleRightBeanList[i].getRoleBean()->getName()) {
            return m_roleRightBeanList[i].getRoleBean()->getID();
        }
    }
    return -1;
}
