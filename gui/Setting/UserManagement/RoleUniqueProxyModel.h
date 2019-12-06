#ifndef ROLEUNIQUEPROXYMODEL_H
#define ROLEUNIQUEPROXYMODEL_H

#include <QSet>
#include <QSortFilterProxyModel>

/**
 * @brief The RoleUniqueProxyModel class
 * 对用户角色进行整理，用来存放到ComboBox中进行过滤筛选
 */
class RoleUniqueProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit RoleUniqueProxyModel(int column, QObject *parent=0)
        : QSortFilterProxyModel(parent), m_nColumn(column), m_nCount(0), m_bFlag(true) {}

    void setSourceModel(QAbstractItemModel *sourceModel);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    const int m_nColumn;
    mutable int m_nCount;
    bool m_bFlag;
    mutable QSet<QString> m_setCache;
};

#endif // ROLEUNIQUEPROXYMODEL_H
