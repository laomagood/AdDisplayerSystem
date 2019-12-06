#include "RoleUniqueProxyModel.h"
#include <QDebug>

void RoleUniqueProxyModel::setSourceModel(QAbstractItemModel *sourceModel)
{
    // 关联源模型，源模型变更清空set，重新filterAcceptsRow
    connect(sourceModel, &QAbstractItemModel::modelReset, [this] {
        m_setCache.clear();
    });
    m_bFlag = false;
    QSortFilterProxyModel::setSourceModel(sourceModel);
}

bool RoleUniqueProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    // 这里有问题，刚开始setSourceModel一次执行ilterAcceptsRow，返回正确值
    // 当源模型更改后会两次执行ilterAcceptsRow，导致set里有唯一的文字，而返回空
    m_nCount ++;
    if (m_nCount == sourceModel()->rowCount() && m_bFlag) {
        qDebug() << "Reset";
        m_setCache.clear();
        m_nCount = 0;
    }

    QModelIndex index = sourceModel()->index(sourceRow, m_nColumn, sourceParent);
    const QString &text = sourceModel()->data(index).toString();
    if (m_setCache.contains(text))
        return false;

    m_setCache << text;
    return true;
}
