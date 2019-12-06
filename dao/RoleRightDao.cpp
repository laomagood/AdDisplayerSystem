#include "RoleRightDao.h"
#include "bean/RoleRightBean.h"
#include "RoleDao.h"
#include "RightDao.h"
#include <QDebug>

RoleRightBean RoleRightDao::findById(qlonglong roleId)
{
    RoleRightBean roleRightBean;
    RoleDao roleDao;
    RightDao rightDao;

    roleRightBean.setRoleBean(roleDao.findById(roleId));
    roleRightBean.setRightBean(rightDao.findById(roleRightBean.getRoleBean()->getRightId()));
    return roleRightBean;
}

QList<RoleRightBean> RoleRightDao::findBy(const QString &name)
{
    QList<RoleRightBean> roleRightBeanList;
    QList<RoleBean> roleBeanList;
    RoleDao roleDao;
    RightDao rightDao;

    roleBeanList = roleDao.findBy(name);
    for (int i=0; i<roleBeanList.size(); i++) {
        RoleRightBean roleRightBean;
        // [2] 查找RightBean
        RightBean rightBean = rightDao.findById(roleBeanList.at(i).getID());

        roleRightBean.setRoleBean(roleBeanList.at(i));
        roleRightBean.setRightBean(rightBean);
        roleRightBeanList.append(roleRightBean);
    }

    return roleRightBeanList;
}

QList<RoleRightBean> RoleRightDao::findAll()
{
    QList<RoleRightBean> roleRightBeanList;
    QList<RoleBean> roleBeanList;

    RoleDao roleDao;
    RightDao rightDao;

    // [1] 查找RoleBean
    roleBeanList = roleDao.findAll();
    for (int i=0; i<roleBeanList.size(); i++) {
        RoleRightBean roleRightBean;
        // [2] 查找RightBean
        RightBean rightBean = rightDao.findById(roleBeanList.at(i).getRightId());

        roleRightBean.setRoleBean(roleBeanList.at(i));
        roleRightBean.setRightBean(rightBean);
        roleRightBeanList.append(roleRightBean);
    }
    return roleRightBeanList;
}

bool RoleRightDao::insert(RoleRightBean &bean)
{
    RoleDao roleDao;
    RightDao rightDao;

    int rightId = rightDao.insert(*bean.getRightBean());
    bean.getRoleBean()->setRightId(rightId);
    bean.getRightBean()->setID(rightId);

    int roleId = roleDao.insert(*bean.getRoleBean());
    bean.getRoleBean()->setId(roleId);
    return true;
}

bool RoleRightDao::updateBy(const RoleRightBean &bean)
{
    RoleRightBean roleRightBean = bean;
    qDebug() << roleRightBean.getRoleBean()->getID()
             << roleRightBean.getRoleBean()->getRightId()
             << roleRightBean.getRightBean()->getID();
    RoleDao roleDao;
    RightDao rightDao;

    roleDao.updateBy(*roleRightBean.getRoleBean());
    rightDao.updateBy(*roleRightBean.getRightBean());
    return true;
}

bool RoleRightDao::deleteBy(const RoleRightBean &bean)
{
    RoleRightBean roleRightBean = bean;
    RoleDao roleDao;
    RightDao rightDao;
    int rightId = roleRightBean.getRoleBean()->getRightId();
    int roleId = roleRightBean.getRoleBean()->getID();
    qDebug() << "delete" << rightId << roleId;

    if ( !rightDao.deleteById(rightId) )
        return false;
    if ( !roleDao.deleteById(roleId) )
        return false;
    return true;
}

