#include "UserManagementProxyModel.h"
#include "UserManagementModel.h"
UserManagementProxyModel::UserManagementProxyModel(QObject *parent) : QSortFilterProxyModel(parent)
{

}

void UserManagementProxyModel::clearFilters()
{
    m_strRole.clear();
    invalidateFilter();
}

bool UserManagementProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (!m_strRole.isEmpty()) {
        QModelIndex index = sourceModel()->index(sourceRow, (int)UserManagementModel::Column::Role, sourceParent);
        if (m_strRole != sourceModel()->data(index).toString())
            return false;
    }
    return true;
}

void UserManagementProxyModel::setRole(const QString &role)
{
    if (m_strRole != role) {
        m_strRole = role;
        invalidateFilter();
    }
}
