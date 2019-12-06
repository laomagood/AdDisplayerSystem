#ifndef RoleDao_H
#define RoleDao_H

#include <QString>
class RoleBean;

class RoleDao
{
public:
    RoleBean findById(qlonglong id);
    QList<RoleBean> findAll();
    QList<RoleBean> findBy(const QString &name);          // 模糊查询
    bool isExistName(QString name);                // 查询是否存在

    int insert(const RoleBean &roleBean);
    bool updateBy(const RoleBean &roleBean);

    bool deleteById(qlonglong id);
};

#endif // RoleDao_H
