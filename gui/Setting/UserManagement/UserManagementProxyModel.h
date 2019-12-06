#ifndef USERMANAGEMENTPROXYMODEL_H
#define USERMANAGEMENTPROXYMODEL_H

#include <QSortFilterProxyModel>

class UserManagementProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit UserManagementProxyModel(QObject *parent = 0);

    QString getRole() const { return m_strRole; }

public slots:
    void clearFilters();
    void setRole(const QString &role);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

private:
    QString m_strRole;

};

#endif // USERMANAGEMENTPROXYMODEL_H
