#ifndef RoleRightBean_H
#define RoleRightBean_H

#include "RoleBean.h"
#include "RightBean.h"
#include "dao/RoleDao.h"
#include "dao/RightDao.h"

class RoleRightBean
{
public:
    void setRoleBean(const RoleBean &roleBean) { m_nRoleBean = roleBean; }
    void setRightBean(const RightBean &rightBean) { m_nRightBean = rightBean; }

    RoleBean* getRoleBean() { return &m_nRoleBean; }
    RightBean* getRightBean() { return &m_nRightBean; }

private:
    RoleBean m_nRoleBean;
    RightBean m_nRightBean;
};

#endif // RoleRightBean_H
