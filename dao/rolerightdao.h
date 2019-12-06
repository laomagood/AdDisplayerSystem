#ifndef ROLERIGHTDAO_H
#define ROLERIGHTDAO_H

#include "bean/RoleRightBean.h"

class RoleRightDao
{
public:
    RoleRightBean findById(qlonglong roleId);
    QList<RoleRightBean> findBy(const QString &name);
    QList<RoleRightBean> findAll();
    bool insert(RoleRightBean &bean);
    bool updateBy(const RoleRightBean &bean);
    bool deleteBy(const RoleRightBean &bean);
};

#endif // ROLERIGHTDAO_H
